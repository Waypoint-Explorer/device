#ifndef __BATTERY__
#define __BATTERY__

#include "Arduino.h"
#include "errors.h"
#include "utility/Logger.h"

#define ADC_PIN 35
#define VREF 1100
#define BATTERY_LOW_VOLTAGE 3.7
#define BATTERY_HIGH_VOLTAGE 4

/* Battery class definition */
class Battery {
   public:
    /**
     * @brief Function that returns battery level and get battery voltage
     * @param batteryVoltage : Voltage of the battery
     * @return Battery error as level (HIGH, HALF, LOW)
     */
    static BatteryError getBatteryLevel(float* batteryVoltage);

    /**
     * @brief Function to get battery level as string
     * @return String of battery voltage
     */
    static String toString();

    /**
     * @brief Function for debug battery
     */
    static void log();

   private:
};

#endif