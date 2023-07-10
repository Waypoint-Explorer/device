#ifndef __TYPE_DATA__
#define __TYPE_DATA__

#include "Arduino.h"

/* Type data structs for RTC */

struct EnvDataStruct {
    int8_t temperature;
    uint8_t humidity;
    uint32_t pressure;
    uint8_t airQuality;
};

struct ErrorsStruct {
    uint8_t battery;
    uint8_t init;
    uint8_t file;
    uint8_t gps;
    uint8_t qrCode;
    uint8_t sensor;
};

#endif