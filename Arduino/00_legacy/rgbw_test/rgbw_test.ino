#include <FastLED.h>
#include "FastLED_RGBW.h"

// https://www.partsnotincluded.com/fastled-rgbw-neopixels-sk6812/
// https://gist.github.com/dmurph/c650904699d0b3a6db3dfefd8c2fded4

// How many leds in your strip?
#define NUM_LEDS 60
#define DATA_PIN 12
#define BRIGHTNESS  255

int pos = 0;
int master = 0;
int inc = 2;

// FastLED with RGB
// CRGB leds[NUM_LEDS];
// FastLED with RGBW
CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

void setup() {

  Serial.begin(115200);
  // FastLED with RGB
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  //FastLED with RGBW
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
}

void loop() {

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGBW(0,0,0,0);
  }

  // Snake
  // pos++;
  // if (pos>NUM_LEDS-8) pos = 0;
  // leds[pos+7] = CRGBW(250,250,250,250);
  // leds[pos+6] = CRGBW(250,250,250,250);
  // leds[pos+5] = CRGBW(250,250,250,250);
  // leds[pos+4] = CRGBW(200,200,200,200);
  // leds[pos+3] = CRGBW(150,150,150,150);
  // leds[pos+2] = CRGBW(100,100,100,100);
  // leds[pos+1] = CRGBW(50,50,50,50);
  // leds[pos] = CRGBW(20,20,20,20);

  // Wave
  // master=master+inc;
  // if((master>253)||(master<=0)){inc=-1*inc;}
  // for (int i = 0; i < NUM_LEDS; i++){
  //   leds[i] = CRGBW(0,0,0,master);
  // }

  for (int i = 0; i < NUM_LEDS; i++){
    // leds[i] = CRGBW(0,0,0,255); // 1.26A -- W - un peu jaune
    // leds[i] = CRGBW(255,255,255,255); // 2.8A -- RGBW - blanc bien
    leds[i] = CRGBW(255,255,255,0); // 1.8A -- RGB -  blanc bleuté
    // leds[i] = CRGBW(255,0,0,0); // 0.73A -- R
    // leds[i] = CRGBW(0,255,0,0); // 0.73A -- G
    // leds[i] = CRGBW(0,0,255,0); // 0.73A -- B
  }

  FastLED.show();
  delay(5);




}
