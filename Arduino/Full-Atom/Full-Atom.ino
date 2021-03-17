
#define K32_SET_NODEID      0
#define K32_SET_HWREVISION  1


#include "debug.h"

#include <ESP_FlexyStepper.h>   // https://github.com/pkerspe/ESP-FlexyStepper
ESP_FlexyStepper stepper;

#include "src/K32-lite/K32.h"
#include "anim_leds.h"

#define PIN_LEDSTRIP  26
#define PIN_LEDDOT    19
#define STRIP_TYPE    LED_SK6812W_V1  // LED_WS2812_V1  LED_WS2812B_V1  LED_WS2812B_V2  LED_WS2812B_V3  LED_WS2813_V1  LED_WS2813_V2   LED_WS2813_V3  LED_WS2813_V4  LED_SK6812_V1  LED_SK6812W_V1,
#define STRIP_SIZE    180

K32 *k32;

float maxSpeed = 0;

void setup()
{
  
  
  // K32 Lib
  k32 = new K32();

  // STEPPER
  stepper_setup();

  // WIFI
  k32->init_wifi("faraway-leds");
  k32->wifi->connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");

  // STRIP
  k32->light->addStrip(PIN_LEDSTRIP, (led_types)STRIP_TYPE, STRIP_SIZE);

  // ANIMATIONS  
  k32->light->anim( 0, "test0",   new K32_anim_test )->push(300)->master(60)->play()->wait();
  k32->light->anim( 0, "color",   new K32_anim_color)->push(255,255,255,255)->play();
  // k32->light->anim( 0, "chaser",   new Anim_chaser)->push(5000)->play();

  // DEBUG
  k32->timer->every(100, []() {
    debugD("speed: %.3f", stepper.getCurrentVelocityInStepsPerSecond());
  });

  // LIDDAR
  liddar_setup();

  

}

int dec = 255;
int inc = -1;

void loop()
{
  // bool trig = liddar_check();

  // if (trig) k32->light->anim("color")->push(100);
  // else k32->light->anim("color")->push(0);

  delay(10);
  stepper_loop();

  float speed = stepper.getCurrentVelocityInStepsPerSecond();
  if (speed < 0) speed *= -1;
  if (speed > maxSpeed) maxSpeed = speed;

  int master = min(255, 100*speed/maxSpeed);
  // k32->light->anim("color")->push(master);
}

