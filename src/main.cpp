#include "Button.h"
#include "Preferences.h"
#include "config.h"
#include "devices/EnvSensor.h"
#include "model/Device.h"
#include "model/EnvData.h"
#include "utility/DeviceDataHandler.h"
#include "utility/Logger.h"

EnvSensor envSensor;
LoggerService Logger;
Preferences preferences;

TaskHandle_t UpdateDataTask;
Device* device;

Button btnReset(RESET_BUTTON, TIME_LONGPRESS_RESET);

static volatile uint8_t dataTaskStatus = STATUS_DEAD;

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

void setup() {
    Logger.init(true);
    Logger.log(".:: SETUP Device ::.");

    pinMode(LED_BUILTIN, OUTPUT);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);

    preferences.begin("device");
    device = new Device();
    envSensor.init();

    DeviceDataHandler::init();

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
        // First seconds check for reset
        while (esp_timer_get_time() <= TIME_RESET) {
            if (btnReset.checkPress() == LONG_PRESS) {
                Logger.log("! DEVICE RESET");
                preferences.putBool("init", false);
                DeviceDataHandler::reset();
                ESP.restart();
            }
        }

        // Init data
        if (preferences.getBool("init") == false) {
            Logger.log("@ INIT DATA");
            preferences.putBool("init", true);
            DeviceDataHandler::initData(device);
        }

        Logger.log(".:: SLEEP Device ::.");
        esp_deep_sleep_start();
    }

    preferences.end();

    // Create update data task
    xTaskCreatePinnedToCore(updateData, "updateDataTask", 10000, NULL, 10,
                            &UpdateDataTask, 0);

    // Wait task to finish
    delay(100);
    while (dataTaskStatus == STATUS_ALIVE) {
    }

    Logger.log(".:: SLEEP Device ::.");
    esp_deep_sleep_start();
}

// Never used
void loop() {}