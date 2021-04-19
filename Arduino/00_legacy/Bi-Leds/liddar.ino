#include <TFMPlus.h>  // Include TFMini Plus Library v1.4.1
TFMPlus tfmini;         // Create a TFMini Plus object

#define TRIG1_DISTANCE 100

#define RXD2 33
#define TXD2 23

unsigned long Trigger1 = 0;
unsigned long Trigger2 = 0;

int16_t tfDist1 = 0;
int16_t lastDist1 = 0;


void liddar_setup() {

    // TFmini Plus init
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  // Initialize TFMPLus device serial port.
    delay(20);
    tfmini.begin(&Serial2);
    if( tfmini.sendCommand( SYSTEM_RESET, 0)) debugD( "TFMini: System reset OK\n");
    else {
    Serial.print("TFMini: System reset ERROR ");
    tfmini.printReply();
    }
    delay(100);

    // - - Display the firmware version - - - - - - - - -
    debugD( "TFMini: Firmware version: ");
    if( tfmini.sendCommand( OBTAIN_FIRMWARE_VERSION, 0))
    {
        debugD( "%1u.",    tfmini.version[0]); // print three single numbers
        debugD( "%1u.",    tfmini.version[1]); // each separated by a dot
        debugD( "%1u\r\n", tfmini.version[2]);
    }
    else tfmini.printReply();

    // - - Set the data frame-rate to FRAME_1000Hz - - - - - - - -
    debugD( "TFMini: Data-Frame rate: ");
    if( tfmini.sendCommand( SET_FRAME_RATE, FRAME_1000)) debugD( "%dHz.\r\n", FRAME_1000);
    else tfmini.printReply();
    // - - - - - - - - - - - - - - - - - - - - - - - -

    delay(100);            // And wait for half a second.

}

bool liddar_check() {

    // CHECK TRIGGER1
    if( tfmini.getData(tfDist1) )      // Get data from the device.
    {

        // Value changed: PRINT
        if ( abs(lastDist1-tfDist1) > 3)
        {
            debugD( "TRIG1: %icm \r\n", tfDist1);
            lastDist1 = tfDist1;
        }

        // Trigger1
        if (lastDist1 > 0 && lastDist1 < TRIG1_DISTANCE) return true;

    }
    else tfmini.printFrame();   // Error

    return false;
}
