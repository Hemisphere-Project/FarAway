#include <Wire.h>
#include <TFMPI2C.h>  // TFMini-Plus I2C Library v1.5.0
TFMPI2C tfmP;         // Create a TFMini-Plus I2C object


int addr = 0;


int tfI2CError()
{
    Serial.print("ERROR");
    tfmP.printReply();
    return 0;
}

int i2cScan() 
{

    Serial.println( "Scanning...");
    for( uint8_t x = 1; x < 127; x++ )
    {
        Wire.beginTransmission(x);
        // Use return value of Write.endTransmisstion() to
        // see if a device did acknowledge the I2C address.
        if( Wire.endTransmission() == 0)
        {
            Serial.print( "I2C device found at address ");
            Serial.print( x);
            Serial.print( " (0x");
            Serial.print( x < 16 ? "0" : ""); 
            Serial.print( x, HEX);
            Serial.println( " Hex)");
            
            addr = x;
            return true;
        }
    }
    Serial.println( "No I2C devices found.");
    return false;
}


bool i2cInit(uint8_t dataPin, uint8_t clockPin) {


    Serial.println("\nI2C begin");

    // Init I2C
    Wire.setPins( dataPin, clockPin);
    Wire.setClock(400000);

    Serial.println("LIDDAR: I2C bus recovery ");
    //try i2c bus recovery at 100kHz = 5uS high, 5uS low
    pinMode( dataPin, OUTPUT); //keeping SDA high during recovery
    digitalWrite( dataPin, HIGH);
    pinMode( clockPin, OUTPUT);
    for (int i = 0; i < 10; i++)   //9nth cycle acts as NACK
    {
      digitalWrite( clockPin, HIGH);
      delayMicroseconds( 5);
      digitalWrite( clockPin, LOW);
      delayMicroseconds( 5);
    }
    //a STOP signal (SDA from low to high while CLK is high)
    digitalWrite( dataPin, LOW);
    delayMicroseconds( 5);
    digitalWrite( clockPin, HIGH);
    delayMicroseconds( 2);
    digitalWrite( dataPin, HIGH);
    delayMicroseconds( 2);
    //bus status is now : FREE
    //return to power up mode
    pinMode( dataPin, INPUT);
    pinMode( clockPin, INPUT);
    delay( 10);


    // Find device ADDR
    if (!i2cScan()) return false;


    // - - Reset - - - - - - - - -
    Serial.print("TFMini I2C Reset: ");
    if( !tfmP.sendCommand( SOFT_RESET, 0, addr)) return tfI2CError();
    Serial.println("ok");
    delay(500);

    // - - Display the firmware version - - - - - - - - -
    Serial.print( "TFMini Firmware version: ");
    if( !tfmP.sendCommand( GET_FIRMWARE_VERSION, 0, addr)) return tfI2CError();
    Serial.printf( "%1u.",    tfmP.version[0]); // print three single numbers
    Serial.printf( "%1u.",    tfmP.version[1]); // each separated by a dot
    Serial.printf( "%1u\r\n", tfmP.version[2]);
    delay(100);

    // - - Set the data frame-rate to FRAME_1000Hz - - - - - - - -
    Serial.print( "TFMini: Data-Frame rate: ");
    if( !tfmP.sendCommand( SET_FRAME_RATE, FRAME_100, addr)) return tfI2CError();
    Serial.printf( "%dHz.\r\n", FRAME_100);

    delay(100);            
    return true;
}

int i2cAddr() {
    return addr;
}

void i2cAddr(int newAddr) 
{
    Serial.printf( "I2C set addr %d ", newAddr);
    if( tfmP.sendCommand(SET_I2C_ADDRESS, newAddr, addr)) {
        addr = newAddr;
        Serial.println("OK");
        delay(500);
        ESP.restart();
    }
    else tfmP.printReply();

    
}

void i2c2Serial()
{
    Serial.printf( "Device set in SERIAL mode: ");
    if( tfmP.sendCommand(SET_SERIAL_MODE, 0, addr)) {
        Serial.println("OK");
        delay(500);
        ESP.restart();
    }
    else tfmP.printReply();   
}
