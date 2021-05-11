/*
  K32_dmx.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
#ifndef K32_dmx_h
#define K32_dmx_h

#include <LXESP32DMX.h>
#include "esp_task_wdt.h"

enum DmxDirection { DMX_IN, DMX_OUT };

class K32_dmx {
  public:
    K32_dmx(const int DMX_PIN[3], DmxDirection dir) {
      
      if (dir == DMX_OUT) 
      {
        pinMode(DMX_PIN[0], OUTPUT);
        digitalWrite(DMX_PIN[0], HIGH);

        pinMode(DMX_PIN[1], OUTPUT);
        ESP32DMX.startOutput(DMX_PIN[1]);
      }
      else  // TODO: implement dmx in !
      {
        pinMode(DMX_PIN[0], OUTPUT);
        digitalWrite(DMX_PIN[0], LOW);

        pinMode(DMX_PIN[2], INPUT);
        // ESP32DMX.startInput(DMX_PIN[2]);   
      }
      

    };

    // SET one value
    K32_dmx* set(int index, int value) 
    {
      xSemaphoreTake(ESP32DMX.lxDataLock, portMAX_DELAY);
      ESP32DMX.setSlot(index, value);
      xSemaphoreGive(ESP32DMX.lxDataLock);
      return this;
    }

    // SET multiple values
    K32_dmx* setMultiple(int* values, int size, int offsetAdr = 1) 
    {
      xSemaphoreTake(ESP32DMX.lxDataLock, portMAX_DELAY);
      for (int i = 0; i < size; i++)
        ESP32DMX.setSlot(i+offsetAdr, values[i]);
      xSemaphoreGive(ESP32DMX.lxDataLock);
      return this;
    }


  private:

};

#endif
