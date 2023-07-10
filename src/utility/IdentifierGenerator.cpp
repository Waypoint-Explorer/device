#include "IdentifierGenerator.h"

String IdentifierGenerator::generateUniqueNumberId(uint8_t idLength) {
    String id = "";
    for (int16_t i = 0; i < idLength ; i++) {
        id += ESPTrueRandom.random(10);
    }

    return id;
}