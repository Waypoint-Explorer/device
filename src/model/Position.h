#ifndef __POSITION__
#define __POSITION__

#include "Arduino.h"
#include "utility/Logger.h"

/* Position class definition */
class Position {
   public:
    /**
     * @brief Contstructor to initialize position
     * @param latitude : Latitude as float
     * @param longitude : Longitude as float
     */
    Position(float latitude, float longitude);

    float latitude;
    float longitude;

    /**
     * @brief Function to get coordinates as string
     * @return String of coordinates
     */
    String toString();

    /**
     * @brief Function for debug position
     */
    void log();

   private:
};

#endif