#include "Device.h"

Device::Device() {
    envDataList = new LinkedList<EnvData>();
    errorsHandler = new ErrorsHandler();
}

void Device::setup() {
    id = IdentifierGenerator::generateUniqueNumberId(16);
    
}

String Device::getId() { return id; }
