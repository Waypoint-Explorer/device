#ifndef __DEVICE__
#define __DEVICE__

#include "Arduino.h"
#include "Position.h"
#include "utility/ErrorsHandler.h"
#include "utility/IdentifierGenerator.h"

#define MAX_ENVDATA_ENTRIES 1000

/* Device class definition */
class Device {
   public:
    /**
     * @brief Constructor for Device
     */
    Device();

    /**
     * @brief Function to setup the device data
     */
    void setup();

    /* Unique id as string*/
    String id;

    /* Position as latitude and longitude */
    Position* position;

    /* Error handler */
    ErrorsHandler* errorsHandler;

    /**
     * @brief Function for debug device
     */
    void log();

   private:
};

#endif