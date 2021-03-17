#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
// #include "K32.h"
#include "bluetooth.h"


// TARGET HOST
uint8_t address[6]  = {0x50, 0x02, 0x91, 0xA0, 0x7C, 0x82};

uint32_t lastHello = 0;
int counter = 0;

// K32 *k32;


void setup() 
{
  // K32 Lib
  // k32 = new K32();
  Serial.begin(115200);

  // WIFI
  // k32->init_wifi("faraway-bt");
  // k32->wifi->connect("interweb", "superspeed37");
  
  // WiFi.begin("interweb", "superspeed37");

  // Serial.print("Connecting to WiFi");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println();
  // Serial.println("Connected to the WiFi network");

  // BLUETOOTH
  if (BT_MODE == 0) bt_start();
  else if (BT_MODE == 1) bt_connect(address);
}


void loop() 
{

  if (!bt_ok()) ESP.restart();

  if (millis()-lastHello > 1000) {

    if ( SerialBT.connected(1) )
    {
      if (BT_MODE == 0)       SerialBT.print("hello ");
      else if (BT_MODE == 1)  SerialBT.print("world ");
      
      counter += 1;
      SerialBT.println(counter);
    }
    lastHello = millis();
  }

  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

  delay(1);
}