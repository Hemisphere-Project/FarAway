#include <FastLED.h>
#include "FastLED_RGBW.h"   // https://www.partsnotincluded.com/fastled-rgbw-neopixels-sk6812/

#define NUM_LEDS 150
#define DATA_PIN 26
#define BRIGHTNESS  255

int pos = 0;
int master = 0;
int inc = 2;

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

void leds_setup() {
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
    leds_clear();
    leds_show();
}

bool leds_clear() {
    for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGBW(0,0,0,0);
}

void leds_show() {
    FastLED.show();
}

bool leds_all(int value) {
    value = min(255, max(0, value));
    for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGBW(value,0,0,0);
    leds_show();
}
