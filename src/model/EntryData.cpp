#include "EntryData.h"

EntryData::EntryData(){
    this->envData = EnvironmentalData();
    this->timestamp = 0;
}

EntryData::EntryData(EnvironmentalData envData, int64_t timestamp){
    this->envData = envData;
    this->timestamp = timestamp;
}

String EntryData::toString() { return String(timestamp) + envData.toString(); }

void EntryData::log() {
    Logger.log("Timestamp: " + String(timestamp));
    envData.log();
}