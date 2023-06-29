#ifndef __ENV_DATA__
#define __ENV_DATA__

#include "Arduino.h"
#include "utility/Logger.h"

/* Environmental Data class definition */
class EnvData {
   public:
    /**
     * @brief Contstructor to initialize enviromental data
     */
    EnvData();

    int8_t temperature;
    uint8_t humidity;
    uint32_t pressure;
    uint8_t airQuality;
    uint32_t gasResistance;

    /**
     * @brief Function to get data as string
     * @return String of environmental data
     */
    String toString();

    /**
     * @brief Function for debug data
     */
    void log();
};

#endif