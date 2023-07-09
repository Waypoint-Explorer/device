#include "StringFormatter.h"

String StringFormatter::formatNumberToStringWithSign(int value, int length, bool sign) {
    String str = "";
    if (sign) {
        if (value < 0)
            str += "0";
        else
            str += "1";
        length--;
    }
    return str + formatNumberToString(value, length);
}

String StringFormatter::formatNumberToString(int value, int length) {
    char buffer[length + 1];
    String restrict = "%0" + String(length) + "d";
    if (snprintf(buffer, sizeof(buffer), restrict.c_str(), value) == -1 || String(value).length() > length) 
        snprintf(buffer, sizeof(buffer), restrict.c_str(), 0);
    return String(buffer);
}
