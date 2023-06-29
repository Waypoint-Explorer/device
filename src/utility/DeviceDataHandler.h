#ifndef __SAVEDATA__
#define __SAVEDATA__

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Logger.h"
#include "SPIFFS.h"
#include "model/Device.h"
#include "config.h"
#include "errors.h"

#define MAX_STORED_ELEMENTS 100


class DeviceDataHandler {
   public:
    static void init();
    static void reset();
    static FileError initData(Device* device); 
    static FileError updateEnvData(EnvData envData, Device* device);
};

#endif
