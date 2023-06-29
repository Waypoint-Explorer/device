#ifndef __CONFIG__
#define __CONFIG__

#include "Arduino.h"
#include "Wire.h"

// File name
#define FILENAME "/env_data.json"

// Task timer
#define mS_TO_S_FACTOR 1000
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP UINT32_C(60 * 60 * uS_TO_S_FACTOR) // 60 min   // Test 3 * uS_TO_S_FACTOR

// Admin reset
#define TIME_RESET 5 * uS_TO_S_FACTOR
#define TIME_LONGPRESS_RESET 3 * mS_TO_S_FACTOR

// Button pression type
#define LONG_PRESS -1
#define SHORT_PRESS 1
#define NO_PRESS 0

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
#define RESET_BUTTON GPIO_NUM_27

#endif