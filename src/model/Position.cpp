#include "Position.h"

Position::Position(float latitude, float longitude) {
    this->latitude = latitude;
    this->longitude = longitude;
}

String Position::toString() { return String(latitude) + String(longitude); }

void Position::log() {
    Logger.log("Position: Latitude: " + String(latitude) +
               " | Longitude:  " + String(longitude));
}