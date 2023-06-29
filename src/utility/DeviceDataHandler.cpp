#include "DeviceDataHandler.h"

void DeviceDataHandler::init() { SPIFFS.begin(true); }

void DeviceDataHandler::reset() { SPIFFS.remove(FILENAME); }

FileError DeviceDataHandler::initData(Device* device) {
    File file = SPIFFS.open(FILENAME, FILE_WRITE);
    DynamicJsonDocument doc(JSON_DOC_SIZE);

    doc[ID_LABEL] = device->getId();

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

FileError DeviceDataHandler::updateEnvData(EnvData envData, Device* device) {
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
            device->envDataList->remove(0);
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

            // Add env data to device
            device->envDataList->add(envData);
        }
        file.close();

    } else {
        return FILE_CANT_OPEN;
    }
    return FILE_OK;
}