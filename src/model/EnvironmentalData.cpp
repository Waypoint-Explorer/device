#include "EnvironmentalData.h"

EnvironmentalData::EnvironmentalData() {
    temperature = 0;
    humidity = 0;
    pressure = 0;
    airQuality = 0;
    gasResistance = 0;
}

String EnvironmentalData::toString() {
    return StringFormatter::formatNumberToStringWithSign(
               temperature, TEMPERATURE_LENGTH, true) +
           StringFormatter::formatNumberToStringWithSign(pressure,
                                                         PRESSURE_LENGTH) +
           StringFormatter::formatNumberToStringWithSign(humidity,
                                                         HUMIDITY_LENGTH) +
           StringFormatter::formatNumberToStringWithSign(airQuality,
                                                         AIR_QUALITY_LENGTH);
}

void EnvironmentalData::log() {
    Logger.log("Enviromental Data: T " + String(temperature) + " C° | P " +
               String(pressure) + " hPA | H " + String(humidity) + " % | AQ " +
               String(airQuality) + " % | GR " + String(gasResistance) +
               " Ohm");
}
