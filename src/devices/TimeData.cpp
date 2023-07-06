#include "TimeData.h"

TimeData::TimeData() { rtc.offset = ITALY_OFFSET; }

void TimeData::setTimeDate(int sec, int min, int hour, int day, int month,
                           int year) {
    rtc.setTime(sec, min, hour, day, month, year);
}

String TimeData::getTime() { return rtc.getTime("%T"); }

String TimeData::getDate() { return rtc.getTime("%d/%m/%Y"); }

int64_t TimeData::getTimestamp() { return rtc.getEpoch(); }

int64_t TimeData::getTimestampMillis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}

int64_t TimeData::getSleepTimeInSeconds() {
    if (rtc.getMinute() == 0 && rtc.getSecond() == 0) return 0;
    return 3600 - (rtc.getMinute() * 60) - rtc.getSecond();
}

void TimeData::setOffset(uint32_t offset) {
    rtc.offset = offset + setDaylightSavingOffset();
}

String TimeData::toString() { return String(getTimestamp()); }

void TimeData::log() { Logger.log("Timestamp: " + toString()); }

uint32_t setDaylightSavingOffset() {
    // TODO: low priority
    return 0;
}