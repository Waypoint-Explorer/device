#include "IdentifierGenerator.h"

String IdentifierGenerator::generateUniqueNumberId(byte idLength) {
    String id = "";
    for (int i = 0; i < idLength - 1; i++) {
        id += ESPTrueRandom.random(10);
    }

    return id;
}