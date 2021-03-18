#include <TFMPlus.h>  // Include TFMini Plus Library v1.4.1
TFMPlus tfmini;         // Create a TFMini Plus object

bool tfSerialError()
{
    Serial.print("ERROR");
    tfmini.printReply();
    Serial2.end();
    return false;
}


bool serialInit(int RX, int TX) {

    // TFmini Plus init
    Serial.println("\nSerial2 begin");

    Serial2.begin(115200, SERIAL_8N1, RX, TX);  // Initialize TFMPLus device serial port.
    Serial2.flush();
    tfmini.begin(&Serial2);

    Serial.print("TFMini Serial Reset: ");
    if( !tfmini.sendCommand( SYSTEM_RESET, 0)) return tfSerialError();
    Serial.println( "OK");
    delay(500);

    // - - Display the firmware version - - - - - - - - -
    Serial.print( "TFMini Firmware version: ");
    if( !tfmini.sendCommand( OBTAIN_FIRMWARE_VERSION, 0)) return tfSerialError();
    Serial.printf( "%1u.",    tfmini.version[0]); // print three single numbers
    Serial.printf( "%1u.",    tfmini.version[1]); // each separated by a dot
    Serial.printf( "%1u\r\n", tfmini.version[2]);
    delay(100);

    // - - Set the data frame-rate to FRAME_1000Hz - - - - - - - -
    Serial.print( "TFMini: Data-Frame rate: ");
    if( !tfmini.sendCommand( SET_FRAME_RATE, FRAME_1000)) return tfSerialError();
    Serial.printf( "%dHz.\r\n", FRAME_1000);

    delay(100);            
    return true;
}


void serial2I2C() {
    if( tfmini.sendCommand(SET_I2C_MODE, 1)) {
        Serial.printf( "Device set in I2C mode\r\n");
    }
    else tfmini.printReply();

    delay(1000);
    ESP.restart();
}