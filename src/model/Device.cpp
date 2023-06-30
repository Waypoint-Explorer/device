#include "Device.h"

Device::Device() {
    errorsHandler = new ErrorsHandler();
}

void Device::setup() {
    id = IdentifierGenerator::generateUniqueNumberId(16);
    Logger.log("ID: " + id);
}

void Device::log(){
    Logger.log("ID: " + id);
    errorsHandler->log();
}