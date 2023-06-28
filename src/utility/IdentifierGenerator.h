#ifndef __IDENTIFIER_GENERATOR__
#define __IDENTIFIER_GENERATOR__

#include "Arduino.h"
#include "ESPTrueRandom.h"

class IdentifierGenerator {
   public:
    static String generateUniqueNumberId(byte idLength);
};

#endif
