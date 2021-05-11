/*
  K32_timer_h.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
// #ifndef K32_timer_h
// #define K32_timer_h

#include "../../Timer/Timer.h"


class K32_timer : public Timer {
    public:
        K32_timer() {
            // Update task
            xTaskCreate(this->task,       // function
              "ktimer",                     // task name
              10000,                         // stack memory
              (void *)this,                 // args
              0,                            // priority
              NULL);                        // handler

        }

        static void task(void *parameter)
        {
            K32_timer *that = (K32_timer *)parameter;
            while (true) {
                that->update();
                delay(1);
            }
            vTaskDelete(NULL);
        }

};

// #endif  