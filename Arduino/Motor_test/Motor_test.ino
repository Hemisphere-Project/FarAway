
// STEPPER
#include <AccelStepper.h>
// DEBUG
#include "debug.h"


int dirPin = 17;
int pulPin = 4;
// AccelStepper stepper(1, pulPin, dirPin);
AccelStepper stepper(AccelStepper::DRIVER, pulPin, dirPin);


void setup()
{
  // SERIAL
  Serial.begin(115200);
  // STEPPER
  stepper.setMaxSpeed(10);
  stepper.setAcceleration(20);//200
  stepper.moveTo(200);
  // stepper.setSpeed(200);

  // WIFI
  wifi_ota( "faraway-proto");
  wifi_maxTry(20);
  wifi_connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");
}

void loop()
{

  wifi_loop();

  if (stepper.distanceToGo() == 0){
    delay(1000);
    stepper.moveTo(-stepper.currentPosition());
  }
  stepper.run();

  // Serial.println(stepper.currentPosition());

  // stepper.runSpeed();


}
