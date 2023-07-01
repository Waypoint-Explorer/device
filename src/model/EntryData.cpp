#include "EntryData.h"

EntryData::EntryData(EnvData envData, int64_t timestamp){
    this->envData = envData;
    this->timestamp = timestamp;
}

String EntryData::toString() { return String(timestamp) + envData.toString(); }

void EntryData::log() {
    Logger.log("Timestamp: " + String(timestamp));
    envData.log();
}