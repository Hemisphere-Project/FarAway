#include "BluetoothSerial.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

bool outOfOrder = false;


void bt_task(void * parameter)
{
  while(true) 
  {
    delay(30000);
    if (!SerialBT.connected(1)) {
        Serial.println("No friends for 30 seconds... Restarting.."); 
        outOfOrder = true;
    }
  }
}

void bt_watch()
{
    xTaskCreate(
      bt_task, /* Function to implement the task */
      "BT_watch", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      NULL); /* Handler */
}

void bt_event(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.print("Client Connected,  address: ");
    const uint8_t* point = param->srv_open.rem_bda;
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", point[0], point[1], point[2], point[3], point[4], point[5]);
    Serial.println();
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    Serial.println("Client disconnected... Restarting..");
    outOfOrder = true;
  }
}

// HOST CONNECT
void bt_start()
{
    SerialBT.register_callback(bt_event);
    Serial.println("Starting BT stack..");

    SerialBT.begin("faraway");
    Serial.print("Starting as Host device: ");
    const uint8_t* point = esp_bt_dev_get_address();
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", point[0], point[1], point[2], point[3], point[4], point[5]);
    Serial.println();
    
    bt_watch();
}

// CLI CONNECT
void bt_connect(uint8_t address[6]) {

    SerialBT.register_callback(bt_event);
    Serial.println("Starting BT stack..");

    SerialBT.begin("faraway-cli", true); 
    Serial.print("Starting as Client device.. ");
    delay(5000);

    Serial.print("looking for Host: ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", address[0], address[1], address[2], address[3], address[4], address[5]);
    Serial.println();

    bt_watch();

    if (!SerialBT.connect(address)) {
        Serial.println("Failed to connect... Restarting.."); 
        outOfOrder = true;
    }
    
}

bool bt_ok()
{
  return !outOfOrder;
}

