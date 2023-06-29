#ifndef __CONFIG__
#define __CONFIG__

#include "Arduino.h"
#include "Wire.h"

/* Configuration main */

// Task timer
#define mS_TO_S_FACTOR 1000
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP UINT32_C(60 * 60 * uS_TO_S_FACTOR)  // 60 min   // Test 3 * uS_TO_S_FACTOR

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
#define UPDATE_DATA_TASK_PRIORITY 10
#define UPDATE_DATA_TASK_WORDS 10000




#endif