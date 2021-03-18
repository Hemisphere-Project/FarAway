/*
  K32_debug.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
#ifndef K32_debug_h
#define K32_debug_h

#define DEBUG_DISABLE_AUTO_FUNC

#include <RemoteDebug.h>  //https://github.com/JoaoLopesF/debug 
#include "../../debug.h"
#include "../network/K32_wifi.h"

class K32_debug : public RemoteDebug {
    public:
      K32_debug(bool serialfallback = true);

      void setWifi(K32_wifi* wifi);

      bool _serialfallback;

    private:
      static void task(void *parameter);
      
      K32_wifi* _wifi = nullptr;
      bool isCon = false;
};

extern K32_debug Debug;

// #define LOGF(x, ...)  Debug.printf(x, ##__VA_ARGS__)
// #define LOG(x)        Debug.println(x)

#endif