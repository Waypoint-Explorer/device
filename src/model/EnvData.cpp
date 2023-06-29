#include "EnvData.h"

EnvData::EnvData() {
    temperature = 0;
    humidity = 0;
    pressure = 0;
    airQuality = 0;
    gasResistance = 0;
}

String EnvData::toString() {
    return String(temperature) + String(pressure) + String(humidity) +
           String(airQuality) + String(gasResistance);
}

void EnvData::log() {
    Logger.log("Enviromental Data: T " + String(temperature) + " C° | P " +
               String(pressure) + " hPA | H " + String(humidity) + " % | AQ " +
               String(airQuality) + " % | GR " + String(gasResistance) +
               " Ohm");
}
