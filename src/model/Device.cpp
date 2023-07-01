#include "Device.h"

Device::Device() {
    position = new Position(0, 0);
    errorsHandler = new ErrorsHandler();
}

void Device::setup() {
    id = IdentifierGenerator::generateUniqueNumberId(16);
    Logger.log("ID: " + id);
}

void Device::log(){
    Logger.log("ID: " + id);
    errorsHandler->log();
    position->log();
}