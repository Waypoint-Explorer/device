#ifndef __TIME_DATA__
#define __TIME_DATA__

#include "Arduino.h"
#include "ESP32Time.h"
#include "utility/Logger.h"

#define ITALY_OFFSET 3600  // GMT+1

/* Time Data class definition */
class TimeData {
   public:
    /**
     * @brief Constructor for time data
     */
    TimeData();

    /**
     * @brief Function to set the internal RTC time
     * @param  sec   : second (0-59)
     * @param  min   : minute (0-59)
     * @param  hour  : hour of day (0-23)
     * @param  day   : day of month (1-31)
     * @param  month : month (1-12)
     * @param  year  : year ie 2021
     */
    void SetTimeDate(int sec, int min, int hour, int day, int month, int year);

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
    void setOffset(uint32_t offset = ITALY_OFFSET);

    /**
     * @brief Function to get time data as string
     * @return String timestamp
     */
    String toString();

    /**
     * @brief Function for debug timestamp
     */
    void log();

   private:
    ESP32Time rtc;

    /**
     * @brief Function to set the daylight saving offset
     * @return  Offset daylight saving (0 or 3600)
     */
    uint32_t setDaylightSavingOffset();
};

#endif