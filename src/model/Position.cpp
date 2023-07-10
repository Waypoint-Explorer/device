#include "Position.h"

Position::Position(float latitude, float longitude) {
    this->latitude = latitude;
    this->longitude = longitude;
}

String Position::toString() { return String((float) latitude, 6) + String((float) longitude, 6); }

void Position::log() {
    Logger.log("Position: Latitude: " + String((float) latitude, 6) +
               " | Longitude:  " + String((float) longitude, 6));
}