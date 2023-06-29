#ifndef __LOGGER__
#define __LOGGER__

#include "Arduino.h"

/* Logger Service class definition*/
class LoggerService {
   public:
    /**
     * @brief Function to initialize logger
     * @param enable : Enable or disable logger
     */
    void init(bool enable);

    /**
     * @brief Function to print log message
     * @param msg : Log message
     */
    void log(const String& msg);
};

extern LoggerService Logger;

#endif
