#include "Arduino.h"

int pulPin = 13;
int dirPin = 12;
int enablePin = 14;

int stepsPerRevolution = 25600*2;

States state = STOP;
unsigned long lastRunAt = 0;

float targetRun = 1.2;
bool enableState = false;

int speed;
int accel;
int decel;

unsigned long accu_trig;

void stepper_enable(bool doEnable)
{
    enableState = doEnable;
    digitalWrite(enablePin, !enableState);
}

void stepper_breaker()
{
    stepper_enable(false);
    if (stepper.isStartedAsService()) stepper.stopService();
}

void stepper_slowrun()
{
    state = SLOW;

    stepper_enable(false);
    float target = random(5,7)*targetRun/abs(targetRun);
    stepper.setTargetPositionRelativeInRevolutions(target);
    stepper.setSpeedInStepsPerSecond(speed * (0.9+random(30)/100) / 12);
    stepper_enable(true);
}

bool stepper_accumulate()
{
    if (state != ACCU) accu_trig = millis();
    state = ACCU;
    return (millis()-accu_trig < 1000);
}

void stepper_offTask(void* param)
{
    if (state == MOVE || state == FREE) {
        stepper_slowrun();
        while(stepper_accumulate()) delay(10);
    }

    stepper_slowrun();
    vTaskDelete( NULL );
}

void stepper_setup()
{
    // set the speed and acceleration rates for the stepper motor
    speed = stepsPerRevolution*0.14;                //*0.14;

    accel = 0.055*stepsPerRevolution;
    decel = 0.015*stepsPerRevolution/targetRun;       //0.15*speed/targetRun;

    // set enable Pin
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, enableState);

    stepper_enable(false);

    // connect and configure the stepper motor to its IO pins
    stepper.connectToPins(pulPin, dirPin);
    stepper.setStepsPerRevolution(stepsPerRevolution);

    if (k32->system->id() == 6) { // Accel factor reduced for ESP-6
        stepper.setAccelerationInStepsPerSecondPerSecond(accel/2);
        stepper.setDecelerationInStepsPerSecondPerSecond(decel);
    }
    else {
        stepper.setAccelerationInStepsPerSecondPerSecond(accel);
        stepper.setDecelerationInStepsPerSecondPerSecond(decel);
    }


    // Reach destination Callback
    stepper.registerTargetPositionReachedCallback([](long position){
        xTaskCreatePinnedToCore(stepper_offTask, "state_off", 2000, NULL, 1, NULL, 1);
    });

    // Start the stepper instance as a service in the "background" as a separate task
    // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function
    stepper.startAsService();
}


bool stepper_kick()
{
    // Ready to start
    if (state == SLOW) {

        stepper_enable(false);
        stepper.setTargetPositionRelativeInRevolutions(targetRun);
        stepper.setSpeedInStepsPerSecond(speed);
        stepper_enable(true);

        targetRun *= -1;
        state = MOVE;
        lastRunAt = millis();

        return true;
    }
    return false;
}

void stepper_freerun()
{
    state = FREE;
    stepper_enable(false);
}


States stepper_state() {
    return state;
}

bool stepper_watchdog(int timeoutSeconds) {
    if (timeoutSeconds > 0)
        if ((millis() - lastRunAt) > 1000*timeoutSeconds) return false;
    return true;
}