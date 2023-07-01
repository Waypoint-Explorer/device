#ifndef __GPS__
#define __GPS__

#include "Arduino.h"
#include "TimeData.h"
#include "Wire.h"
#include "errors.h"
#include "model/Position.h"
#include "utility/Logger.h"

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024  // Set RX buffer to 1Kb
#define SerialAT Serial1

// #define SerialAT Serial1
#include <TinyGsmClient.h>

#define UART_BAUD 9600
#define PIN_TX 27
#define PIN_RX 26
#define PWR_PIN 4

#define MAX_CYCLE_BEGIN 50
#define MAX_CYCLE_GPS 200
#define DELAY_MILLIS 1000
#define POWER_ON_MILLIS 1000
#define POWER_OFF_MILLIS 1500
#define GPS_RESPONSE 10000L

/*#define LAT 44.408836
#define LON 12.198644*/

/* Gps class definition */
class Gps {
   public:
    /**
     * @brief Function to initialize gps modem
     * @return GpsError in case of error
     */
    GpsError begin();

    /**
     * @brief Function to initialize gps modem
     * @param timeData : TimeData for time and date
     * @param position : Position coordinates
     * @return GpsError in case of error
     */
    GpsError getGpsData(TimeData timeData, Position* position = NULL);

   private:
    /**
     * @brief Function to enable gps modem
     * @return false in case of error, true otherwise
     */
    bool enableGPS();

    /**
     * @brief Function to disable gps modem
     * @return false in case of error, true otherwise
     */
    bool disableGPS();

    /**
     * @brief Function to power on modem
     */
    void modemPowerOn();

    /**
     * @brief Function to power off modem
     */
    void modemPowerOff();

    /**
     * @brief Function to restart modem
     */
    void modemRestart();
};

#endif