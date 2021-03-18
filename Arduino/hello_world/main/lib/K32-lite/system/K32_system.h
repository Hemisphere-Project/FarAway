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
    int _id = -1;
    int _hw = -1;
    int _mode = -1;

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

        if (_id >= 0) return _id;

        int id;
        xSemaphoreTake(this->lock, portMAX_DELAY);
        id = preferences.getUInt("id", 0);
        _id = id;
        xSemaphoreGive(this->lock);
        return id; 
      #endif
    }

    void id(int id) {
      xSemaphoreTake(this->lock, portMAX_DELAY);
      int old = preferences.getUInt("id", 0);
      if (id != old) {
        _id = -1;
        preferences.putUInt("id", id);
      }
      xSemaphoreGive(this->lock);
    }

    int mode() {

      if (_mode >= 0) return _mode;

      int mod;
      xSemaphoreTake(this->lock, portMAX_DELAY);
      mod = preferences.getUInt("mode", 0);
      _mode = mod;
      xSemaphoreGive(this->lock);
      return mod;
    }

    void mode(int mode) {
      xSemaphoreTake(this->lock, portMAX_DELAY);
      int old = preferences.getUInt("mode", 0);
      if (mode != old) {
        _mode = -1;
        preferences.putUInt("mode", mode);
      }
      xSemaphoreGive(this->lock);
    }

    int hw() {
      #ifdef K32_SET_HWREVISION
        return K32_SET_HWREVISION;
      #elif HW_REVISION
        return HW_REVISION;
      #else

        if (_hw >= 0) return _hw;

        int hw;
        xSemaphoreTake(this->lock, portMAX_DELAY);
        hw = preferences.getUInt("hw", 0);
        _hw = hw;
        xSemaphoreGive(this->lock);
        return hw;
      #endif
    }

    void hw(int hwrevision) {
      xSemaphoreTake(this->lock, portMAX_DELAY);
      int old = preferences.getUInt("hw", 0);
      if (hwrevision != old) {
        _hw = -1;
        preferences.putUInt("hw", hwrevision);
      }
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
