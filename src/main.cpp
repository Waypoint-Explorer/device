#include "Button.h"
#include "Preferences.h"
#include "config.h"
#include "devices/EnvSensor.h"
#include "devices/Gps.h"
#include "model/Device.h"
#include "model/EnvData.h"
#include "pins.h"
#include "utility/DeviceDataHandler.h"
#include "utility/Logger.h"

LoggerService Logger;
Preferences preferences;

Device* device;
EnvSensor envSensor;
Gps gps;
TimeData timeData;
Button btnReset(RESET_BUTTON, TIME_LONGPRESS_RESET);

TaskHandle_t UpdateDataTask;
static volatile uint8_t dataTaskStatus = STATUS_DEAD;

// Function called by UpdateDataTask to update environmental data
void updateData(void* parameter) {
    Logger.log("- BORN ENV DATA");
    dataTaskStatus = STATUS_ALIVE;
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        Logger.log("-DATE/TIME UPDATE");
        device->errorsHandler->gps = gps.getGpsData(timeData);

        Logger.log("- ENV DATA UPDATE");

        DeviceDataHandler::writeLastEnvData(
            EntryData(envSensor.getEnvData(), timeData.getTimestamp()), device);
    }
    Logger.log("- DEAD ENV DATA");
    dataTaskStatus = STATUS_DEAD;
    vTaskDelete(UpdateDataTask);
}

// Setup function
void setup() {
    Logger.begin(true);
    Logger.log(".:: SETUP Device ::.");

    pinMode(LED_BUILTIN, OUTPUT);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);

    preferences.begin("device");

    device = new Device();
    timeData = TimeData();

    DeviceDataHandler::begin();
    gps.begin();
    device->errorsHandler->sensor = envSensor.begin();

    // Check if wake up cause is undefined
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
        // First seconds wait and check for reset button pressed
        while (esp_timer_get_time() <= TIME_RESET) {
            if (btnReset.checkPress() == LONG_PRESS) {
                Logger.log("! DEVICE RESET");
                // Reset device
                preferences.putBool("init", false);
                DeviceDataHandler::reset();
                ESP.restart();
            }
        }

        // Check if already initialized
        if (preferences.getBool("init") == false) {
            Logger.log("@ INIT DATA");
            // Setup device data
            preferences.putBool("init", true);
            device->setup();
            device->errorsHandler->file =
                DeviceDataHandler::initEnvDataJsonArray();
            device->errorsHandler->gps =
                gps.getGpsData(timeData, device->position);

            // Save preferences
            preferences.putString("id", device->id);
            preferences.putBool("init", true);
            preferences.putFloat("latitude", device->position->latitude);
            preferences.putFloat("longitude", device->position->longitude);
            envSensor.calibrate(20);
        }

        Logger.log(".:: SLEEP Device ::.");
        esp_deep_sleep_start();
    }

    // Get data from Preferences
    device->id = preferences.getString("id");
    device->position->latitude = preferences.getFloat("latitude");
    device->position->longitude = preferences.getFloat("longitude");

    preferences.end();

    // Create update data task
    xTaskCreatePinnedToCore(updateData, "updateDataTask",
                            UPDATE_DATA_TASK_WORDS, NULL,
                            UPDATE_DATA_TASK_PRIORITY, &UpdateDataTask, CORE_0);

    // Wait task to finish
    delay(100);
    while (dataTaskStatus == STATUS_ALIVE) {
    }

    device->log();

    Logger.log(".:: SLEEP Device ::.");
    esp_deep_sleep_start();
}

// Loop functionn never used
void loop() {}