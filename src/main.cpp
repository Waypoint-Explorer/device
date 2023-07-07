#include "config.h"

LoggerService Logger;
Preferences preferences;

Device* device;
EnvSensor envSensor;
EnvData lastEnvData;
Gps gps;
TimeData timeData;
Display display;
Button btnReset(RESET_BUTTON, TIME_LONGPRESS_RESET);
Button btnUpdate(UPDATE_BUTTON);

TaskHandle_t UpdateByTimerTask;
static volatile uint8_t updateByTimerStatus = STATUS_DEAD;

TaskHandle_t UpdateByButtonTask;
static volatile uint8_t updateByButtonStatus = STATUS_DEAD;

SemaphoreHandle_t xMutex;

RTC_DATA_ATTR uint8_t countGpsRead = 0;

void printEnvData();
void updateByTimer(void* parameter);
void updateByButton(void* parameter);

// Setup function
void setup() {
    Logger.begin(true);
    Logger.log(".:: WAKE UP Device ::.");

    pinMode(LED_BUILTIN, OUTPUT);
    xMutex = xSemaphoreCreateMutex();

    Logger.log("• PREFERENCES");
    preferences.begin("device");

    Logger.log("• DEVICE");
    device = new Device();

    Logger.log("• TIMEDATA");
    timeData = TimeData();

    Logger.log("• DATA HANDLER");
    DeviceDataHandler::begin();

    Logger.log("• ENV SENSOR");
    device->errorsHandler->sensor = envSensor.begin();

    Logger.log("• DISPLAY");
    display = Display(DISPLAY_ROTATION);

    esp_sleep_enable_ext0_wakeup(UPDATE_BUTTON, HIGH);

    updateByTimerStatus = STATUS_DEAD;
    updateByButtonStatus = STATUS_DEAD;

    /////////////// INIT DEVICE IF FIRST TIME /////////////////////
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
        // First seconds wait and check for reset button pressed
        while (esp_timer_get_time() <= TIME_RESET) {
            if (btnReset.checkPress() == LONG_PRESS) {
                Logger.log("• DEVICE RESET");
                display.clear();
                display.drawString(70, 20, ".:: RESET DEVICE ::.");
                display.paint();
                // Reset device
                preferences.putBool("init", false);
                DeviceDataHandler::reset();
                ESP.restart();
            }
        }

        display.clear();
        display.drawString(85, 5, ".:: SETUP DEVICE ::.");
        display.paint();

        // Check if already initialized
        if (preferences.getBool("init") == false) {
            Logger.log("• INIT DATA");

            // Setup device data
            preferences.putBool("init", true);
            device->setup();
            device->errorsHandler->file =
                DeviceDataHandler::initEnvDataJsonArray();

            // Save preferences
            preferences.putString("id", device->id);
            preferences.putBool("init", true);

            display.clear();
            display.drawString(85, 20, ".:: INIT DEVICE ::.");
            display.drawString(55, 40, "ID: " + device->id);
            display.paint();
        }

        display.drawString(20, 100, "GET GPS DATA......");
        display.paint();
        Logger.log("• GPS");
        gps.begin();
        device->errorsHandler->gps = gps.getGpsData(timeData, device->position);
        preferences.putFloat("latitude", device->position->latitude);
        preferences.putFloat("longitude", device->position->longitude);

        (device->errorsHandler->gps == 0)
            ? display.drawString(230, 100, "DONE")
            : display.drawString(230, 100, "ERROR");

        display.drawString(80, 140, ".:: SETUP DONE ::.");
        display.paint();

        display.clear();
        lastEnvData = envSensor.getCalibratedEnvData(10);
        printEnvData();
        display.paint();

        esp_sleep_enable_timer_wakeup(timeData.getSleepTimeInSeconds() *
                                      uS_TO_S_FACTOR);
        Logger.log("• SLEEP REMAINING: " +
                   String(timeData.getSleepTimeInSeconds()));

        Logger.log(".:: SLEEP Device ::.");
        esp_deep_sleep_start();
    }

    // Get data from Preferences
    device->id = preferences.getString("id");
    device->position->latitude = preferences.getFloat("latitude");
    device->position->longitude = preferences.getFloat("longitude");

    preferences.end();

    preferences.begin("lastEnvData");
    lastEnvData.temperature = preferences.getInt("temperature");
    lastEnvData.humidity = preferences.getInt("humidity");
    lastEnvData.pressure = preferences.getInt("pressure");
    lastEnvData.airQuality = preferences.getInt("airQuality");
    preferences.end();

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
    }

    device->log();
    gps.modemPowerOff();

    esp_sleep_enable_timer_wakeup(timeData.getSleepTimeInSeconds() *
                                  uS_TO_S_FACTOR);
    Logger.log("• SLEEP REMAINING: " +
               String(timeData.getSleepTimeInSeconds()));

    Logger.log(".:: SLEEP Device ::.");
    esp_deep_sleep_start();
}

// Function called by UpdateByTimerTask to update environmental data every hour
void updateByTimer(void* parameter) {
    Logger.log("TASK_BY_TIMER: BORN");
    updateByTimerStatus = STATUS_ALIVE;
    while (1) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER ||
            timeData.getSleepTimeInSeconds() == 0) {
            if (countGpsRead >= GET_GPS_DATA_COUNT) {
                Logger.log("TASK_BY_TIMER: UPDATE TIME/DATE");
                gps.begin();
                device->errorsHandler->gps = gps.getGpsData(timeData);
                countGpsRead = 0;
            } else {
                countGpsRead++;
            }

            Logger.log("TASK_BY_TIMER: UPDATE ENV DATA");
            lastEnvData = envSensor.getCalibratedEnvData(10);

            preferences.begin("lastEnvData");
            preferences.putInt("temperature", lastEnvData.temperature);
            preferences.putInt("humidity", lastEnvData.humidity);
            preferences.putInt("pressure", lastEnvData.pressure);
            preferences.putInt("airQuality", lastEnvData.airQuality);
            preferences.end();

            xSemaphoreTake(xMutex, portMAX_DELAY);

            DeviceDataHandler::writeLastEnvData(
                EntryData(lastEnvData, timeData.getTimestamp()), device);

            printEnvData();
            display.paint();

            xSemaphoreGive(xMutex);

            Logger.log("TASK_BY_TIMER: DEAD executed");
            updateByTimerStatus = STATUS_DEAD;
            vTaskDelete(UpdateByTimerTask);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
        if (updateByButtonStatus == STATUS_DEAD) {
            Serial.println("TASK_BY_TIMER: DEAD");
            updateByTimerStatus = STATUS_DEAD;
            vTaskDelete(UpdateByTimerTask);
        }
    }
}

/* Function called by UpdateByButtonTask to generate QR code */
void updateByButton(void* parameter) {
    Serial.println("TASK_BY_BUTTON: BORN");
    updateByButtonStatus = STATUS_ALIVE;
    while (1) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0 ||
            btnUpdate.checkPress() == SHORT_PRESS) {
            Serial.println("TASK_BY_BUTTON: GENERATE QR");

            xSemaphoreTake(xMutex, portMAX_DELAY);
            LinkedList<EntryData>* entrydataList = new LinkedList<EntryData>;
            DeviceDataHandler::readEnvDataList(entrydataList);
            display.clear();
            printEnvData();
            uint8_t qr[qrcodegen_BUFFER_LEN_MAX];
            String qrText =
                QrCodeHandler::generateStringForQr(device, *entrydataList);
            QrCodeHandler::generateQrCode(qrText, qr);
            QrCodeHandler::displayQrCode(qr, display);
            display.paint();

            vTaskDelay(pdMS_TO_TICKS(QR_CODE_DISPLAY_TIME));

            display.clear();
            printEnvData();
            display.drawString(50, 220, "PREMI IL PULSANTE SOTTO");
            display.paint();
            xSemaphoreGive(xMutex);

            updateByButtonStatus = STATUS_DEAD;
            Logger.log("TASK_BY_BUTTON: DEAD executed");
            vTaskDelete(UpdateByButtonTask);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
        if (updateByTimerStatus == STATUS_DEAD) {
            Logger.log("TASK_BY_BUTTON: DEAD");
            updateByButtonStatus = STATUS_DEAD;
            vTaskDelete(UpdateByButtonTask);
        }
    }
}

/* Print environmental data on top of screen */
void printEnvData() {
    display.drawString(110, 10, timeData.getDate());

    // TODO: remove this
    display.drawString(250, 10, String(timeData.getSleepTimeInSeconds()) + "s");

    display.drawString(
        5, 40, "Temperatura: " + String(lastEnvData.temperature) + "°C");
    display.drawString(160, 40,
                       "Qualità aria: " + String(lastEnvData.airQuality) + "%");
    display.drawString(5, 70, "Umidità: " + String(lastEnvData.humidity) + "%");
    display.drawString(
        120, 70,
        "Pressione:" + String((float)lastEnvData.pressure / 10, 1) + " hPa");
}

// Loop functionn never used
void loop() {}