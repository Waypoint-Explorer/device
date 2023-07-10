#include "StringFormatter.h"

String StringFormatter::formatNumberToStringWithSign(int16_t value,
                                                     int16_t length,
                                                     bool sign) {
    String str = "";
    if (sign) {
        if (value < 0)
            str += "0";  // Negative -
        else
            str += "1";  // Positive +
        length--;
    }
    return str + formatNumberToString(value, length);
}

String StringFormatter::formatNumberToString(int16_t value, int16_t length) {
    char buffer[length + 1];
    String restrict = "%0" + String(length) + "d";
    if (snprintf(buffer, sizeof(buffer), restrict.c_str(), value) == -1 ||
        String(value).length() > length)
        snprintf(buffer, sizeof(buffer), restrict.c_str(), 0);
    return String(buffer);
}
