#ifndef __CONFIG__
#define __CONFIG__

#include <Arduino.h>
#include <Wire.h>

// File name
#define FILENAME "/env_data.json"


// Errors
enum battery_error { BATTERY_OK, BATTERY_LOW };
enum init_error { INIT_OK, INIT_FAIL };
enum file_error { FILE_OK, FILE_CANT_OPEN, FILE_CANT_WRITE, FILE_CANT_READ };
enum gps_error { GPS_OK, GPS_CANT_LOCATE };
enum qrCode_error { QR_OK, QR_CANT_CREATE };
enum sensor_error { SENSOR_OK, SENSOR_CANT_READ };



#endif