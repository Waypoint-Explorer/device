#ifndef __ERRORS_HANLDER__
#define __ERRORS_HANDLER__

#include "Arduino.h"
#include "errors.h"
#include "utility/Logger.h"

/* Errors Handler class definition */
class ErrorsHandler {
   public:
    /**
     * @brief Constructor to initialize everything to OK
     */
    ErrorsHandler();

    /**
     * @brief Constructor to initialize errors with values
     * @param battery : Battery error
     * @param init    : Init error
     * @param file    : File management error
     * @param gps     : Gps error
     * @param qrCode  : QR code error
     * @param sensor  : Sensor error
     */
    ErrorsHandler(byte battery, byte init, byte file, byte gps, byte qrCode,
                  byte sensor);

    byte battery;
    byte init;
    byte file;
    byte gps;
    byte qrCode;
    byte sensor;

    /**
     * @brief Function to get all errors as a single string of numbers
     * @return String of errors
     */
    String getString();

    /**
     * @brief Function to debug errors
     */
    void log();
};

#endif