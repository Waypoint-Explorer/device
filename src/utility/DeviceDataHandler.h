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
    static void begin();

    /**
     * @brief Function to remove old data
     */
    static void reset();

    /**
     * @brief Function to initialize data of the device and store in file JSON
     * @return FileError in case of errors
     */
    static FileError initEnvDataJsonArray();

    /**
     * @brief Function that updates environmental data list with last reading
     * @param envData : EnvData object containing the last reading
     * @param device  : Device object
     * @return FileError in case of errors
     */
    static FileError writeLastEnvData(EnvData envData, Device* device);

    /**
     * @brief Function that read environmental data list
     * @param LinkedList<EnvData> : List of environmental data
     * @return FileError in case of errors
     */
     static FileError readEnvDataList(LinkedList<EnvData>* envDataList);

};

#endif
