#include "Device.h"

Device::Device() {
    position = new Position(0, 0);
    errorsHandler = new ErrorsHandler();
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
    errorsHandler->log();
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
    errorsRTC->battery = errorsHandler->battery;
    errorsRTC->file = errorsHandler->file;
    errorsRTC->gps = errorsHandler->gps;
    errorsRTC->init = errorsHandler->init;
    errorsRTC->qrCode = errorsHandler->qrCode;
    errorsRTC->sensor = errorsHandler->sensor;
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
    errorsHandler->battery = errorsRTC->battery;
    errorsHandler->file = errorsRTC->file;
    errorsHandler->gps = errorsRTC->gps;
    errorsHandler->init = errorsRTC->init;
    errorsHandler->qrCode = errorsRTC->qrCode;
    errorsHandler->sensor = errorsRTC->sensor;
}