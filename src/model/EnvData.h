#ifndef __ENV_DATA__
#define __ENV_DATA__

#include "Arduino.h"

/// @brief Containing environmental data
class EnvData {
   public:
    /// @brief Init environmental data
    EnvData();
    
    int8_t temperature;
    uint8_t humidity;
    uint32_t pressure;
    uint8_t airQuality;

    /// @brief Data to string
    /// @return string containing environmental data
    String toString();
};

#endif