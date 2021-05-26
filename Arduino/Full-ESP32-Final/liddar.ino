#include <Wire.h>
#include "filter.h"
#include <TFMPI2C.h>  // TFMini-Plus I2C Library v1.5.0
TFMPI2C tfmini;         // Create a TFMini-Plus I2C object

#define TRIG1_DISTANCE 50  // BLUE
#define TRIG2_DISTANCE 100  // GREEN

#define PIN_SCL 33 
#define PIN_SDA 32

#define I2C_ADDR1 16    // BLUE
#define I2C_ADDR2 37    // GREEN

unsigned long Trigger1 = 0;
unsigned long Trigger2 = 0;

int16_t tfDist1, tfDist2 = 0;
int16_t lastDist1, lastDist2 = 0;

bool liddar1OK, liddar2OK = false;
bool runningOK = false;

long FilterWeight = 95;
ExponentialFilter<long> Filter1(FilterWeight, TRIG1_DISTANCE+1);
ExponentialFilter<long> Filter2(FilterWeight, TRIG2_DISTANCE+1);


void liddar_setup() {

    Serial.println("LIDDAR: I2C begin");
    delay(1000);

    // Init I2C
    Wire.setPins( PIN_SDA, PIN_SCL);
    Wire.setClock(400000);

    Serial.println("LIDDAR: I2C bus recovery ");
    //try i2c bus recovery at 100kHz = 5uS high, 5uS low
    pinMode( PIN_SDA, OUTPUT); //keeping SDA high during recovery
    digitalWrite( PIN_SDA, HIGH);
    pinMode( PIN_SCL, OUTPUT);
    for (int i = 0; i < 10; i++)   //9nth cycle acts as NACK
    {
      digitalWrite( PIN_SCL, HIGH);
      delayMicroseconds( 5);
      digitalWrite( PIN_SCL, LOW);
      delayMicroseconds( 5);
    }
    //a STOP signal (SDA from low to high while CLK is high)
    digitalWrite( PIN_SDA, LOW);
    delayMicroseconds( 5);
    digitalWrite( PIN_SCL, HIGH);
    delayMicroseconds( 2);
    digitalWrite( PIN_SDA, HIGH);
    delayMicroseconds( 2);
    //bus status is now : FREE
    //return to power up mode
    pinMode( PIN_SDA, INPUT);
    pinMode( PIN_SCL, INPUT);
    delay( 10);
    
    delay(1000);

    Wire.begin();

    // Check LIDDAR1
    Serial.print("LIDDAR: liddar 1 ");
    Wire.beginTransmission(I2C_ADDR1);
    if( Wire.endTransmission() == 0) {
        Serial.println("OK");
        liddar1OK = true;
        tfmini.sendCommand( SET_FRAME_RATE, FRAME_1000, I2C_ADDR1);
    }
    else Serial.println("NOT FOUND");

    // Check LIDDAR2
    Serial.print("LIDDAR: liddar 2 ");
    Wire.beginTransmission(I2C_ADDR2);
    if( Wire.endTransmission() == 0) {
        Serial.println("OK");
        liddar2OK = true;
        tfmini.sendCommand( SET_FRAME_RATE, FRAME_1000, I2C_ADDR2);
    }
    else Serial.println("NOT FOUND");

}


bool liddar_check() {

    // CHECK TRIGGER1
    if( liddar1OK && tfmini.getData(tfDist1, I2C_ADDR1) )      // Get data from the device.
    {   
        if (tfDist1 > 0) {
            
            Filter1.Filter(tfDist1);

            // Value changed: PRINT
            if ( abs(lastDist1-Filter1.Current()) > 10)  
            {  
                // debugD( "TRIG1: %icm \r\n", Filter1.Current());   
                lastDist1 = Filter1.Current();
            }

            // Trigger1
            if (lastDist1 > 0 && lastDist1 < TRIG1_DISTANCE) return true;
        }
    }
    // else tfmini.printFrame();   // Error

    // CHECK TRIGGER2
    if( liddar2OK && tfmini.getData(tfDist2, I2C_ADDR2) )      // Get data from the device.
    {
        
        if (tfDist2 > 0) {
            
            Filter2.Filter(tfDist2);

            // Value changed: PRINT
            if ( abs(lastDist2-Filter2.Current()) > 10)  
            {  
                // debugD( "TRIG2: %icm \r\n", Filter2.Current());   
                lastDist2 = Filter2.Current();
            }

            // Trigger1
            if (lastDist2 > 0 && lastDist2 < TRIG2_DISTANCE) return true;
        }
    }
    // else tfmini.printFrame();   // Error

    return false;
}