
#define K32_SET_NODEID      0
#define K32_SET_HWREVISION  1


#include "debug.h"

// #include <ESP_FlexyStepper.h>   // https://github.com/pkerspe/ESP-FlexyStepper
// ESP_FlexyStepper stepper;

// #define PIN_LEDSTRIP  18
// #define PIN_LEDDOT    19
// #define STRIP_TYPE    LED_SK6812W_V1  // LED_WS2812_V1  LED_WS2812B_V1  LED_WS2812B_V2  LED_WS2812B_V3  LED_WS2813_V1  LED_WS2813_V2   LED_WS2813_V3  LED_WS2813_V4  LED_SK6812_V1  LED_SK6812W_V1,
// #define STRIP_SIZE    180

float maxSpeed = 0;


void setup()
{
  // STEPPER
  // stepper_setup();

  // WIFI
  wifi_ota( "faraway-proto");
  wifi_maxTry(20);
  wifi_connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");

  // LEDS manual
  leds_setup();
  leds_all(255);
}

int dec = 255;

void loop()
{
  wifi_loop();
  // stepper_loop();
  delay(10);

  // float speed = stepper.getCurrentVelocityInStepsPerSecond();
  // if (speed < 0) speed *= -1;
  // if (speed > maxSpeed) maxSpeed = speed;

  // int master = min(255, 100*speed/maxSpeed);

  dec -= 1;
  if (dec < 0) dec = 255;
  // leds_all(dec);  

  leds_all(dec);
}

