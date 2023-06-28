#include "DeviceDataHandler.h"

void DeviceDataHandler::init() { SPIFFS.begin(true); }

void DeviceDataHandler::initData(Device* device) {
    File file = SPIFFS.open(FILENAME, FILE_WRITE);
    DynamicJsonDocument doc(10000);

    doc["id"] = device->getId();

    JsonArray env_data = doc.createNestedArray("env_data");

    if (serializeJsonPretty(doc, file) == 0) {
        Logger.log("Failed to write to SPIFFS file");
        device->errors->file = FILE_CANT_WRITE;
    } else {
        Logger.log("File Success!");
    }
    file.close();
}

void DeviceDataHandler::updateEnvData(EnvData envData, Device* device) {
    File file = SPIFFS.open(FILENAME, FILE_READ);
    DynamicJsonDocument doc(10000);

    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        file.close();

        file = SPIFFS.open(FILENAME, FILE_WRITE);
        JsonArray array = doc["env_data"];

        // Remove older element
        if (array.size() >= MAX_STORED_ELEMENTS) {
            array.remove(0);
            device->envDataList->remove(0);
        }

        JsonObject elem = array.createNestedObject();

        elem["temperature"] = envData.temperature;
        elem["humidity"] = envData.humidity;
        elem["pressure"] = envData.pressure;
        elem["airQuality"] = envData.airQuality;

        if (serializeJsonPretty(doc, file) == 0) {
            Logger.log("Failed to write to SPIFFS file");
            device->errors->file = FILE_CANT_WRITE;
        } else {
            Logger.log("Success!");

            // Add env data to device
            device->envDataList->add(envData);
        }
        file.close();

    } else {
        device->errors->file = FILE_CANT_OPEN;
    }
}