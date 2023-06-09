#ifndef __DEVICE_DATA_HANDLER__
#define __DEVICE_DATA_HANDLER__

#include "Arduino.h"
#include "ArduinoJson.h"
#include "LinkedList.h"
#include "Logger.h"
#include "SPIFFS.h"
#include "errors.h"
#include "label.h"
#include "model/Device.h"
#include "model/EntryData.h"

#define MAX_STORED_ELEMENTS 240
#define MAX_HOUR_DIFFERENCE 999
#define S_TO_HOUR_FACTOR 3600
#define JSON_DOC_SIZE 24576  // Calculated with ArduinoJson Assistant

// File name of flash saved entry data
#define FILENAME "/entry_data.json"

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
    static FileError initEntryDataJsonArray();

    /**
     * @brief Function that updates entry data list with last reading
     * @param entryData : EntryData object containing the last reading
     * @param device  : Device object
     * @return FileError in case of errors
     */
    static FileError writeLastEntryData(EntryData entryData, Device* device);

    /**
     * @brief Function that read entry data list
     * @param LinkedList<EntryData> : List of entry data (EnvData e Timestamp)
     * @return FileError in case of errors
     */
    static FileError readEntryDataList(LinkedList<EntryData>* entryDataList);
};

#endif
