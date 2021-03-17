int dirPin = 17;
int pulPin = 4;

int stepsPerRevolution = 25600*1.65;

enum States {STOP, MOVE, WAIT};
States state = STOP;

float targetRun = 2.0;
unsigned long waiterTime = 0;

void stepper_setup() 
{
    // connect and configure the stepper motor to its IO pins
    stepper.connectToPins(pulPin, dirPin);
    stepper.setStepsPerRevolution(stepsPerRevolution);
    
    // set the speed and acceleration rates for the stepper motor
    int speed = stepsPerRevolution*0.1; 
    int accel = 4*speed;
    int decel = 0.15*speed/targetRun;

    stepper.setSpeedInStepsPerSecond(speed);
    stepper.setAccelerationInStepsPerSecondPerSecond(accel);
    stepper.setDecelerationInStepsPerSecondPerSecond(decel);

    // Reach destination Callback
    stepper.registerTargetPositionReachedCallback(targetPositionReachedCallback);

    // Start the stepper instance as a service in the "background" as a separate task
    // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function
    stepper.startAsService();
}

void targetPositionReachedCallback(long position)
{
    state = WAIT;
    delay(5000);
    state = STOP;
}


void stepper_loop()
{
    // Ready to start
    if (state == STOP) {
        targetRun *= -1;
        stepper.setTargetPositionRelativeInRevolutions(targetRun);
        state = MOVE;
    }
}
