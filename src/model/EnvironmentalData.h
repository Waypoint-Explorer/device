#ifndef __ENVIRONMENTAL_DATA__
#define __ENVIRONMENTAL_DATA__

#include "Arduino.h"
#include "utility/Logger.h"
#include "utility/StringFormatter.h"

#define TEMPERATURE_LENGTH 3
#define HUMIDITY_LENGTH 2
#define PRESSURE_LENGTH 5
#define AIR_QUALITY_LENGTH 2

/* Environmental Data class definition */
class EnvironmentalData {
   public:
    /**
     * @brief Contstructor to initialize enviromental data
     */
    EnvironmentalData();

    int8_t temperature;     /* Temperature value as °C*/
    uint8_t humidity;       /* Humidity value as percentage */
    uint32_t pressure;      /* Pressure value as decaPascal */
    uint8_t airQuality;     /* Air Quality value as percentage */
    uint32_t gasResistance; /* Gas resistence value kOhm */

    /**
     * @brief Function to get data as string
     * @return String of environmental data
     */
    String toString();

    /**
     * @brief Function to debug data
     */
    void log();

   private:
};

#endif