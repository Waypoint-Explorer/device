#include "DeviceDataHandler.h"

void DeviceDataHandler::begin() { SPIFFS.begin(true); }

void DeviceDataHandler::reset() { SPIFFS.remove(FILENAME); }

FileError DeviceDataHandler::initEntryDataJsonArray() {
    File file = SPIFFS.open(FILENAME, FILE_WRITE);
    DynamicJsonDocument doc(JSON_DOC_SIZE);

    JsonArray entryData = doc.createNestedArray(ARRAY_DATA_LABEL);

    if (serializeJsonPretty(doc, file) == 0) {
        Logger.log("Failed to write to SPIFFS file");
        return FILE_CANT_WRITE;
    } else {
        Logger.log("File write success!");
    }
    file.close();
    return FILE_OK;
}

FileError DeviceDataHandler::writeLastEntryData(EntryData entryData,
                                                Device* device) {
    File file = SPIFFS.open(FILENAME, FILE_READ);
    DynamicJsonDocument doc(JSON_DOC_SIZE);

    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        file.close();

        file = SPIFFS.open(FILENAME, FILE_WRITE);
        JsonArray array = doc[ARRAY_DATA_LABEL];

        int64_t diff = 0;
        int64_t diffHours = 0;
        int64_t diffPrec = 0;
        int64_t diffHoursPrec = -1;

        if (array.size() > 0) {  // atleast 1 element
            // check if new entry hour diff with first entry is between 0 and
            // 999
            JsonObject firstobject = array[0].as<JsonObject>();

            diff = entryData.timestamp -
                   firstobject[TIMESTAMP_LABEL].as<int64_t>();

            if (diff <= 0)
                diffHours = -1;
            else
                diffHours = round((double)diff / S_TO_HOUR_FACTOR);

            if (array.size() > 1) {  // atleast 2 elements
                // check if new entry hour diff with last entry is different
                JsonObject objectPrec =
                    array[array.size() - 1].as<JsonObject>();  // last element
                diffPrec = objectPrec[TIMESTAMP_LABEL].as<int64_t>() -
                           firstobject[TIMESTAMP_LABEL].as<int64_t>();
                if (diffPrec <= 0)
                    diffHoursPrec = -1;
                else
                    diffHoursPrec = round((double)diffPrec / S_TO_HOUR_FACTOR);
            }
        }

        Logger.log("DIFF HOURS: " + String(diffHours));
        Logger.log("DIFF HOURS PREC: " + String(diffHoursPrec));

        if (diffHours >= 0 && diffHours < MAX_HOUR_DIFFERENCE &&
            diffHours > diffHoursPrec) {
            Logger.log("Add element");
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

        } else {
            Logger.log("Entry data not added!");
        }

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

FileError DeviceDataHandler::readEntryDataList(
    LinkedList<EntryData>* entryDataList) {
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
        }

    } else {
        return FILE_CANT_OPEN;
    }
    return FILE_OK;
}