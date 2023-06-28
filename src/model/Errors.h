#ifndef __ERRORS__
#define __ERRORS__

#include "Arduino.h"
#include "config.h"
#include "utility/Logger.h"

class Errors {
   public:
    Errors();
    Errors(byte battery, byte init, byte file, byte gps, byte qrCode,
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