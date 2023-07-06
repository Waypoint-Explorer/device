#ifndef __CONFIG__
#define __CONFIG__

#include "Arduino.h"
#include "Button.h"
#include "Preferences.h"
#include "Wire.h"
#include "devices/Battery.h"
#include "devices/Display.h"
#include "devices/EnvSensor.h"
#include "devices/Gps.h"
#include "fonts.h"
#include "model/Device.h"
#include "model/EnvData.h"
#include "pins.h"
#include "utility/DeviceDataHandler.h"
#include "utility/Logger.h"
#include "utility/QrCodeHandler.h"

/* Configuration main */

// Task timer
#define mS_TO_S_FACTOR 1000
#define uS_TO_S_FACTOR 1000000LL
#define TIME_TO_SLEEP 60 * 60  // 60 min

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

// Task settings
#define CORE_0 0
#define CORE_1 1
#define UPDATE_BY_TIMER_TASK_WORDS 10000
#define UPDATE_BY_TIMER_TASK_PRIORITY 10

#define UPDATE_BY_BUTTON_TASK_WORDS 10000
#define UPDATE_BY_BUTTON_TASK_PRIORITY 9

#define DISPLAY_ROTATION 1

#define GET_GPS_DATA_COUNT 24
#define SENSOR_CALIBRATION_CYCLES 2

#endif