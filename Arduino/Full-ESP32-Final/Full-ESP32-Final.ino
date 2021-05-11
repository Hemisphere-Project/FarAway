#define FA_VERSION  1     // PROTO
#define FA_VERSION  2     // PROTO PCB

// Config (one time Burn): it is then stored in EEPROM !
//
#define K32_SET_NODEID    1

enum States {STOP, MOVE, FREE, END, ACCU, SLOW};

#include "debug.h"

#include <ESP_FlexyStepper.h>   // https://github.com/pkerspe/ESP-FlexyStepper
ESP_FlexyStepper stepper;

#include "src/K32-lite/K32.h"
#include "anim_leds.h"


#define PIN_LEDSTRIP  4
#define PIN_REDDOT    27
#define STRIP_TYPE    LED_SK6812W_V1  // LED_WS2812_V1  LED_WS2812B_V1  LED_WS2812B_V2  LED_WS2812B_V3  LED_WS2813_V1  LED_WS2813_V2   LED_WS2813_V3  LED_WS2813_V4  LED_SK6812_V1  LED_SK6812W_V1,
#define STRIP_SIZE    140

K32 *k32;

float maxSpeed = 1;
float lastSpeed = 0;
float debugSpeed = -1;
int master = 0;

void setup()
{
  // K32 Lib
  k32 = new K32();

  // LIDDAR
  liddar_setup();

  // STEPPER
  stepper_setup();

  // WIFI
  k32->init_wifi("faraway-v"+String(FA_VERSION));
  k32->wifi->connect("hmsphr", "hemiproject");
  // k32->wifi->connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");
  // k32->wifi->connect("interweb", "superspeed37");

  // STRIP
  k32->light->addStrip(PIN_LEDSTRIP, (led_types)STRIP_TYPE, STRIP_SIZE);

  // ANIMATIONS  
  k32->light->anim( 0, "test0",   new K32_anim_test )->push(200)->master(60)->play()->wait();
  k32->light->anim( 0, "color",   new K32_anim_color)->push(0,0,0,0)->play();
  k32->light->anim( 0, "chaser",   new Anim_chaser)->push(1000, 5, 100);

  // DEBUG
  k32->timer->every(300, []() {
    float speed = stepper.getCurrentVelocityInStepsPerSecond();
    if (speed != debugSpeed) {
      debugD("speed: %.3f", speed);
      debugSpeed = speed;
    }
  });

  // RED DOT
  pinMode(PIN_REDDOT, OUTPUT);
  k32->timer->every(1000, []() {
    digitalWrite(PIN_REDDOT, HIGH);
    delay(200);
    digitalWrite(PIN_REDDOT, LOW);
  });

  stepper_slowrun();

  // AUDIO
  audio_setup();
  k32->timer->every(2000, []() {
    if (stepper_state() == MOVE) audio_play();
  });
}

int dec = 255;
int inc = -1;

void loop()
{
  // OTA in progress
  if (k32->wifi->otaState == ERROR) ESP.restart();
  else if (k32->wifi->otaState > OFF) {
    stepper_breaker();
    delay(1000);
    k32->light->anim("chaser")->stop();
    k32->light->anim("color")->push(0, 0, 50, 0)->play();
    return;
  }

  // AUDIO
  audio_loop();

  if (liddar_check()) stepper_kick();

  float speed = stepper.getCurrentVelocityInStepsPerSecond();
  if (speed < 0) speed *= -1;
  if (speed > maxSpeed) maxSpeed = speed;

  if (lastSpeed<speed) master = 255;            // Accelerate -> full brightness
  else {
    master = min(255, 255*speed/maxSpeed);   // Decelerate -> proportional brightness
    //if (stepper_time() > 2000) stepper_enable(false);
  }
  lastSpeed = speed;

  if (stepper_state() == MOVE || stepper_state() == FREE) {
    k32->light->anim("chaser")->stop();
    k32->light->anim("color")->push(master, master, master, master)->play();
  }
  else if (stepper_state() == ACCU) {
    k32->light->anim("color")->stop();
    k32->light->anim("chaser")->push(2000, STRIP_SIZE/2, 40)->play();
  }
  else {
    // k32->light->anim("chaser")->stop();
    // k32->light->anim("color")->push(100, 0, 0, 0)->play();
    k32->light->anim("color")->stop();
    k32->light->anim("chaser")->push(12000, 5, 40)->play();
  }

  delay(2);
}

