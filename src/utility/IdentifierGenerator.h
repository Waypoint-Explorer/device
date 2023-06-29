#ifndef __IDENTIFIER_GENERATOR__
#define __IDENTIFIER_GENERATOR__

#include "Arduino.h"
#include "ESPTrueRandom.h"

/*Identifier Generator class definition*/
class IdentifierGenerator {
   public:
   /**
    * @brief Function thats generates an unique id of numbers
    * @param idLength : Length of the id
    * @return id as a string of numbers
   */
    static String generateUniqueNumberId(byte idLength);
};

#endif
