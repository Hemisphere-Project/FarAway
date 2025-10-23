/*
  K32_leds.cpp
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/

#include "K32_ledstrip.h"


K32_ledstrip::K32_ledstrip(int chan, int pin, int type, int size) {

  this->buffer_lock = xSemaphoreCreateMutex();
  this->draw_lock = xSemaphoreCreateBinary();
  this->show_lock = xSemaphoreCreateBinary();
  xSemaphoreTake(this->draw_lock, 1);
  xSemaphoreTake(this->show_lock, 1);

  this->_strand = digitalLeds_addStrand(
    {.rmtChannel = chan, .gpioNum = pin, .ledType = type, .brightLimit = 255, .numPixels = size, .pixels = nullptr, ._stateVars = nullptr});

  this->_buffer = static_cast<pixelColor_t*>(malloc(this->_strand->numPixels * sizeof(pixelColor_t)));

  // LOOP task
  xTaskCreatePinnedToCore(this->draw,       // function
              "leds_show_task", // task name
              1000,             // stack memory
              (void *)this,     // args
              4,                // priority
              NULL, 1);            // handler
  
  // LOG("-- init strip");
  this->black();
}

int K32_ledstrip::size() {
  return this->_strand->numPixels;
}

void K32_ledstrip::lock() {
  xSemaphoreTake(this->show_lock, portMAX_DELAY);
}

void K32_ledstrip::unlock() {
  xSemaphoreGive(this->show_lock);
}

K32_ledstrip *K32_ledstrip::clear() {

  xSemaphoreTake(this->buffer_lock, portMAX_DELAY);
  memset(this->_buffer, 0, this->size() * sizeof(pixelColor_t));
  // for(int i=0; i < this->size(); i++) this->_buffer[i] = {0,0,0};
  this->dirty = true;
  xSemaphoreGive(this->buffer_lock);

  return this;
}

K32_ledstrip *K32_ledstrip::black() {
  this->clear()->show();
  return this;
}

K32_ledstrip* K32_ledstrip::all(pixelColor_t color) {

  xSemaphoreTake(this->buffer_lock, portMAX_DELAY);
  for(int k= 0; k<this->size(); k++) this->_buffer[k] = color;
  this->dirty = true;
  xSemaphoreGive(this->buffer_lock);

  return this;
}

K32_ledstrip* K32_ledstrip::all(int red, int green, int blue, int white) {
  return this->all( pixelFromRGBW(red, green, blue, white) );
}

K32_ledstrip* K32_ledstrip::pix(int pixelStart, int count, pixelColor_t color) {
    
  xSemaphoreTake(this->buffer_lock, portMAX_DELAY);
  for(int i = pixelStart; i<pixelStart+count; i++)
    if (i < this->size()) this->_buffer[i] = color;
  this->dirty = true;
  xSemaphoreGive(this->buffer_lock);

  return this;
}

K32_ledstrip* K32_ledstrip::pix(int pixel, pixelColor_t color) {
  if (pixel < this->size()) 
  {
    xSemaphoreTake(this->buffer_lock, portMAX_DELAY);
    this->_buffer[pixel] = color;
    this->dirty = true;
    xSemaphoreGive(this->buffer_lock);
  }
  return this;
}

K32_ledstrip* K32_ledstrip::pix(int pixel, int red, int green, int blue, int white) {
  return this->pix( pixel, pixelFromRGBW(red, green, blue, white) );
}

void K32_ledstrip::getBuffer(pixelColor_t* buffer, int _size, int offset) {
  xSemaphoreTake(this->buffer_lock, portMAX_DELAY);
  for(int k= 0; (k<this->size() && k<_size); k++) 
    buffer[k] = this->_buffer[k+offset];
  xSemaphoreGive(this->buffer_lock);
}

void K32_ledstrip::setBuffer(pixelColor_t* buffer, int _size, int offset) {
  xSemaphoreTake(this->buffer_lock, portMAX_DELAY);
  for(int k= 0; (k<this->size() && k<_size); k++) 
    this->_buffer[k+offset] = buffer[k];
  this->dirty = true;
  xSemaphoreGive(this->buffer_lock);
}

void K32_ledstrip::show() {
  // LOG("LIGHT: show in");
  
  // Wait until previous transmission is complete
  xSemaphoreTake(this->show_lock, portMAX_DELAY);
  
  // Copy buffer to strand pixels
  xSemaphoreTake(this->buffer_lock, portMAX_DELAY);
  if (this->dirty) {
    // FIX: sizeof(this->_buffer) was wrong! _buffer is a pointer, so sizeof() returns 4/8 bytes not buffer size
    // This was copying only first few pixels, leaving rest as garbage → causing random color flashes!
    memcpy(this->_strand->pixels, this->_buffer, this->size() * sizeof(pixelColor_t));
    this->dirty = false;
    xSemaphoreGive(this->buffer_lock);
    
    // Trigger draw task (which will release show_lock when done)
    xSemaphoreGive(this->draw_lock);
  }
  else {
    xSemaphoreGive(this->buffer_lock);
    xSemaphoreGive(this->show_lock);
  }
}

void K32_ledstrip::draw(void *parameter)
{
  K32_ledstrip *that = (K32_ledstrip *)parameter;

  // ready - initial state
  xSemaphoreGive(that->show_lock);

  // run
  while (true) {
    // Wait for show() to signal there's data to transmit
    xSemaphoreTake(that->draw_lock, portMAX_DELAY);
    
    // Transmit to LEDs (this blocks until RMT transmission completes)
    digitalLeds_updatePixels(that->_strand);
    delay(1);

    // Release show_lock to allow next show() call
    xSemaphoreGive(that->show_lock);
  }
  vTaskDelete(NULL);
}




