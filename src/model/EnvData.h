#ifndef __ENV_DATA__
#define __ENV_DATA__

#include "Arduino.h"
#include "utility/Logger.h"
#include "utility/StringFormatter.h"

#define TEMPERATURE_LENGTH 3
#define HUMIDITY_LENGTH 2
#define PRESSURE_LENGTH 5
#define AIR_QUALITY_LENGTH 2

/* Environmental Data class definition */
class EnvData {
   public:
    /**
     * @brief Contstructor to initialize enviromental data
     */
    EnvData();

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
     * @brief Function for debug data
     */
    void log();

   private:
    /**
     * @brief Function that format two digits number as string and add sign as 1
     * (positive) and 0 (negative) at begin
     * @param sign  : If the sign is needed
     * @param value : Value to convert in string
     * @return String of the data
     */
    String formatData(bool sign, int value);
};

#endif