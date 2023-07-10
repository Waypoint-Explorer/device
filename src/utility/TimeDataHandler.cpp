#include "TimeDataHandler.h"

TimeDataHandler::TimeDataHandler() { rtc.offset = TIME_OFFSET; }

void TimeDataHandler::setTimeDate(int16_t sec, int16_t min, int16_t hour,
                                  int16_t day, int16_t month, int16_t year) {
    rtc.setTime(sec, min, hour, day, month, year);
}

String TimeDataHandler::getTime() { return rtc.getTime("%T"); }

String TimeDataHandler::getDate() { return rtc.getTime("%d/%m/%Y"); }

int64_t TimeDataHandler::getTimestamp() { return rtc.getEpoch(); }

int64_t TimeDataHandler::getTimestampMillis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}

int64_t TimeDataHandler::getSleepTimeInSeconds() {
    if (rtc.getMinute() == 0 && rtc.getSecond() == 0) return 0;
    return 3600 - (rtc.getMinute() * 60) - rtc.getSecond();
}

void TimeDataHandler::setOffset(uint32_t offset) { rtc.offset = offset; }

String TimeDataHandler::toString() { return String(getTimestamp()); }

void TimeDataHandler::log() { Logger.log("Timestamp: " + toString()); }