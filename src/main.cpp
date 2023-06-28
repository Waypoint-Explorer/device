#include "config.h"
#include "devices/EnvSensor.h"
#include "model/Device.h"
#include "model/EnvData.h"
#include "utility/DeviceDataHandler.h"
#include "utility/Logger.h"

EnvSensor envSensor;
LoggerService Logger;

TaskHandle_t UpdateDataTask;
Device* device;

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

    envSensor.init();

    device = new Device();

    DeviceDataHandler::init();

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
        device->setup();
        DeviceDataHandler::initData(device);
    }

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

void loop() {}