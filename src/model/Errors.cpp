#include "Errors.h"

Errors::Errors() {
    battery = BATTERY_OK;
    init = INIT_OK;
    file = FILE_OK;
    gps = GPS_OK;
    qrCode = QR_OK;
    sensor = SENSOR_OK;
}

Errors::Errors(byte battery, byte init, byte file, byte gps, byte qrCode,
               byte sensor) {
    this->battery = battery;
    this->init = init;
    this->file = file;
    this->gps = gps;
    this->qrCode = qrCode;
    this->sensor = sensor;
}

String Errors::getString() {
    return String(battery) + String(init) + String(file) + String(gps) +
           String(qrCode) + String(sensor);
}

void Errors::log() {
    Logger.log("B" + String(battery) + "|I" +  String(init) + "|F" + String(file) + "|G" + String(gps) + "|Q" +
               String(qrCode) + "|S" + String(sensor));
}