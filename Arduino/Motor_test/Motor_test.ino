
#include <AccelStepper.h>



int dirPin = 17;
int pulPin = 4;
// AccelStepper stepper(1, pulPin, dirPin);
AccelStepper stepper(AccelStepper::DRIVER, pulPin, dirPin);

void setup()
{
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(100000);
  stepper.setAcceleration(50000);
  stepper.moveTo(8000);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    stepper.run();
}

// void setup()
// {
//    stepper.setMaxSpeed(100000);
//    stepper.setSpeed(100000);
// }
//
// void loop()
// {
//    stepper.runSpeed();
// }
