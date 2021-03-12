
#define K32_SET_NODEID      0
#define K32_SET_HWREVISION  1


#include "debug.h"

// #include <AccelStepper.h>
#include <SpeedyStepper.h>
#include "src/K32-lite/K32.h"
#include "anim_leds.h"



#define PIN_LEDSTRIP  18
#define PIN_LEDDOT    19
#define STRIP_TYPE    LED_SK6812W_V1  // LED_WS2812_V1  LED_WS2812B_V1  LED_WS2812B_V2  LED_WS2812B_V3  LED_WS2813_V1  LED_WS2813_V2   LED_WS2813_V3  LED_WS2813_V4  LED_SK6812_V1  LED_SK6812W_V1,
#define STRIP_SIZE    180

int dirPin = 17;
int pulPin = 4;

// AccelStepper* stepper;
SpeedyStepper* stepper;
K32 *k32;

enum Steps {STOP, MOVE, DECEL};
Steps step = STOP;


void setup()
{
  // K32 Lib
  k32 = new K32();

  // STEPPER
  // stepper = new AccelStepper(AccelStepper::DRIVER, pulPin, dirPin);
  // stepper->setMaxSpeed(700);
  // stepper->setAcceleration(400);//200

  // STEPPER 2
  stepper = new SpeedyStepper();
  stepper->connectToPins(pulPin, dirPin);
  stepper->setStepsPerRevolution(4800);
  stepper->setSpeedInRevolutionsPerSecond(0.2);
  stepper->setAccelerationInRevolutionsPerSecondPerSecond(0.2);


  // WIFI
  k32->init_wifi("faraway");
    // k32->wifi->staticIP("10.2.100." + String(k32->system->id()), "10.2.0.1", "255.255.0.0");
  k32->wifi->connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");

  // LEDS
  k32->light->addStrip(PIN_LEDSTRIP, (led_types)STRIP_TYPE, STRIP_SIZE);

  // ANIMATIONS  
  k32->light->anim( 0, "test0",   new K32_anim_test )->push(300)->master(60)->play()->wait();
  // k32->light->anim( 0, "color",   new K32_anim_color)->push(100,100,100,100)->play();
  k32->light->anim( 0, "chaser",   new Anim_chaser)->push(5000)->play();

  // DEBUG
  k32->timer->every(300, []() {
    
    debugD("pos: %d, %d, %d", stepper->getCurrentPositionInRevolutions(), stepper->getCurrentVelocityInRevolutionsPerSecond(), step);

  });

}

void loop()
{
  
  if (step == STOP) {
    delay(5000);
    triggerStart();
  }

  if (step == MOVE && stepper->getCurrentPositionInRevolutions() > 0.5) {
    triggerDecel();
  }

  if (step == DECEL && stepper->getCurrentVelocityInRevolutionsPerSecond() == 0) {
    step = STOP;
  }


  // Decelerate
  // if (side != (stepper->currentPosition() > 0)) {
  //   stepper->setAcceleration(50);
  //   side = (stepper->currentPosition() > 0);
  // }

  // if (stepper->distanceToGo() == 0){
  //   delay(5000);
  //   stepper->setAcceleration(4000);
  //   stepper->moveTo(-stepper->currentPosition());
  //   side = (stepper->currentPosition() > 0);
  // }


  delay(1);
}


void triggerStart() {
  stepper->setAccelerationInRevolutionsPerSecondPerSecond(0.2);
  stepper->moveRelativeInRevolutions(1.0);
  step = MOVE;
}

void triggerDecel() {
  stepper->setAccelerationInRevolutionsPerSecondPerSecond(0.01);
  step = DECEL;
}