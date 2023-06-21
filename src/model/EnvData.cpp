#include "EnvData.h"

EnvData::EnvData() {
    temperature = 0;
    humidity = 0;
    pressure = 0;
    airQuality = 0;
}

String EnvData::toString() {
    return "Enviromental Data: " + String(temperature) + " C° | " +
           String(pressure) + " hPA | " + String(humidity) + " % | " +
           String(airQuality);
}
