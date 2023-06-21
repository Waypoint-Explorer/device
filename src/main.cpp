#include "config.h"
#include "devices/EnvSensor.h"
#include "model/EnvData.h"
#include "utility/Logger.h"

EnvSensor envSensor;
LoggerService Logger;

void setup() {
    Logger.init();
    Logger.log(".:: Init Device ::.");
    envSensor.init();
}

void loop() { 
    
}