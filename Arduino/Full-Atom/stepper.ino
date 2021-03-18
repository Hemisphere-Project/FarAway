int dirPin = 21;
int pulPin = 25;
int enablePin = 32;

int stepsPerRevolution = 25600*1.65;

enum States {STOP, MOVE, WAIT};
States state = STOP;

float targetRun = 1.0;
bool enableState = false;

void stepper_setup() 
{
    // set enable Pin
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, enableState);

    // connect and configure the stepper motor to its IO pins
    stepper.connectToPins(pulPin, dirPin);
    stepper.setStepsPerRevolution(stepsPerRevolution);
    
    // set the speed and acceleration rates for the stepper motor
    int speed = stepsPerRevolution*0.1;     //*0.1; 
    int accel = 4*speed;                    //4*speed;
    int decel = 0.15*speed/targetRun;       //0.15*speed/targetRun;

    stepper.setSpeedInStepsPerSecond(speed);
    stepper.setAccelerationInStepsPerSecondPerSecond(accel);
    stepper.setDecelerationInStepsPerSecondPerSecond(decel);

    // Reach destination Callback
    stepper.registerTargetPositionReachedCallback([](long position){
        xTaskCreate(state_offTask, "state_off", 2000, NULL, 1, NULL);
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
    stepper.stopService();
}

void state_offTask(void* param)
{
    state = WAIT;
    delay(1000);
    state = STOP;
    stepper_enable(false);

    vTaskDelete( NULL );
}


void stepper_kick()
{
    // Ready to start
    if (state == STOP) {
        stepper_enable(true);
        targetRun *= -1;
        stepper.setTargetPositionRelativeInRevolutions(targetRun);
        state = MOVE;
    }
}

void stepper_enable(bool doEnable) 
{
    enableState = doEnable;
    digitalWrite(enablePin, !enableState);
} 
