#include <Arduino.h>
#include "BluetoothSerial.h"
#include <M5Atom.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"

// #define BT_MODE=1


#define PIN_MC005 22

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

bool connected = false;
uint32_t lastHello = 0;
int counter = 0;

uint8_t address[6]  = {0x50, 0x02, 0x91, 0xA0, 0x7C, 0x82};


void printDeviceAddress() 
{
  const uint8_t* point = esp_bt_dev_get_address();
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", point[0], point[1], point[2], point[3], point[4], point[5]);
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if(event == ESP_SPP_SRV_OPEN_EVT){
    connected = true;
    Serial.print("Client Connected,  address: ");
    const uint8_t* point = param->srv_open.rem_bda;
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", point[0], point[1], point[2], point[3], point[4], point[5]);
    Serial.println();
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    Serial.println("Client disconnected... Restarting..");
    if (BT_MODE == 1) delay(3000);
    ESP.restart();
  }
}


/* this function will be invoked when additionalTask was created */
void checkConn( void * parameter )
{
  SerialBT.register_callback(callback);

  while(true)
  {
    if (!SerialBT.isReady(false)) {
      Serial.println("Starting BT stack..");

      // BT HOST
      if (BT_MODE == 0) {  
        SerialBT.begin("faraway");
        Serial.print("Starting as Host device: ");
        printDeviceAddress();
        delay(2000);
      }

      // BT CLI
      else if (BT_MODE == 1) {
        SerialBT.begin("faraway-cli", true); 
        Serial.print("Starting as Client device.. ");
        delay(5000);
        Serial.print("looking for Host: ");
        Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", address[0], address[1], address[2], address[3], address[4], address[5]);
        Serial.println();
        if (!SerialBT.connect(address)) {
          Serial.println("Failed to connect...."); 
          ESP.restart();
        }

      }
    }
    else delay(1000);

    // else if(!SerialBT.connected(5000)) 
    // {
      
    //   // BT CLI
    //   if (BT_MODE == 1) {
    //     Serial.println("Not connected.. retrying");
    //     if (!SerialBT.connect()) Serial.println("Failed to connect...."); 
    //   }
    // }

  }
  vTaskDelete( NULL );
}





void setup() {
  Serial.begin(115200);
  
  xTaskCreate(
    checkConn, /* Task function. */
    "BT_check_con", /* name of task. */
    10000, /* Stack size of task */
    NULL, /* parameter of the task */
    1, /* priority of the task */
    NULL); /* Task handle to keep track of created task */
}


void loop() 
{

  if (millis()-lastHello > 1000) {

    if (SerialBT.connected(1))
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