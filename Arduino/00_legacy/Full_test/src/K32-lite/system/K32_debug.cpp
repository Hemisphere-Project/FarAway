#include "K32_debug.h"

K32_debug Debug;

K32_debug::K32_debug(bool serialfallback) : _serialfallback(serialfallback) {
    Serial.begin(115200);
    
    if (_serialfallback) this->setSerialEnabled(true);
}

void K32_debug::setWifi(K32_wifi* wifi) 
{   
    this->_wifi = wifi;

    // Update task
    if (this->_wifi != nullptr)
    xTaskCreate(this->task,       // function
        "kdebug",                     // task name
        5000,                         // stack memory
        (void *)this,                 // args
        0,                            // priority
        NULL);                        // handler
}

void K32_debug::task(void *parameter)
{
    K32_debug *that = (K32_debug *)parameter;

    while(!that->_wifi->isConnected()) delay(1000);
    
    debugI("DEBUG: connected at %s", that->_wifi->nameDevice);

    that->begin(that->_wifi->nameDevice);
    that->setResetCmdEnabled(true);

    while (true) {
        that->handle();
        delay(1);

        if (that->_serialfallback)
            if (!that->isCon && that->isConnected()) {
                that->isCon = true; 
                that->setSerialEnabled(false);
            }
            else if (that->isCon && !that->isConnected()) {
                that->isCon = false; 
                that->setSerialEnabled(true);
            }
    }

    vTaskDelete(NULL);
}