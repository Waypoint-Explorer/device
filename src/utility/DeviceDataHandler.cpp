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

FileError DeviceDataHandler::writeLastEnvData(EnvData envData, Device* device) {
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

        elem[TEMPERATURE_LABEL] = envData.temperature;
        elem[HUMIDITY_LABEL] = envData.humidity;
        elem[PRESSURE_LABEL] = envData.pressure;
        elem[AIR_QUALITY_LABEL] = envData.airQuality;

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

FileError DeviceDataHandler::readEnvDataList(LinkedList<EnvData>* envDataList) {
    File file = SPIFFS.open(FILENAME, FILE_READ);
    DynamicJsonDocument doc(JSON_DOC_SIZE);

    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        file.close();
        JsonArray array = doc[ARRAY_DATA_LABEL];

        // Add each element in list
        for (JsonObject elem : array) {
            EnvData envData;
            envData.temperature = elem[TEMPERATURE_LABEL].as<int>();
            envData.humidity = elem[HUMIDITY_LABEL].as<int>();
            envData.pressure = elem[PRESSURE_LABEL].as<int>();
            envData.airQuality = elem[AIR_QUALITY_LABEL].as<int>();
            envDataList->add(envData);

            envData.log();
        }

    } else {
        return FILE_CANT_OPEN;
    }
    return FILE_OK;
}