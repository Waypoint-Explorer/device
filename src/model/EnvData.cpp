#include "EnvData.h"

EnvData::EnvData() {
    temperature = 0;
    humidity = 0;
    pressure = 0;
    airQuality = 0;
    gasResistance = 0;
}

String EnvData::toString() {
    return formatData(true, temperature) + String(pressure) + formatData(false, humidity) +
           formatData(false, airQuality);
}

void EnvData::log() {
    Logger.log("Enviromental Data: T " + String(temperature) + " C° | P " +
               String(pressure) + " hPA | H " + String(humidity) + " % | AQ " +
               String(airQuality) + " % | GR " + String(gasResistance) +
               " Ohm");
}

String EnvData::formatData(bool sign, int value) {
    //* TODO: generalize to more digits
    String str = "";
    if (sign)
        if (value < 0)
            str += "0";
        else
            str += "1";

    if (abs(value) < 10) str += "0";

    str += abs(value);
    return str;
}
