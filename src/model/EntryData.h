#ifndef __ENTRY_DATA__
#define __ENTRY_DATA__

#include "Arduino.h"
#include "EnvironmentalData.h"
#include "utility/Logger.h"

/* Entry data class definition */
class EntryData {
   public:
    /**
     * @brief Constructor for EntryData
     */
    EntryData();

    /**
     * @brief Contstructor to initialize entry data
     * @param envData : Environmental data
     * @param timestamp : Timestamp
     */
    EntryData(EnvironmentalData envData, int64_t timestamp);

    EnvironmentalData envData;
    int64_t timestamp;

    /**
     * @brief Function to get entry data as string
     * @return String of entry data
     */
    String toString();

    /**
     * @brief Function to debug entry data
     */
    void log();

   private:
};

#endif