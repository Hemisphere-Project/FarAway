#define FA_VERSION  3


// Config (one time Burn): it is then stored in EEPROM !
//
// #define K32_SET_NODEID    0
// #define FA_MODE           K32_SET_NODEID%2   // 0: motor // 1: leds


//file: main.cpp
#define ESP32

#include "Arduino.h"
#include "WiFi.h"
#include "bluetooth.h"

#include "lib/Timer/Timer.h"
#include "lib/RemoteDebug/src/RemoteDebug.h"
#include "lib/K32-lite/K32.h"



// // TARGET HOST
uint8_t address[6]  = {0x50, 0x02, 0x91, 0xA0, 0x7C, 0x82};

uint32_t lastHello = 0;
int counter = 0;

K32* k32;

Timer timer;
RemoteDebug Debug;
String inData;


void setup(){

  Serial.begin(115200);
  delay(1000);

  // K32 Lib
  k32 = new K32();

  // WIFI
  String mode = (k32->system->mode() == 0) ? "motor" : (k32->system->mode() == 1) ? "leds" : "zzz";
  k32->init_wifi("faraway-"+mode+"-v"+String(FA_VERSION));
  k32->wifi->connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");

  Debug.begin(k32->wifi->nameDevice);
  Debug.setResetCmdEnabled(true);

  if (k32->system->mode() == 0) bt_start();
  else if (k32->system->mode() == 1) bt_connect(address);
}

void loop(){
  Debug.handle();

  // OTA in progress
  if (k32->wifi->otaState == ERROR) ESP.restart();
  else if (k32->wifi->otaState > OFF) {
    delay(1000);
    return;
  }

  // BlueTooth failed
  if (!bt_ok()) ESP.restart();

  // Send HELO
  if (millis()-lastHello > 1000) {

    if ( SerialBT.connected(1) )
    {
      if (k32->system->mode() == 0)       SerialBT.print("hello ");
      else if (k32->system->mode() == 1)   SerialBT.print("world ");
      
      counter += 1;
      SerialBT.println(counter);
    }
    lastHello = millis();
    // Serial.println(ESP.getFreeHeap());
  }

  // Listen
  while (SerialBT.available() > 0)
  {
      char recieved = SerialBT.read();
      inData += recieved; 

      // Process message when new line character is recieved
      if (recieved == '\n')
      {
          Serial.print(inData);
          debugD("%s", inData.c_str());
          inData = ""; // Clear recieved buffer
      }
  }

  delay(1);
}
