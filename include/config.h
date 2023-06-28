#ifndef __CONFIG__
#define __CONFIG__

#include <Arduino.h>
#include <Wire.h>

// File name
#define FILENAME "/env_data.json"

// Task timer
#define mS_TO_S_FACTOR 1000
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP UINT32_C(60 * 60 * uS_TO_S_FACTOR)

// Task status
#define STATUS_DEAD 0
#define STATUS_ALIVE 1

// Errors
enum battery_error { BATTERY_OK, BATTERY_LOW };
enum init_error { INIT_OK, INIT_FAIL };
enum file_error { FILE_OK, FILE_CANT_OPEN, FILE_CANT_WRITE, FILE_CANT_READ };
enum gps_error { GPS_OK, GPS_CANT_LOCATE };
enum qrCode_error { QR_OK, QR_CANT_CREATE };
enum sensor_error { SENSOR_OK, SENSOR_CANT_READ };

// Pins
#define LED_BUILTIN 12

#endif