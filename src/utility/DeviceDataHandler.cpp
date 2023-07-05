#include "DeviceDataHandler.h"

void DeviceDataHandler::begin() { SPIFFS.begin(true); }

void DeviceDataHandler::reset() { SPIFFS.remove(FILENAME); }

FileError DeviceDataHandler::initEnvDataJsonArray() {
    File file = SPIFFS.open(FILENAME, FILE_WRITE);
    DynamicJsonDocument doc(JSON_DOC_SIZE);

    JsonArray env_data = doc.createNestedArray(ARRAY_DATA_LABEL);

    if (serializeJsonPretty(doc, file) == 0) {
        Logger.log("Failed to write to SPIFFS file");
        return FILE_CANT_WRITE;
    } else {
        Logger.log("File write success!");
    }
    file.close();
    return FILE_OK;
}

FileError DeviceDataHandler::writeLastEnvData(EntryData entryData, Device* device) {
    File file = SPIFFS.open(FILENAME, FILE_READ);
    DynamicJsonDocument doc(JSON_DOC_SIZE);

    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        file.close();

        file = SPIFFS.open(FILENAME, FILE_WRITE);
        JsonArray array = doc[ARRAY_DATA_LABEL];

        // Remove older element
        if (array.size() >= MAX_STORED_ELEMENTS) {
            array.remove(0);
        }

        JsonObject elem = array.createNestedObject();

        elem[TIMESTAMP_LABEL] = entryData.timestamp;
        elem[TEMPERATURE_LABEL] = entryData.envData.temperature;
        elem[HUMIDITY_LABEL] = entryData.envData.humidity;
        elem[PRESSURE_LABEL] = entryData.envData.pressure;
        elem[AIR_QUALITY_LABEL] = entryData.envData.airQuality;

        if (serializeJsonPretty(doc, file) == 0) {
            Logger.log("Failed to write to SPIFFS file");
            return FILE_CANT_WRITE;
        } else {
            Logger.log("File update success!");
        }
        file.close();

    } else {
        return FILE_CANT_OPEN;
    }
    return FILE_OK;
}

FileError DeviceDataHandler::readEnvDataList(LinkedList<EntryData>* entryDataList) {
    File file = SPIFFS.open(FILENAME, FILE_READ);
    DynamicJsonDocument doc(JSON_DOC_SIZE);

    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        file.close();
        JsonArray array = doc[ARRAY_DATA_LABEL];

        // Add each element in list
        for (JsonObject elem : array) {
            EntryData entryData;
            entryData.timestamp = elem[TIMESTAMP_LABEL].as<int64_t>();
            entryData.envData.temperature = elem[TEMPERATURE_LABEL].as<int>();
            entryData.envData.humidity = elem[HUMIDITY_LABEL].as<int>();
            entryData.envData.pressure = elem[PRESSURE_LABEL].as<int>();
            entryData.envData.airQuality = elem[AIR_QUALITY_LABEL].as<int>();
            entryDataList->add(entryData);

            //entryData.log();
        }

    } else {
        return FILE_CANT_OPEN;
    }
    return FILE_OK;
}