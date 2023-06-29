#ifndef __ERRORS_HANLDER__
#define __ERRORS_HANDLER__

#include "Arduino.h"
#include "errors.h"
#include "utility/Logger.h"

class ErrorsHandler {
   public:
    ErrorsHandler();
    ErrorsHandler(byte battery, byte init, byte file, byte gps, byte qrCode,
           byte sensor);

    byte battery;  // battery low
    byte init;     // init error
    byte file;     // writing file error
    byte gps;      // gps unreachable error
    byte qrCode;   // generate qr code error
    byte sensor;   // read sensor error
    String getString();
    void log();

   private:
};

#endif