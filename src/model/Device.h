#ifndef __DEVICE__
#define __DEVICE__

#include "Arduino.h"
#include "EnvData.h"
#include "LinkedList.h"
#include "utility/ErrorsHandler.h"
#include "utility/IdentifierGenerator.h"

#define MAX_ENVDATA_ENTRIES 1000

class Device {
   public:
    Device();
    void setup();
    String getId();

    LinkedList<EnvData>* envDataList;
    ErrorsHandler* errorsHandler;

   private:
    String id;
};

#endif