/*
  K32_leds.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
#ifndef K32_leds_h
#define K32_leds_h

#define LEDSTRIP_MAXPIXEL 500

#include <Arduino.h>

#include "_librmt/esp32_digital_led_lib.h"
#include "_libfast/crgbw.h"


class K32_ledstrip {
  public:
    K32_ledstrip(int chan, int pin, int type, int size);

    int size();
    
    void lock();
    void unlock();

    K32_ledstrip* clear();
    K32_ledstrip* black();
    K32_ledstrip* all(pixelColor_t color);
    K32_ledstrip* all(int red, int green, int blue, int white = 0);
    K32_ledstrip* pix(int pixel, pixelColor_t color);
    K32_ledstrip* pix(int pixelStart, int count, pixelColor_t color);
    K32_ledstrip* pix(int pixel, int red, int green, int blue, int white = 0);

    void getBuffer(pixelColor_t* buffer, int size, int offset=0);
    void setBuffer(pixelColor_t* buffer, int size, int offset=0);

    void show();

  private:

    strand_t* _strand;
    pixelColor_t* _buffer;
    
    bool dirty;

    SemaphoreHandle_t buffer_lock;
    SemaphoreHandle_t show_lock;
    SemaphoreHandle_t draw_lock;

    static void draw( void * parameter );

};

#endif
