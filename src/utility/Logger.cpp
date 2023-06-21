#include "Logger.h"

void LoggerService::init() { 
    Serial.begin(112500); 
}

void LoggerService::log(const String& msg) { 
    Serial.println(msg); 
}