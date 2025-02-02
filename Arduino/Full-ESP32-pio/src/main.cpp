#include <Arduino.h>
#define FA_VERSION  1     // PROTO
#define FA_VERSION  2     // PROTO PCB
#define FA_VERSION  5     // WEDNESDAY
#define FA_VERSION  6     // INSTALL METZ
#define FA_VERSION  8     // METZ AOUT (watchdog stepper_shake)
#define FA_VERSION  9     // PRE-BXL (config watchdog)
#define FA_VERSION  11    // FIX Watchdog
#define FA_VERSION  12    // PIO version
#define FA_VERSION  13    // Fix flexyCore

// Config (one time Burn): it is then stored in EEPROM !
//
// #define K32_SET_NODEID    1

enum States {STOP, MOVE, FREE, END, ACCU, SLOW};

#include "K32-lite/K32.h"
#include "anim_leds.h"
K32 *k32;

#include "ESP-FlexyStepper-1.4.3/ESP_FlexyStepper.h"
ESP_FlexyStepper stepper;

#include "debug.h"
#include "audio.h"
#include "stepper.h"
#include "liddar.h"


#define TRIGGER_WATCH_TIME 15*60   // Restart ESP if not trigger for that time (in seconds)   // 0 = disable watchdog restart

#define PIN_LEDSTRIP  4
#define PIN_REDDOT    27
#define STRIP_TYPE    LED_SK6812W_V1  // LED_WS2812_V1  LED_WS2812B_V1  LED_WS2812B_V2  LED_WS2812B_V3  LED_WS2813_V1  LED_WS2813_V2   LED_WS2813_V3  LED_WS2813_V4  LED_SK6812_V1  LED_SK6812W_V1,
#define STRIP_SIZE    135


float maxSpeed = 1;
float lastSpeed = 0;
float debugSpeed = -1;
int master = 0;

int lastTrigger = 0;

unsigned long lastDot = 0;
bool redDotState = false;


void setup()
{
  LOGSETUP();
  LOG("STARTING..");

  // K32 Lib
  k32 = new K32();

  // LIDDAR
  liddar_setup();

  // STEPPER
  if (k32->system->id() == 6) stepper_setup(); // Accel factor reduced for ESP-6
  else stepper_setup();


  // WIFI
  k32->init_wifi("faraway-v" + String(FA_VERSION));
  k32->wifi->connect("hmsphr", "hemiproject");

  // STRIP
  k32->light->addStrip(PIN_LEDSTRIP, (led_types)STRIP_TYPE, STRIP_SIZE);

  // ANIMATIONS
  // k32->light->anim( 0, "test0",   new K32_anim_test )->push(200)->master(60)->play()->wait();
  k32->light->anim( 0, "color",    new Anim_color)->push(0, 0, 0, 0);
  k32->light->anim( 0, "runner",   new Anim_runner)->push(0);
  k32->light->anim( 0, "chaser",   new Anim_chaser)->push(6000, 10, 500)->play();

  // RED DOT
  pinMode(PIN_REDDOT, OUTPUT);
  digitalWrite(PIN_REDDOT, HIGH);

  stepper_slowrun();

  // AUDIO
  audio_setup();

  // WDT
  esp_task_wdt_init(3, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);

  LOG("READY");
}

int dec = 255;
int inc = -1;


void loop()
{
  // OTA in progress
  if (k32->wifi->otaState == ERROR) ESP.restart();
  else if (k32->wifi->otaState > OFF) {
    stepper_breaker();
    k32->light->anim("chaser")->stop();
    k32->light->anim("runner")->stop();
    k32->light->anim("color")->push(0, 0, 50, 0)->play();

    for (int k = 0; k < 10; k++) {
      esp_task_wdt_reset();
      delay(100);
    }

    return;
  }

  // RED DOT
  if ((millis() - lastDot) > 1000)
  {
    if (!redDotState) {
      digitalWrite(PIN_REDDOT, HIGH);
      redDotState = true;
    }
    else if ((millis() - lastDot) > 1100) {
      digitalWrite(PIN_REDDOT, LOW);
      redDotState = false;
      lastDot = millis();
    }
  }

  // AUDIO
  audio_loop();

  // TRIG
  // if (millis()-lastTrigger > 15000) { lastTrigger = millis();

  if (liddar_check()) {
    audio_play( (stepper_state() == SLOW) );

    if (stepper_state() == SLOW)
    {
      stepper_kick();
      k32->light->anim("runner")->mod("load")->trigger();
      maxSpeed = 1;
    }
  }

  master = 255;
  float speed = stepper.getCurrentVelocityInStepsPerSecond();
  if (speed < 0) speed *= -1;
  if (speed > maxSpeed) maxSpeed = speed;

  if (lastSpeed < speed) master = 255;          // Accelerate -> full brightness
  else {
    master = min(255, 255 * speed / maxSpeed); // Decelerate -> proportional brightness
  }
  lastSpeed = speed;

  if (stepper_state() == MOVE || stepper_state() == FREE) {
    k32->light->anim("chaser")->stop();
    // k32->light->anim("color")->push(master, master, master, master)->play();
    k32->light->anim("runner")->push(master)->play();
  }
  else {
    // k32->light->anim("color")->stop();
    k32->light->anim("runner")->stop();
    k32->light->anim("chaser")->push(6000, 10)->play();
  }

  // Feed dog
  if ( stepper_watchdog(TRIGGER_WATCH_TIME) ) 
    esp_task_wdt_reset();

  delay(2);
}
