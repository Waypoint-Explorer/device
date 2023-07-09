#ifndef __DEVICE__
#define __DEVICE__

#include "Arduino.h"
#include "EnvData.h"
#include "Position.h"
#include "Preferences.h"
#include "typedata.h"
#include "utility/ErrorsHandler.h"
#include "utility/IdentifierGenerator.h"

/* Device class definition */
class Device {
   public:
    /**
     * @brief Constructor for Device
     */
    Device();

    /**
     * @brief Function to setup the device data
     */
    void setup();

    /* Unique id as string*/
    String id;

    /* Init state */
    bool init;

    /* Position as latitude and longitude */
    Position* position;

    /* Error handler */
    ErrorsHandler* errorsHandler;

    /* Last environmental data measured */
    EnvData* lastEnvData;

    /**
     * @brief Function that end preferences
     */
    void endPreferences();

    /**
     * @brief Function that store init state to preferences
     */
    void storeInitToPreferences();

    /**
     * @brief Function that store Id to preferences
     */
    void storeIdToPreferences();

    /**
     * @brief Function that store position to preferences
     */
    void storePositionToPreferences();

    /**
     * @brief Function that store last environmental data to RTC memory
     * @param lastEnvDataRTC : Last environmental data struct RTC
     */
    void storeLastEnvDataToRTC(EnvDataStruct* lastEnvDataRTC);

    /**
     * @brief Function that store errors to RTC memory
     * @param errorsRTC : Errors struct RTC
     */
    void storeErrorsToRTC(ErrorsStruct* errorsRTC);

    /**
     * @brief Function that retrieve init state from preferences
     */
    void retrieveInitFromPreferences();

    /**
     * @brief Function that retrieve Id from preferences
     */
    void retrieveIdFromPreferences();

    /**
     * @brief Function that retrieve position from preferences
     */
    void retrievePositionFromPreferences();

    /**
     * @brief Function that retrieve last environmental data from RTC memory
     * @param lastEnvDataRTC : Last environmental data struct RTC
     */
    void retrieveEnvDataFromRTC(EnvDataStruct* lastEnvDataRTC);

    /**
     * @brief Function that retrieve errors from RTC memory
     * @param errorsRTC : Errors struct RTC
     */
    void retrieveErrorsFromRTC(ErrorsStruct* errorsRTC);

    /**
     * @brief Function for debug device
     */
    void log();

   private:
    /* Preferences to store data to flash */
    Preferences preferences;
};

#endif