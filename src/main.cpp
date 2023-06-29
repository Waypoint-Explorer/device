#include "Button.h"
#include "Preferences.h"
#include "config.h"
#include "devices/EnvSensor.h"
#include "model/Device.h"
#include "model/EnvData.h"
#include "pins.h"
#include "utility/DeviceDataHandler.h"
#include "utility/Logger.h"

EnvSensor envSensor;
LoggerService Logger;
Preferences preferences;

TaskHandle_t UpdateDataTask;
static volatile uint8_t dataTaskStatus = STATUS_DEAD;

Device* device;

Button btnReset(RESET_BUTTON, TIME_LONGPRESS_RESET);

// Function called by UpdateDataTask to update environmental data
void updateData(void* parameter) {
    Logger.log("- BORN ENV DATA");
    dataTaskStatus = STATUS_ALIVE;
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        Logger.log("- ENV DATA UPDATE");
        DeviceDataHandler::updateEnvData(envSensor.getEnvData(), device);
    }
    Logger.log("- DEAD ENV DATA");
    dataTaskStatus = STATUS_DEAD;
    vTaskDelete(UpdateDataTask);
}

// Setup function
void setup() {
    Logger.init(true);
    Logger.log(".:: SETUP Device ::.");

    pinMode(LED_BUILTIN, OUTPUT);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);

    preferences.begin("device");

    device = new Device();
    device->errorsHandler->sensor = envSensor.init();

    DeviceDataHandler::init();

    // Check wakeup type
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
        // First seconds wait and check for reset button pressed
        while (esp_timer_get_time() <= TIME_RESET) {
            if (btnReset.checkPress() == LONG_PRESS) {
                Logger.log("! DEVICE RESET");
                preferences.putBool("init", false);
                DeviceDataHandler::reset();
                ESP.restart();
            }
        }

        // Check if already initialized
        if (preferences.getBool("init") == false) {
            Logger.log("@ INIT DATA");
            preferences.putBool("init", true);
            // Setup device data
            device->setup();
            device->errorsHandler->file = DeviceDataHandler::initData(device);
        }

        Logger.log(".:: SLEEP Device ::.");
        esp_deep_sleep_start();
    }

    preferences.end();

    // Create update data task
    xTaskCreatePinnedToCore(updateData, "updateDataTask",
                            UPDATE_DATA_TASK_WORDS, NULL,
                            UPDATE_DATA_TASK_PRIORITY, &UpdateDataTask, CORE_0);

    // Wait task to finish
    delay(100);
    while (dataTaskStatus == STATUS_ALIVE) {
    }

    device->errorsHandler->log();

    Logger.log(".:: SLEEP Device ::.");
    esp_deep_sleep_start();
}

// Loop functionn never used
void loop() {}