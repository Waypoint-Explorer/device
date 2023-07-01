#include "Gps.h"

TinyGsm modem(SerialAT);

GpsError Gps::begin() {
    modemPowerOn();
    SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
    int counter = 0;
    while (counter <= MAX_CYCLE_BEGIN) {
        counter++;
        if (!modem.testAT()) {
            Logger.log("Failed to restart modem!");
            modemRestart();
        } else {
            break;
        }
        delay(DELAY_MILLIS);
    }

    if (counter > MAX_CYCLE_BEGIN) return GPS_CANT_INIT;

    return GPS_OK;
}

GpsError Gps::getGpsData(TimeData timeData, Position* position) {
    Logger.log("GET GPS DATA");
    if (!enableGPS()) return GPS_CANT_ENABLE;

    float latitude, longitude, speed, accuracy, alt;
    int viewsat, usedsat, year, month, day, hour, min, sec, counter = 0;
    while (counter <= MAX_CYCLE_GPS) {
        counter++;
        if (modem.getGPS(&latitude, &longitude, &speed, &alt, &viewsat,
                         &usedsat, &accuracy, &year, &month, &day, &hour, &min,
                         &sec)) {
            if (position != NULL) {
                position->latitude = latitude;
                position->longitude = longitude;
                Logger.log("Lat: " + String(latitude, 6) +
                           " Lon: " + String(longitude, 6) + " | Accuracy: " +
                           String(accuracy) + " | Speed: " + String(speed) +
                           " | Altitude: " + String(alt) + " | View Sat: " +
                           String(viewsat) + " | Used Sat: " + String(usedsat));
            }
            timeData.SetTimeDate(sec, min, hour, day, month, year);
            break;
        }
        delay(DELAY_MILLIS);
    }

    if (counter > MAX_CYCLE_GPS) return GPS_CANT_LOCATE;

    Logger.log("Time & Date: " + timeData.getDate() + " " + timeData.getTime());

    if (!disableGPS()) return GPS_CANT_DISABLE;

    modemPowerOff();

    return GPS_OK;
}

bool Gps::enableGPS() {
    // Set Modem GPS Power Control Pin to HIGH, turn on GPS power
    modem.sendAT("+CGPIO=0,48,1,1");
    if (modem.waitResponse(GPS_RESPONSE) != 1) {
        Logger.log("Set GPS Power HIGH Failed");
        return false;
    }
    modem.enableGPS();
    return true;
}

bool Gps::disableGPS() {
    // Set Modem GPS Power Control Pin to LOW, turn off GPS power
    modem.sendAT("+CGPIO=0,48,1,0");
    if (modem.waitResponse(GPS_RESPONSE) != 1) {
        Logger.log("Set GPS Power LOW Failed");
        return false;
    }
    modem.disableGPS();
    return true;
}

void Gps::modemPowerOn() {
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(POWER_ON_MILLIS);  // Datasheet Ton = 1S
    digitalWrite(PWR_PIN, LOW);
}

void Gps::modemPowerOff() {
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(POWER_OFF_MILLIS);  // Datasheet Ton = 1.2S
    digitalWrite(PWR_PIN, LOW);
}

void Gps::modemRestart() {
    modemPowerOff();
    delay(DELAY_MILLIS);
    modemPowerOn();
}