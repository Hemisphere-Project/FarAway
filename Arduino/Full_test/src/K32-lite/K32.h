/*
  K32.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
#ifndef K32_h
#define K32_h

#include <Arduino.h>


#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#include "../../debug.h"
#include "network/K32_wifi.h"
#include "system/K32_timer.h"
#include "system/K32_system.h"
#include "light/K32_light.h"
#include "system/K32_debug.h"



class K32
{
public:
    K32()
    {
        // LOG
        LOG("\n\n.:: K32 ::.");

        // SYSTEM
        system = new K32_system();
        timer = new K32_timer();
        light = new K32_light();

// Save NODE_ID in flash
#ifdef K32_SET_NODEID
        system->id(K32_SET_NODEID);
        system->channel(15);
#endif
        LOGINL("Node id: ");
        LOG(system->id());

// Save HW_REVISION in flash
#ifdef K32_SET_HWREVISION
        system->hw(K32_SET_HWREVISION);
#elif HW_REVISION
        system->hw(HW_REVISION);
#endif
        LOGINL("HW rev: ");
        LOG(system->hw());

        LOG("");
        delay(100);
    }


    K32_timer* timer        = NULL;
    K32_system *system      = NULL;
    K32_wifi *wifi          = NULL;
    K32_light *light        = NULL;          

    void init_wifi(String nameAlias = "")
    {
        if (nameAlias != "")
            nameAlias = "-" + nameAlias;
        wifi = new K32_wifi(system->name() + nameAlias);

        Debug.setWifi(wifi);
    }

};

#endif
