#include <Arduino.h>
#include "serial.h"
#include "i2c.h"
#include <M5Atom.h>

#define RXD2 33
#define TXD2 23

#define I2C_ADDR1 16
#define I2C_ADDR2 37

enum Modes {NONE, SERIALZ, I2CZ};
int mode = NONE;

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
    M5.dis.displaybuff(ledMatrix);
}


void setup() {
  

  M5.begin(true, false, true);
  setAllLeds(100,0,100);

  delay(1000);
  Serial.println("");

  if ( serialInit(RXD2, TXD2) ) {
    Serial.println("TFMini: serial OK");
    mode = SERIALZ;
    setAllLeds(255,255,0);
  }
  else if (i2cInit(TXD2, RXD2)) 
  {
    mode = I2CZ;
    if (i2cAddr() == I2C_ADDR1)       setAllLeds(0,0,255);
    else if (i2cAddr() == I2C_ADDR2)  setAllLeds(0,255,0);
    else                              setAllLeds(0,255,255);
  }
  else {
    mode = NONE;
    setAllLeds(255,0,0);
  }

  // Clear previous events
  M5.update();
  M5.Btn.wasReleased();
}


void loop() {
  
  M5.update();

  if (M5.Btn.wasReleased())
  {
    // Retry
    if (mode == NONE) ESP.restart();

    // Toggle I2C addr
    if (mode == I2CZ) i2cAddr( (i2cAddr()==I2C_ADDR1)?I2C_ADDR2:I2C_ADDR1 );
  
  }

  if (M5.Btn.pressedFor(1000))
  {
    // Switch Device to I2C 
    if (mode == SERIALZ) serial2I2C();

    // Switch Device to Serial 
    if (mode == I2CZ) i2c2Serial();
  }

  delay(10);
}


