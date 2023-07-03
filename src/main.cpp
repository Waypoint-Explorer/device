#include "Button.h"
#include "Preferences.h"
#include "config.h"
#include "devices/Battery.h"
#include "devices/Display.h"
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
EnvData envData;
Gps gps;
TimeData timeData;
Display display;
Button btnReset(RESET_BUTTON, TIME_LONGPRESS_RESET);
Button btnUpdate(UPDATE_BUTTON);

TaskHandle_t UpdateByTimerTask;
static volatile uint8_t updateByTimerStatus = STATUS_DEAD;

TaskHandle_t UpdateByButtonTask;
static volatile uint8_t updateByButtonStatus = STATUS_DEAD;

static volatile uint8_t updateDataStatus = STATUS_DEAD;

SemaphoreHandle_t xMutex;

int64_t espTime = 0;
RTC_DATA_ATTR int64_t taskExeTime = 0;
RTC_DATA_ATTR int64_t previousEspTime = 0;
int64_t previousExeTime = 0;

RTC_DATA_ATTR int64_t timerWakeupRemaining = 0;

void printEnvData(EnvData envData);
void updateByTimer(void* parameter);
void updateByButton(void* parameter);

// Setup function
void setup() {
    Logger.begin(true);
    Logger.log(".:: WAKE UP Device ::.");

    pinMode(LED_BUILTIN, OUTPUT);
    preferences.begin("device");

    device = new Device();
    timeData = TimeData();
    xMutex = xSemaphoreCreateMutex();

    DeviceDataHandler::begin();
    gps.begin();
    device->errorsHandler->sensor = envSensor.begin();
    display = Display(DISPLAY_ROTATION);

    envData = envSensor.getEnvData();

    esp_sleep_enable_ext0_wakeup(UPDATE_BUTTON, 1);

    updateByTimerStatus = STATUS_DEAD;
    updateByButtonStatus = STATUS_DEAD;

    updateDataStatus = STATUS_DEAD;

    /////////////// INIT DEVICE IF FIRST TIME /////////////////////
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
        // First seconds wait and check for reset button pressed
        while (esp_timer_get_time() <= TIME_RESET) {
            if (btnReset.checkPress() == LONG_PRESS) {
                Logger.log("! DEVICE RESET");
                display.clear();
                display.drawString(70, 20, "-- RESET DEVICE --");
                display.paint();
                // Reset device
                preferences.putBool("init", false);
                DeviceDataHandler::reset();
                ESP.restart();
            }
        }

        display.clear();
        display.drawString(70, 5, "-- SETUP DEVICE --");
        display.paint();

        // Check if already initialized
        if (preferences.getBool("init") == false) {
            Logger.log("@ INIT DATA");

            // Setup device data
            preferences.putBool("init", true);
            device->setup();
            device->errorsHandler->file =
                DeviceDataHandler::initEnvDataJsonArray();

            // Save preferences
            preferences.putString("id", device->id);
            preferences.putBool("init", true);

            display.clear();
            display.drawString(85, 20, "-- INIT DEVICE --");
            display.drawString(65, 40, "ID: " + device->id);
            display.paint();
        }
        display.drawString(20, 75, "SENSOR CALIBRATION.......");
        display.paint();
        envSensor.calibrate(SENSOR_CALIBRATION_CYCLES);
        display.drawString(230, 75, "DONE");

        display.drawString(20, 100, "GET GPS DATA.......................");
        display.paint();
        device->errorsHandler->gps = gps.getGpsData(timeData, device->position);
        preferences.putFloat("latitude", device->position->latitude);
        preferences.putFloat("longitude", device->position->longitude);

        (device->errorsHandler->gps == 0)
            ? display.drawString(230, 100, "DONE")
            : display.drawString(230, 100, "ERROR");

        display.drawString(80, 140, "-- SETUP DONE --");
        display.paint();

        previousEspTime = timeData.getTimestampMillis();
        timerWakeupRemaining = TIME_TO_SLEEP * mS_TO_S_FACTOR;

        esp_sleep_enable_timer_wakeup(FIRST_TIME_SLEEP * uS_TO_S_FACTOR);

        Logger.log(".:: SLEEP Device ::.");
        esp_deep_sleep_start();
    }

    // Get data from Preferences
    device->id = preferences.getString("id");
    device->position->latitude = preferences.getFloat("latitude");
    device->position->longitude = preferences.getFloat("longitude");

    preferences.end();

    previousExeTime = timeData.getTimestampMillis();

    // Create update data task
    xTaskCreatePinnedToCore(
        updateByTimer, "updateByTimerTask", UPDATE_BY_TIMER_TASK_WORDS, NULL,
        UPDATE_BY_TIMER_TASK_PRIORITY, &UpdateByTimerTask, CORE_0);

    xTaskCreatePinnedToCore(
        updateByButton, "updateByButtonTask", UPDATE_BY_BUTTON_TASK_WORDS, NULL,
        UPDATE_BY_BUTTON_TASK_PRIORITY, &UpdateByButtonTask, CORE_0);

    // Wait task to finish
    delay(100);
    while (updateByButtonStatus == STATUS_ALIVE ||
           updateByTimerStatus == STATUS_ALIVE) {
        espTime = timeData.getTimestampMillis();

        /*Logger.log("STATO QR: " + String(updateByButtonStatus) + " | espTime
        (" + espTime + ") - previousTime(" + previousEspTime + ")" + " | TEMPO:
        " + (espTime - previousEspTime)); delay(100);*/

        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0 &&
            updateDataStatus == STATUS_DEAD) {
            if (timerWakeupRemaining - taskExeTime <= 0) {
                updateDataStatus = STATUS_ALIVE;
                //  Assicuro che il task veda lo status nuovo
                delay(100);
            }
        }
    }

    taskExeTime = timeData.getTimestampMillis() - previousExeTime;
    Logger.log("@ EXEC TASK TIME: " +
               String((float)taskExeTime / mS_TO_S_FACTOR));

    // new timer wakeup removing task execution time
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER ||
        updateDataStatus == STATUS_ALIVE) {
        timerWakeupRemaining = TIME_TO_SLEEP * mS_TO_S_FACTOR - taskExeTime;
    } else {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
            timerWakeupRemaining -= taskExeTime;
        }
    }

    esp_sleep_enable_timer_wakeup(timerWakeupRemaining * mS_TO_S_FACTOR);

    Logger.log("* UPDATE WAKE TIME TO: " +
               String((float)timerWakeupRemaining / mS_TO_S_FACTOR));

    device->log();

    previousEspTime = timeData.getTimestampMillis();

    Logger.log(".:: SLEEP Device ::.");
    esp_deep_sleep_start();
}

// Function called by UpdateByTimerTask to update environmental data
void updateByTimer(void* parameter) {
    Logger.log("- BORN ENV DATA");
    updateByTimerStatus = STATUS_ALIVE;
    while (1) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER ||
            updateDataStatus == STATUS_ALIVE) {
            Logger.log("- DATE/TIME UPDATE");

            device->errorsHandler->gps = gps.getGpsData(timeData);

            Logger.log("- ENV DATA UPDATE");

            DeviceDataHandler::writeLastEnvData(
                EntryData(envData, timeData.getTimestamp()), device);

            printEnvData(envData);

            Logger.log("- DEAD ENV DATA");
            updateByTimerStatus = STATUS_DEAD;
            vTaskDelete(UpdateByTimerTask);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
        if (updateByButtonStatus == STATUS_DEAD) {
            Serial.println("- DEAD ENV DATA outside");
            updateByTimerStatus = STATUS_DEAD;
            vTaskDelete(UpdateByTimerTask);
        }
    }
}

void updateByButton(void* parameter) {
    Serial.println("- BORN QR CODE");
    updateByButtonStatus = STATUS_ALIVE;
    while (1) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0 ||
            btnUpdate.checkPress() == SHORT_PRESS) {
            Serial.println("- QR CODE UPDATE: " + updateByButtonStatus);

            printEnvData(envData);

            updateByButtonStatus = STATUS_DEAD;
            Serial.println("- DEAD QR CODE inside");
            vTaskDelete(UpdateByButtonTask);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
        if (updateByTimerStatus == STATUS_DEAD) {
            Serial.println("- DEAD QR CODE outside");
            updateByButtonStatus = STATUS_DEAD;
            vTaskDelete(UpdateByButtonTask);
        }
    }
}

void printEnvData(EnvData envData) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    display.clear();

    display.drawString(5, 10, timeData.getDate());
    display.drawString(130, 10, timeData.getTime());
    display.drawString(230, 10, Battery::toString());

    display.drawString(5, 40,
                       "Temperatura: " + String(envData.temperature) + "°C");
    display.drawString(150, 40, "Umidità: " + String(envData.humidity) + "%");
    display.drawString(5, 70,
                       "Qualità aria: " + String(envData.airQuality) + "%");
    display.drawString(
        135, 70,
        "Pressione:" + String((float)envData.pressure / 10, 1) + " hPa");

    display.paint();
    xSemaphoreGive(xMutex);
}

// Loop functionn never used
void loop() {}