#ifndef __STRING_FORMATTER__
#define __STRING_FORMATTER__

#include "Arduino.h"
#include "utility/Logger.h"

/* String formatter class definition */
class StringFormatter {
   public:
    /**
     * @brief Function that add sign (1 positive and 0 negative) and 0 as padding to the length desired
     * @param value  : Value to format
     * @param length : Number of digits of the value
     * @param sign   : Enable sign
     * @return string formatted number with sign and padding
     */
    static String formatNumberToStringWithSign(int16_t value, int16_t length,
                                        bool sign = false);


   private:
    /**
     * @brief Function that add 0 as padding to the length desired
     * @param value  : Value to format
     * @param length : Number of digits of the value
     * @return string formatted number with padding
     */
    static String formatNumberToString(int16_t value, int16_t length);
};

#endif