#ifndef __TIME_DATA__
#define __TIME_DATA__

#include "Arduino.h"
#include "ESP32Time.h"
#include "utility/Logger.h"

#define TIME_OFFSET 0

/* Time Data class definition */
class TimeDataHandler {
   public:
    /**
     * @brief Constructor for time data handler
     */
    TimeDataHandler();

    /**
     * @brief Function to set the internal RTC time
     * @param  sec   : second (0-59)
     * @param  min   : minute (0-59)
     * @param  hour  : hour of day (0-23)
     * @param  day   : day of month (1-31)
     * @param  month : month (1-12)
     * @param  year  : year ie 2021
     */
    void setTimeDate(int16_t sec, int16_t min, int16_t hour, int16_t day,
                     int16_t month, int16_t year);

    /**
     * @brief Function that return date as string
     * @return Date as string ie 30/06/2023
     */
    String getDate();

    /**
     * @brief Function that return time as string
     * @return Time as string ie 12:34:56
     */
    String getTime();

    /**
     * @brief Function that return timestamp
     * @return Timestamp as long long
     */
    int64_t getTimestamp();

    /**
     * @brief Function that return timestamp in millis
     * @return Timestamp in millis as long long
     */
    int64_t getTimestampMillis();

    /**
     * @brief Function to set the internal RTC time
     * @param  offset : Offset in seconds
     */
    void setOffset(uint32_t offset = TIME_OFFSET);

    /**
     * @brief Function that return sleep time in seconds (hour by hour)
     * @return Seconds to sleep
     */
    int64_t getSleepTimeInSeconds();

    /**
     * @brief Function to get time data as string
     * @return String timestamp
     */
    String toString();

    /**
     * @brief Function to debug timestamp
     */
    void log();

   private:
    ESP32Time rtc;
};

#endif