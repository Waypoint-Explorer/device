#ifndef __ENV_SENSOR__
#define __ENV_SENSOR__

#include "Arduino.h"
#include "bsec.h"
#include "errors.h"
#include "model/EnvData.h"
#include "utility/Logger.h"

#define HUMIDITY_WEIGHT 0.25
#define HUMIDITY_REFERENCE 40
#define HUMIDITY_CORRECTION 5  // Correction value

#define GAS_WEIGHT 0.75
#define GAS_LOWER_LIMIT 5000
#define GAS_UPPER_LIMIT 65000

#define CALIBRATION_DELAY_MILLIS 3000

/* Environmental Sensor class definition */
class EnvSensor {
   public:
    Bsec sensor;

    /**
     *  @brief Begin sensor
     */
    SensorError begin();

    /**
     * @brief Get environmental data from sensor
     * @return environmental data object
     */
    EnvData getEnvData();

    /**
     * @brief Function to get calibrated data from sensor
     * @param cycles : Cycles of calibration
     */
    EnvData getCalibratedEnvData(int16_t cycles);

   private:
    /**
     * @brief Function to check sensor errors
     * @return true if no errors, false otherwise
     */
    bool checkSensor();

    /**
     * @brief Function to get humidity score
     * @param humidity : Humidity as float
     * @return humidity score value as float
     */
    float getHumidityScore(float humidity);

    /**
     * @brief Function to get gas resistance score
     * @param gasResistance : Gas resistance as float
     * @return gas score value as float
     */
    float getGasScore(float gasResistance);

    /**
     * @brief Function to get air quality
     * @param humidity : Humidity as float
     * @param gasResistance : Gas resistance as float
     * @return air quality value as integer
     */
    int16_t getAirQulity(float humidity, float gasResistance);

    /**
     * @brief Function to format pressure to 5 digits
     * @param pressure : pressure as float
     * @return pressure formatted as integer
     */
    int16_t formatPressure(float pressure);

    // State of BSEC sensor
    uint8_t bsecState[BSEC_MAX_STATE_BLOB_SIZE] = {
        2,   9,   4,   1,   61,  0,   0,   0,   0,   0,   0,  0,   131,
        0,   0,   0,   56,  0,   1,   0,   61,  47,  209, 64, 166, 49,
        209, 64,  105, 56,  205, 64,  58,  55,  205, 64,  0,  0,   0,
        0,   44,  1,   20,  20,  0,   0,   64,  64,  0,   0,  64,  64,
        0,   0,   0,   0,   3,   0,   23,  168, 95,  63,  20, 0,   0,
        225, 68,  0,   0,   225, 68,  1,   12,  0,   2,   0,  95,  69,
        122, 66,  123, 33,  104, 66,  16,  0,   3,   0,   9,  228, 177,
        64,  32,  87,  236, 65,  243, 28,  132, 66,  27,  0,  5,   0,
        0,   92,  198, 166, 184, 0,   0,   0,   0,   0,   0,  0,   98,
        24,  141, 82,  1,   1,   0,   202, 102, 56,  83,  12, 0,   9,
        0,   0,   92,  198, 166, 184, 0,   0,   0,   8,   0,  10,  0,
        117, 83,  234, 65,  0,   0,   0,   0,   140, 128, 0,  0};
};

#endif