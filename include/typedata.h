#ifndef __TYPE_DATA__
#define __TYPE_DATA__

#include "Arduino.h"

/* Type data structs for RTC */

struct EnvDataStruct {
    int temperature;
    byte humidity;
    int pressure;
    byte airQuality;
};

struct ErrorsStruct {
    byte battery;
    byte init;
    byte file;
    byte gps;
    byte qrCode;
    byte sensor;
};

#endif