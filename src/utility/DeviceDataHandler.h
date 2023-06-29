#ifndef __DEVICE_DATA_HANDLER__
#define __DEVICE_DATA_HANDLER__

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Logger.h"
#include "SPIFFS.h"
#include "errors.h"
#include "model/Device.h"
#include "strings.h"

#define MAX_STORED_ELEMENTS 100
#define JSON_DOC_SIZE 10000

/* Device Data Handler class definition */
class DeviceDataHandler {
   public:
    /**
     * @brief Function to begin SPIFFS
     */
    static void init();

    /**
     * @brief Function to remove old data
     */
    static void reset();

    /**
     * @brief Function to initialize data of the device and store in file JSON
     * @param device : Device object
     * @return FileError in case of errors
     */
    static FileError initData(Device* device);

    /**
     * @brief Function that updates environmental data list with last reading
     * @param envData : EnvData object containing the last reading
     * @param device  : Device object
     * @return FileError in case of errors
     */
    static FileError updateEnvData(EnvData envData, Device* device);
};

#endif
