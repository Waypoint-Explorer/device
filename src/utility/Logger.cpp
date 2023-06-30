#include "Logger.h"

void LoggerService::begin(bool enable) {
    if (enable) Serial.begin(115200);
}

void LoggerService::log(const String& msg) { Serial.println(msg); }