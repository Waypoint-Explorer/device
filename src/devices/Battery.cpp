#include "Battery.h"

BatteryError Battery::getBatteryLevel(float* batteryVoltage) {
    uint16_t v = analogRead(ADC_PIN);
    *batteryVoltage = ((float)v / 4095.0) * 2.0 * 3.3 * (VREF / 1000.0);

    if (*batteryVoltage >= BATTERY_HIGH_VOLTAGE)
        return BATTERY_HIGH;
    else if (*batteryVoltage <= BATTERY_LOW_VOLTAGE)
        return BATTERY_LOW;
    else
        return BATTERY_HALF;
}

String Battery::toString() {
    float batteryLevel = 0;
    switch (getBatteryLevel(&batteryLevel)) {
        case BATTERY_HIGH:
            return "HIGH";
            break;

        case BATTERY_HALF:
            return "HALF";
            break;

        case BATTERY_LOW:
            return "LOW";
            break;
        default:
            return "EMPTY";
    }
}

void Battery::log() { Logger.log("Battery: " + toString()); }