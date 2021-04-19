#include "Arduino.h"

int dirPin = 21;
int pulPin = 25;
int enablePin = 22;

int stepsPerRevolution = 25600*2;

States state = STOP;

float targetRun = 1.2;
bool enableState = false;

// set the speed and acceleration rates for the stepper motor
int speed = stepsPerRevolution*0.3;     //*0.1; 
int accel = 4*speed;                    //4*speed;
int decel = 0.15*speed/targetRun;       //0.15*speed/targetRun;

unsigned long accu_trig;

void stepper_setup() 
{
    // set enable Pin
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, enableState);

    // connect and configure the stepper motor to its IO pins
    stepper.connectToPins(pulPin, dirPin);
    stepper.setStepsPerRevolution(stepsPerRevolution);


    // Reach destination Callback
    stepper.registerTargetPositionReachedCallback([](long position){
        xTaskCreate(stepper_offTask, "state_off", 2000, NULL, 1, NULL);
    });

    // Start the stepper instance as a service in the "background" as a separate task
    // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function
    stepper.startAsService();
}

void stepper_breaker()
{
    // if (enableState) {
    //     stepper_enable(false);
    //     stepper.setTargetPositionToStop();
    // }
    stepper_enable(false);
    stepper.stopService();
}

void stepper_offTask(void* param)
{
    stepper_enable(false);

    if (state == MOVE || state == FREE) while(stepper_accumulate()) delay(10);

    if (state == ACCU || state == SLOW) stepper_slowrun();

    vTaskDelete( NULL );
}


bool stepper_kick()
{
    // Ready to start
    if (state == SLOW) {
        stepper_enable(true);
        stepper.setSpeedInStepsPerSecond(speed);
        stepper.setAccelerationInStepsPerSecondPerSecond(accel);
        stepper.setDecelerationInStepsPerSecondPerSecond(decel);
        stepper.setTargetPositionRelativeInRevolutions(targetRun);
        state = MOVE;
        return true;
    }
    return false;
}

void stepper_freerun()
{
    state = FREE;
    stepper_enable(false);
}

bool stepper_accumulate()
{
    if (state != ACCU) accu_trig = millis();
    state = ACCU;
    return (millis()-accu_trig < 5000);
}

void stepper_slowrun()
{
    state = SLOW;

    stepper.setSpeedInStepsPerSecond(speed/50);
    stepper.setAccelerationInStepsPerSecondPerSecond(accel/10);
    stepper.setDecelerationInStepsPerSecondPerSecond(accel/10);

    targetRun *= -1;

    float target = targetRun*random(5,7)/abs(targetRun);
    stepper.setTargetPositionRelativeInRevolutions(target);
    stepper_enable(true);
}


void stepper_enable(bool doEnable) 
{
    enableState = doEnable;
    digitalWrite(enablePin, !enableState);
} 

States stepper_state() {
    return state;
}