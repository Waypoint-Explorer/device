#include "config.h"

LoggerService Logger;
Device* device;
EnvSensor envSensor;
Gps gps;
TimeDataHandler timeDataHandler;
Display display;

Button btnReset(RESET_BUTTON, TIME_LONGPRESS_RESET);
Button btnUpdate(UPDATE_BUTTON);

TaskHandle_t UpdateByTimerTask;
static volatile uint8_t updateByTimerStatus = STATUS_DEAD;

TaskHandle_t UpdateByButtonTask;
static volatile uint8_t updateByButtonStatus = STATUS_DEAD;

SemaphoreHandle_t xMutex;

RTC_DATA_ATTR uint8_t countGpsRead = 0;

RTC_DATA_ATTR struct EnvDataStruct lastEnvDataRTC;
RTC_DATA_ATTR struct ErrorsStruct errorsRTC;

void updateByTimer(void* parameter);
void updateByButton(void* parameter);
void printEnvData();
void printQrBackground();

// Setup function
void setup() {
    Logger.begin(true);
    Logger.log(".:: WAKE UP Device ::.");

    pinMode(LED_BUILTIN, OUTPUT);
    xMutex = xSemaphoreCreateMutex();

    Logger.log("• DEVICE");
    device = new Device();

    Logger.log("• TIMEDATA");
    timeDataHandler = TimeDataHandler();

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
                display.drawStringHCentered(20, ".:: RESET DEVICE ::.",
                                            Cousine_Bold_21);
                display.paint();
                // Reset device
                device->init = false;
                device->storeInitToPreferences();
                DeviceDataHandler::reset();

                device->endPreferences();
                ESP.restart();
            }
        }

        display.clear();
        display.drawStringHCentered(5, ".:: SETUP DEVICE ::.", Cousine_Bold_21);
        display.paint();

        // Check if already initialized
        device->retrieveInitFromPreferences();
        if (device->init == false) {
            Logger.log("• INIT DATA");

            // Setup device data
            device->init = true;
            device->storeInitToPreferences();
            device->setup();
            device->storeIdToPreferences();
            device->errorsHandler->file =
                DeviceDataHandler::initEnvDataJsonArray();

            display.clear();
            display.drawStringHCentered(20, ".:: INIT DEVICE ::.",
                                        Cousine_Bold_21);
            display.drawStringHCentered(40, "ID: " + device->id,
                                        Cousine_Bold_16);
            display.paint();
        }

        display.drawString(20, 100, "GET GPS DATA.....", Cousine_Bold_18);
        display.paint();
        Logger.log("• GPS");
        gps.begin();
        device->errorsHandler->gps = gps.getGpsData(timeDataHandler, device->position);

        (device->errorsHandler->gps == GPS_OK)
            ? display.drawString(230, 100, "DONE", Cousine_Bold_18)
            : display.drawString(230, 100, "ERROR", Cousine_Bold_18);

        device->storePositionToPreferences();

        display.drawStringHCentered(140, ".:: SETUP DONE ::.", Cousine_Bold_21);
        display.paint();

        display.clear();
        *device->lastEnvData = envSensor.getCalibratedEnvData(10);
        printEnvData();
        printQrBackground();
        display.paint();

        delay(100);

        Logger.log("• SAVE TO RTC: " + device->errorsHandler->toString());
        device->storeErrorsToRTC(&errorsRTC);
        device->storeLastEnvDataToRTC(&lastEnvDataRTC);

        esp_sleep_enable_timer_wakeup(timeDataHandler.getSleepTimeInSeconds() *
                                      uS_TO_S_FACTOR);
        Logger.log("• SLEEP REMAINING: " +
                   String(timeDataHandler.getSleepTimeInSeconds()));

        device->endPreferences();

        Logger.log(".:: SLEEP Device ::.");
        esp_deep_sleep_start();
    }

    // Get data from Preferences
    device->retrieveIdFromPreferences();
    device->retrievePositionFromPreferences();

    device->retrieveErrorsFromRTC(&errorsRTC);
    device->retrieveEnvDataFromRTC(&lastEnvDataRTC);

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

    device->storeErrorsToRTC(&errorsRTC);
    device->storeLastEnvDataToRTC(&lastEnvDataRTC);

    esp_sleep_enable_timer_wakeup(timeDataHandler.getSleepTimeInSeconds() *
                                  uS_TO_S_FACTOR);
    Logger.log("• SLEEP REMAINING: " +
               String(timeDataHandler.getSleepTimeInSeconds()));

    gps.modemPowerOff();
    device->endPreferences();

    Logger.log(".:: SLEEP Device ::.");
    esp_deep_sleep_start();
}

// Function called by UpdateByTimerTask to update environmental data every hour
void updateByTimer(void* parameter) {
    Logger.log("TASK_BY_TIMER: BORN");
    updateByTimerStatus = STATUS_ALIVE;
    while (1) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER ||
            timeDataHandler.getSleepTimeInSeconds() == 0) {
            // get gps data every day to update time and date
            if (device->errorsHandler->gps != GPS_OK) {
                device->errorsHandler->gps =
                    gps.getGpsData(timeDataHandler, device->position);
                countGpsRead = 0;
            } else {
                if (countGpsRead >= GET_GPS_DATA_COUNT) {
                    Logger.log("TASK_BY_TIMER: UPDATE TIME/DATE");
                    gps.begin();
                    device->errorsHandler->gps = gps.getGpsData(timeDataHandler);
                    countGpsRead = 0;
                } else {
                    countGpsRead++;
                }
            }

            Logger.log("TASK_BY_TIMER: UPDATE ENV DATA");
            *device->lastEnvData = envSensor.getCalibratedEnvData(10);

            xSemaphoreTake(xMutex, portMAX_DELAY);

            device->storeLastEnvDataToRTC(&lastEnvDataRTC);

            device->errorsHandler->file = DeviceDataHandler::writeLastEnvData(
                EntryData(*device->lastEnvData, timeDataHandler.getTimestamp()),
                device);

            display.clear();
            printEnvData();
            printQrBackground();
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
            device->errorsHandler->file =
                DeviceDataHandler::readEnvDataList(entrydataList);
            display.clear();
            printEnvData();
            uint8_t qr[qrcodegen_BUFFER_LEN_MAX];
            String qrText =
                QrCodeHandler::generateStringForQr(device, *entrydataList);
            device->errorsHandler->qrCode =
                QrCodeHandler::generateQrCode(qrText, qr);
            QrCodeHandler::displayQrCode(qr, display, 5, 105);
            display.paint();

            vTaskDelay(pdMS_TO_TICKS(QR_CODE_DISPLAY_TIME));

            display.clear();
            printEnvData();
            printQrBackground();
            display.paint();

            device->errorsHandler = new ErrorsHandler();
            device->storeErrorsToRTC(&errorsRTC);

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
    display.drawXBitmap(0, 0, ENV_DATA_BG_WIDTH, ENV_DATA_BG_HEIGHT,
                        envDataBackground);

    display.drawString(110, 0, timeDataHandler.getDate());

    display.drawString(
        12, 30,
        "Temperatura: " + String(device->lastEnvData->temperature) + "°C");
    display.drawString(
        167, 30,
        "Qualità aria: " + String(device->lastEnvData->airQuality) + "%");
    display.drawString(
        12, 60, "Umidità: " + String(device->lastEnvData->humidity) + "%");
    display.drawString(
        130, 60,
        "Pressione:" + String((float)device->lastEnvData->pressure / 10, 1) +
            " hPa");
}

void printQrBackground() {
    display.drawXBitmap(0, 100, QR_BG_WIDTH, QR_BG_HEIGHT, qrBackground);
    display.drawStringHCentered(200, "PREMI IL PULSANTE", Cousine_Bold_16);
    display.drawStringHCentered(220, "PER GENERARE", Cousine_Bold_16);
    display.drawStringHCentered(240, "IL CODICE QR", Cousine_Bold_16);
}

// Loop function never used
void loop() {}