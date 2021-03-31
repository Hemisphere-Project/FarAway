#include <Arduino.h>
#include <ArduinoLog.h>
#include <M5Atom.h>

#include <TFMPlus.h>  // Include TFMini Plus Library v1.4.1
TFMPlus tfmini;         // Create a TFMini Plus object

#define RXD2 25
#define TXD2 21

#define PIN_MC005 22

#define DURATION 2000
#define TRIG1_DISTANCE 100

// Initialize variables
int16_t tfDist = 0;    // Distance to object in centimeters
int lastDist = 0;

unsigned long Trigger1 = 0;
unsigned long Trigger2 = 0;
unsigned long nextPush = 0;

uint8_t position = 0;


// LEDS MATRIX
uint8_t ledMatrix[2 + 5 * 5 * 3];
void setAllLeds(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    ledMatrix[0] = 0x05;
    ledMatrix[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        ledMatrix[2 + i * 3 + 0] = Rdata;
        ledMatrix[2 + i * 3 + 1] = Gdata;
        ledMatrix[2 + i * 3 + 2] = Bdata;
    }
}

// LEDS PATH
#define PATHSIZE 16
const int ledPath[PATHSIZE][3] = { {0,6,12}, {1,-1,12}, {2,7,12}, {3,-1,12}, {4,8,12}, {9,-1,12}, {14,13,12}, {19,-1,12}, {24,18,12}, {23,-1,12}, {22,17,12}, {21,-1,12}, {20,16,12}, {15,-1,12}, {10,11,12}, {5,-1,12}};
void setPositionLed(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata) {
  position = position%PATHSIZE;

  for(int k=0; k<3; k++) 
    if (ledPath[position][k] >= 0)
    {
      ledMatrix[2 + ledPath[position][k] * 3 + 0] = Rdata;
      ledMatrix[2 + ledPath[position][k] * 3 + 1] = Gdata;
      ledMatrix[2 + ledPath[position][k] * 3 + 2] = Bdata;
    }
}




void setup()
{   
  // M5 init
  M5.begin(true, false, true);

  // LEDS INIT
  setAllLeds(128,0,0);
  M5.dis.displaybuff(ledMatrix);

  // Log init
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  // MC005 init
  pinMode(PIN_MC005, INPUT);

  // TFmini Plus init
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  // Initialize TFMPLus device serial port.
  delay(20);
  tfmini.begin(&Serial2);
  if( tfmini.sendCommand( SYSTEM_RESET, 0)) Log.notice( "TFMini: System reset OK\n");
  else {
    Serial.print("TFMini: System reset ERROR ");
    tfmini.printReply();
  }
  delay(100);

  // - - Display the firmware version - - - - - - - - -
  Log.notice( "TFMini: Firmware version: ");
  if( tfmini.sendCommand( OBTAIN_FIRMWARE_VERSION, 0))
  {
      Log.notice( "%1u.",    tfmini.version[0]); // print three single numbers
      Log.notice( "%1u.",    tfmini.version[1]); // each separated by a dot
      Log.notice( "%1u\r\n", tfmini.version[2]);
  }
  else tfmini.printReply();

  // - - Set the data frame-rate to FRAME_1000Hz - - - - - - - -
  Log.notice( "TFMini: Data-Frame rate: ");
  if( tfmini.sendCommand( SET_FRAME_RATE, FRAME_1000)) Log.notice( "%dHz.\r\n", FRAME_1000);
  else tfmini.printReply();
  // - - - - - - - - - - - - - - - - - - - - - - - -

  delay(100);            // And wait for half a second.

  // LEDS READY
  setAllLeds(0,0,0);
  M5.dis.displaybuff(ledMatrix);
}

void loop()
{
  M5.update();

  // CANCEL TRIGGERS
  if (M5.Btn.wasPressed()) 
  {
    Trigger1 = 0;
    Trigger2 = 0;
  }

  // END TRIGGERS
  if (millis()-Trigger1 > DURATION) Trigger1 = 0;
  if (millis()-Trigger2 > DURATION) Trigger2 = 0;

  // CHECK TRIGGER1
  if( tfmini.getData(tfDist) )      // Get data from the device.
  {
    
    // Value changed: PRINT
    if ( abs(lastDist-tfDist) > 3)  
    {  
      Log.notice( "TRIG1: %icm \r\n", tfDist);   
      lastDist = tfDist;
    }

    // Trigger1
    if (lastDist < TRIG1_DISTANCE) Trigger1 = millis();

  }
  else tfmini.printFrame();   // Error

  // CHECK TRIGGER2
  if (digitalRead(PIN_MC005) == 0) {
    if (Trigger2 == 0) Log.notice( "TRIG2: triggered \r\n");   
    Trigger2 = millis();
  }

  // ANIMATION
  if (Trigger1 || Trigger2) 
  {
    unsigned long Trig = max(Trigger1, Trigger2);

    // Time to increment
    if(nextPush <= millis())
    {
      position++;
      nextPush = millis() + DURATION/(3*PATHSIZE) * max(1, int(10*(millis()-Trig)/DURATION));
    } 
  } 

  // DRAW
  setAllLeds(0,0,0);
  
  if (Trigger1 > Trigger2) setPositionLed(128,0,0);
  else if (Trigger2 > Trigger1) setPositionLed(0,128,0);
  else setPositionLed(128,128,128);
  
  M5.dis.displaybuff(ledMatrix);
  
}