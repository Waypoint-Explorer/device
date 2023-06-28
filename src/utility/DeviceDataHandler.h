#ifndef __SAVEDATA__
#define __SAVEDATA__

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Logger.h"
#include "SPIFFS.h"
#include "model/Device.h"
#include "config.h"

#define MAX_STORED_ELEMENTS 100


class DeviceDataHandler {
   public:
    static void init();
    static void reset();
    static void initData(Device* device); 
    static void updateEnvData(EnvData envData, Device* device);
};

#endif
