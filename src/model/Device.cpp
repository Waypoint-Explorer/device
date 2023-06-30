#include "Device.h"

Device::Device() {
    position = new Position(0, 0);
    envDataList = new LinkedList<EnvData>();
    errorsHandler = new ErrorsHandler();
}

void Device::setup() { id = IdentifierGenerator::generateUniqueNumberId(16); }

String Device::getId() { return id; }
void Device::log() {
    Logger.log("Id: " + id);
    position->log();
}