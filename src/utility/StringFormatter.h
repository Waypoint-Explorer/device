#ifndef __STRING_FORMATTER__
#define __STRING_FORMATTER__

#include "Arduino.h"
#include "utility/Logger.h"

/* String formatter class definition */
class StringFormatter {
   public:
    /**
     * @brief Function that add sign and 0 as padding to the length desired
     * @param value  : Value to format
     * @param length : Number of digits of the value
     * @param sign   : Enable sign
     */
    static String formatNumberToStringWithSign(int value, int length,
                                        bool sign = false);


   private:
    /**
     * @brief Function that add 0 as padding to the length desired
     * @param value  : Value to format
     * @param length : Number of digits of the value
     */
    static String formatNumberToString(int value, int length);
};

#endif