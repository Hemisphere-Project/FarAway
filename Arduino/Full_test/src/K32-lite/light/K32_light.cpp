/*
  K32_light.cpp
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/

#include "K32_light.h"


K32_light::K32_light() 
{
  digitalLeds_init();

}

void K32_light::start() {
  LOG("start light");
   xTaskCreate( this->refresh,           // function
                    "modulate_task",            // task name
                    10000,                   // stack memory
                    (void*)this,            // args
                    3,                      // priority
                    NULL );                 // handler
}

void K32_light::addStrip(const int pin, led_types type, int size)
{
  if (size == 0) size = LEDSTRIP_MAXPIXEL;

  if (this->_nstrips >= LEDS_MAXSTRIPS) {
    LOG("LEDS: no more strip can be attached..");
    return;
  }

  int s = this->_nstrips;
  this->_nstrips += 1;
  
  this->_strips[s] = new K32_ledstrip(s, pin, type, size);
}

// link every strip to masterStrip
void K32_light::cloneStrips(int masterStrip) {
  if (masterStrip < this->_nstrips) this->_masterClone = masterStrip;
}

// link every strip to masterStrip
void K32_light::copyStrip(stripcopy copy) {
  if (_copyMax<LEDS_MAX_COPY) 
  {
    _copylist[_copyMax] = copy;
    _copyMax += 1;
  }
}

K32_ledstrip* K32_light::strip(int s) {
  return this->_strips[s];
}

K32_light* K32_light::strips() {
  return this;
}

K32_light* K32_light::black()
{
  for (int s = 0; s < this->_nstrips; s++) this->_strips[s]->black();
  return this;
}

K32_light* K32_light::all(pixelColor_t color)
{
  for (int s = 0; s < this->_nstrips; s++) this->_strips[s]->all(color);
  return this;
}

K32_light* K32_light::all(int red, int green, int blue, int white)
{
  return this->all( pixelFromRGBW(red, green, blue, white) );
}

K32_light* K32_light::pix(int pixel, pixelColor_t color) {
  for (int s = 0; s < this->_nstrips; s++) this->_strips[s]->pix(pixel, color);
  return this;
}

K32_light* K32_light::pix(int pixel, int red, int green, int blue, int white) {
  return this->pix( pixel, pixelFromRGBW(red, green, blue, white) );
}

void K32_light::show() {

  // Clone from master strip (if _masterClone exist)
  pixelColor_t* cloneBuffer = NULL;
  int cloneSize = 0;
  if (this->_masterClone >= 0) {
    cloneSize = this->_strips[this->_masterClone]->size();
    cloneBuffer = static_cast<pixelColor_t*>(malloc(cloneSize * sizeof(pixelColor_t)));
    this->_strips[this->_masterClone]->getBuffer(cloneBuffer, cloneSize);
  }

  for (int s=0; s<this->_nstrips; s++)  
  {
    // Clone from master strip
    if (cloneSize > 0) this->_strips[s]->setBuffer(cloneBuffer, cloneSize); 

    // Copy pixels
    for (int c=0; c<_copyMax; c++) {
      if (_copylist[c].destStrip == s) {
        pixelColor_t* copyBuffer = NULL;
        int copySize = 0;
        copySize = _copylist[c].srcStop-_copylist[c].srcStart+1;
        copyBuffer = static_cast<pixelColor_t*>(malloc(copySize * sizeof(pixelColor_t)));
        this->_strips[_copylist[c].srcStrip]->getBuffer(copyBuffer, copySize, _copylist[c].srcStart);
        this->_strips[s]->setBuffer(copyBuffer, copySize, _copylist[c].destPos); 
        free(copyBuffer);
      }
    }  

    this->_strips[s]->show();
  }

  if (cloneBuffer) free(cloneBuffer);
}


void K32_light::blackout() {
  this->stop();
  this->black();
}



// ANIM
//

// register new anim
K32_anim* K32_light::anim( int stripN, String animName, K32_anim* anim, int size, int offset ) 
{

  if (stripN >= this->_nstrips) {
    LOGF("ERROR: strip %i does not exist\n", stripN);
    return anim;
  }

  if (this->_animcounter >= LEDS_ANIMS_SLOTS) {
    LOG("ERROR: no more slot available to register new animation");
    return anim;
  }
  
  anim->name(animName);
  anim->setup( this->strip(stripN), size, offset );

  this->_anims[ this->_animcounter ] = anim;
  this->_animcounter++;
  // LOGINL("ANIM: register "); LOG(anim->name());

  return anim;
}

// get registered anim
K32_anim* K32_light::anim( String animName) 
{
  for (int k=0; k<this->_animcounter; k++)
    if (this->_anims[k]->name() == animName) {
      // LOGINL("LIGHT: "); LOG(name);
      return this->_anims[k];
    }
  LOGINL("ANIM: not found "); LOG(animName);
  return new K32_anim();

}

// stop all
void K32_light::stop() {
  for (int k=0; k<this->_animcounter; k++) this->_anims[k]->stop();
}

// Set FPS
void K32_light::fps(int f) {
  if (f >= 0) _fps = f;
  else _fps = LEDS_SHOW_FPS;
}

/*
 *   PRIVATE
 */


int K32_light::_nstrips = 0;

// thread function
void K32_light::refresh( void * parameter ) 
{
  K32_light* that = (K32_light*) parameter;
  while(true) 
  {
    vTaskDelay( pdMS_TO_TICKS( 1000/that->_fps ) );
    that->show();
  }
  
  vTaskDelete(NULL);
}

 /////////////////////////////////////////////
