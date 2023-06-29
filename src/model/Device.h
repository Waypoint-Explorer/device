#ifndef __DEVICE__
#define __DEVICE__

#include "Arduino.h"
#include "EnvData.h"
#include "LinkedList.h"
#include "utility/ErrorsHandler.h"
#include "utility/IdentifierGenerator.h"

#define MAX_ENVDATA_ENTRIES 1000

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

    /**
     * @brief Function to get the id
     * @return Id as string
     */
    String getId();

    /* List of environmental data */
    LinkedList<EnvData>* envDataList;

    /* Error handler */
    ErrorsHandler* errorsHandler;

   private:
    String id;
};

#endif