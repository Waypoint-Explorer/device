#include "ErrorsHandler.h"

ErrorsHandler::ErrorsHandler() {
    battery = BATTERY_HIGH;
    init = INIT_OK;
    file = FILE_OK;
    gps = GPS_OK;
    qrCode = QR_OK;
    sensor = SENSOR_OK;
}

ErrorsHandler::ErrorsHandler(uint8_t battery, uint8_t init, uint8_t file,
                             uint8_t gps, uint8_t qrCode, uint8_t sensor) {
    this->battery = battery;
    this->init = init;
    this->file = file;
    this->gps = gps;
    this->qrCode = qrCode;
    this->sensor = sensor;
}

String ErrorsHandler::toString() {
    return String(battery) + String(init) + String(file) + String(gps) +
           String(qrCode) + String(sensor);
}

void ErrorsHandler::log() {
    Logger.log("Errors: B" + String(battery) + "|I" + String(init) + "|F" +
               String(file) + "|G" + String(gps) + "|Q" + String(qrCode) +
               "|S" + String(sensor));
}