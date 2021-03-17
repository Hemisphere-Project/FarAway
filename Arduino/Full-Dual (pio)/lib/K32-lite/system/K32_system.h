/*
  K32_system.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
#ifndef K32_system_h
#define K32_system_h

#include <Preferences.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>


class K32_system {
  public:
    K32_system() {
      this->lock = xSemaphoreCreateMutex();

      xSemaphoreTake(this->lock, portMAX_DELAY);
      preferences.begin("k32-app", false);
      xSemaphoreGive(this->lock);
    };

    int id() {
      #ifdef K32_SET_NODEID
        return K32_SET_NODEID;
      #else
        int id;
        xSemaphoreTake(this->lock, portMAX_DELAY);
        id = preferences.getUInt("id", 0);
        xSemaphoreGive(this->lock);
        return id; 
      #endif
    }

    void id(int id) {
      xSemaphoreTake(this->lock, portMAX_DELAY);
      int old = preferences.getUInt("id", 0);
      if (id != old) preferences.putUInt("id", id);
      xSemaphoreGive(this->lock);
    }

    int channel() {
      int chan;
      xSemaphoreTake(this->lock, portMAX_DELAY);
      chan = preferences.getUInt("channel", 15);
      xSemaphoreGive(this->lock);
      return chan;
    }

    void channel(int channel) {
      xSemaphoreTake(this->lock, portMAX_DELAY);
      int old = preferences.getUInt("channel", 15);
      if (channel != old) preferences.putUInt("channel", channel);
      xSemaphoreGive(this->lock);
    }

    int hw() {
      #ifdef K32_SET_HWREVISION
        return K32_SET_HWREVISION;
      #elif HW_REVISION
        return HW_REVISION;
      #else
        int hw;
        xSemaphoreTake(this->lock, portMAX_DELAY);
        hw = preferences.getUInt("hw", 0);
        xSemaphoreGive(this->lock);
        return hw;
      #endif
    }

    void hw(int hwrevision) {
      xSemaphoreTake(this->lock, portMAX_DELAY);
      int old = preferences.getUInt("hw", 0);
      if (hwrevision != old) preferences.putUInt("hw", hwrevision);
      xSemaphoreGive(this->lock);
    }

    String name() {
      String name = "esp-" + String(this->id());
      return name;
    }

    void reset() {
      xSemaphoreTake(this->lock, portMAX_DELAY);
      preferences.end();
      xSemaphoreGive(this->lock);
      // Hard restart
      esp_task_wdt_init(1,true);
      esp_task_wdt_add(NULL);
      while(true);
    }

    Preferences preferences;
    
  private:
    SemaphoreHandle_t lock;

};

#endif
