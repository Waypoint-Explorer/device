#include "Device.h"

Device::Device() {
    envDataList = new LinkedList<EnvData>();
    errors = new Errors();
}

void Device::setup() {
    id = IdentifierGenerator::generateUniqueNumberId(16);
    
}

String Device::getId() { return id; }
