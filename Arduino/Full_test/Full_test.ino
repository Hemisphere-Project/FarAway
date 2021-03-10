#define DEBUGFLAG

#define K32_SET_NODEID      1
#define K32_SET_HWREVISION  1

#include <AccelStepper.h>
#include "src/K32-lite/K32.h"
#include "anim_leds.h"
#include "debug.h"


#define PIN_LEDSTRIP  18
#define PIN_LEDDOT    19
#define STRIP_TYPE    LED_SK6812W_V1  // LED_WS2812_V1  LED_WS2812B_V1  LED_WS2812B_V2  LED_WS2812B_V3  LED_WS2813_V1  LED_WS2813_V2   LED_WS2813_V3  LED_WS2813_V4  LED_SK6812_V1  LED_SK6812W_V1,
#define STRIP_SIZE    180

int dirPin = 17;
int pulPin = 4;

AccelStepper* stepper;
K32 *k32;


void setup()
{
  // K32 Lib
  k32 = new K32();

  // STEPPER
  stepper = new AccelStepper(AccelStepper::DRIVER, pulPin, dirPin);
  stepper->setMaxSpeed(100);
  stepper->setAcceleration(200);//200
  stepper->moveTo(200);

  // LEDS
  k32->light->addStrip(PIN_LEDSTRIP, (led_types)STRIP_TYPE, STRIP_SIZE);
  k32->light->start();

  // LEDS TEST
  k32->light->anim( 0, "test0",   new K32_anim_test )->push(300)->master(60)->play()->wait();
  k32->light->anim( 0, "color",   new K32_anim_color);
  k32->light->anim( 0, "chaser",   new Anim_chaser)->push(5000)->play();
  Serial.println("leds ok");

  // WIFI
  k32->init_wifi("faraway");
  LOG("wifi inited");
  // k32->wifi->staticIP("10.2.100." + String(k32->system->id()), "10.2.0.1", "255.255.0.0");
  // k32->wifi->connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");  ############ BLOCKING (Serial ??)
  LOG("wifi connecting");
}

void loop()
{

  if (stepper->distanceToGo() == 0){
    delay(1000);
    stepper->moveTo(-stepper->currentPosition());
  }

  stepper->run();

  delay(1);

  // LOG(stepper->currentPosition());
  // stepper->runSpeed();

}
