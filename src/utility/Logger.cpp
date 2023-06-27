#include "Logger.h"

void LoggerService::init() { 
    Serial.begin(115200); 
}

void LoggerService::log(const String& msg) { 
    Serial.println(msg); 
}