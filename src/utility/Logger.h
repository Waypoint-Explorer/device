#ifndef __LOGGER__
#define __LOGGER__

#include "Arduino.h"

/// @brief Print logging data for debug
class LoggerService {
   public:
    /// @brief Init logger
    void init(bool enable);

    /// @brief Print log message
    /// @param msg log message
    void log(const String& msg);

};

extern LoggerService Logger;

#endif
