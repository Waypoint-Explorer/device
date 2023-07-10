#ifndef __ERRORS_DATA__
#define __ERRORS_DATA__

#include "Arduino.h"
#include "errors.h"
#include "utility/Logger.h"

/* Errors data class definition */
class ErrorsData {
   public:
    /**
     * @brief Constructor to initialize everything to OK
     */
    ErrorsData();

    /**
     * @brief Constructor to initialize errors with values
     * @param battery : Battery error
     * @param init    : Init error
     * @param file    : File management error
     * @param gps     : Gps error
     * @param qrCode  : QR code error
     * @param sensor  : Sensor error
     */
    ErrorsData(uint8_t battery, uint8_t init, uint8_t file, uint8_t gps, uint8_t qrCode,
                  uint8_t sensor);

    uint8_t battery;
    uint8_t init;
    uint8_t file;
    uint8_t gps;
    uint8_t qrCode;
    uint8_t sensor;

    /**
     * @brief Function to get all errors as a single string of numbers
     * @return String of errors
     */
    String toString();

    /**
     * @brief Function to debug errors
     */
    void log();
};

#endif