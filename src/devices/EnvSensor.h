#ifndef __ENV_SENSOR__
#define __ENV_SENSOR__

#include "Arduino.h"
#include "bsec.h"
#include "model/EnvData.h"
#include "utility/Logger.h"

class EnvSensor {
   public:
    Bsec sensor;

    /// @brief Init sensor
    bool init();

    /// @brief Ger environmental data from sensor
    /// @return environmental data
    EnvData getEnvData();

   private:
    bool checkSensor();
};

#endif