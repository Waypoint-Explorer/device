#ifndef __ENTRY_DATA__
#define __ENTRY_DATA__

#include "Arduino.h"
#include "EnvData.h"
#include "utility/Logger.h"

/* Entry data class definition */
class EntryData {
   public:
    /**
     * @brief Contstructor to initialize entry data
     * @param envData : Environmental data
     * @param timestamp : Timestamp
     */
    EntryData(EnvData envData, int64_t timestamp);

    EnvData envData;
    int64_t timestamp;

    /**
     * @brief Function to get entry data as string
     * @return String of entry data
     */
    String toString();

    /**
     * @brief Function for debug entry data
     */
    void log();

   private:
};

#endif