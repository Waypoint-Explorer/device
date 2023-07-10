#include "Device.h"

Device::Device() {
    position = new Position(0, 0);
    errorsData = new ErrorsData();
    lastEnvData = new EnvData();
    preferences.begin("device");
}

void Device::endPreferences() { preferences.end(); }

void Device::setup() {
    id = IdentifierGenerator::generateUniqueNumberId(16);
    Logger.log("ID: " + id);
}

void Device::log() {
    Logger.log("Init: " + init ? "TRUE" : "FALSE");
    Logger.log("ID: " + id);
    position->log();
    errorsData->log();
    lastEnvData->log();
}

void Device::storeInitToPreferences() { preferences.putBool("init", init); }

void Device::storeIdToPreferences() { preferences.putString("id", id); }

void Device::storePositionToPreferences() {
    preferences.putFloat("latitude", position->latitude);
    preferences.putFloat("longitude", position->longitude);
}

void Device::storeLastEnvDataToRTC(EnvDataStruct* lastEnvDataRTC) {
    lastEnvDataRTC->temperature = lastEnvData->temperature;
    lastEnvDataRTC->humidity = lastEnvData->humidity;
    lastEnvDataRTC->pressure = lastEnvData->pressure;
    lastEnvDataRTC->airQuality = lastEnvData->airQuality;
}

void Device::storeErrorsToRTC(ErrorsStruct* errorsRTC) {
    errorsRTC->battery = errorsData->battery;
    errorsRTC->file = errorsData->file;
    errorsRTC->gps = errorsData->gps;
    errorsRTC->init = errorsData->init;
    errorsRTC->qrCode = errorsData->qrCode;
    errorsRTC->sensor = errorsData->sensor;
}

void Device::retrieveInitFromPreferences() {
    init = preferences.getBool("init");
}

void Device::retrieveIdFromPreferences() { id = preferences.getString("id"); }

void Device::retrievePositionFromPreferences() {
    position->latitude = preferences.getFloat("latitude");
    position->longitude = preferences.getFloat("longitude");
}

void Device::retrieveEnvDataFromRTC(EnvDataStruct* lastEnvDataRTC) {
    lastEnvData->temperature = lastEnvDataRTC->temperature;
    lastEnvData->humidity = lastEnvDataRTC->humidity;
    lastEnvData->pressure = lastEnvDataRTC->pressure;
    lastEnvData->airQuality = lastEnvDataRTC->airQuality;
}

void Device::retrieveErrorsFromRTC(ErrorsStruct* errorsRTC) {
    errorsData->battery = errorsRTC->battery;
    errorsData->file = errorsRTC->file;
    errorsData->gps = errorsRTC->gps;
    errorsData->init = errorsRTC->init;
    errorsData->qrCode = errorsRTC->qrCode;
    errorsData->sensor = errorsRTC->sensor;
}