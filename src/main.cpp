#include "config.h"

LoggerService Logger;
Device* device;
EnvSensor envSensor;
Gps gps;
TimeDataHandler timeDataHandler;
Display display;

Button btnReset(RESET_BUTTON, TIME_LONGPRESS_RESET);
Button btnUpdate(UPDATE_BUTTON);

TaskHandle_t CalibrateSensorTask;
static volatile uint8_t calibrateSensorStatus = STATUS_DEAD;

TaskHandle_t UpdateByTimerTask;
static volatile uint8_t updateByTimerStatus = STATUS_DEAD;

TaskHandle_t UpdateByButtonTask;
static volatile uint8_t updateByButtonStatus = STATUS_DEAD;

SemaphoreHandle_t xMutex;

RTC_DATA_ATTR uint8_t countGpsRead = 0;
RTC_DATA_ATTR bool firstTimeGps = false;

RTC_DATA_ATTR struct EnvDataStruct lastEnvDataRTC;
RTC_DATA_ATTR struct ErrorsStruct errorsRTC;

void calibrateSensor(void* parameter);
void updateByTimer(void* parameter);
void updateByButton(void* parameter);
void printEnvData();
void printQrBackground();
void printDataAndQr();
void storeAndSpleep();

// Setup function
void setup() {
    Logger.begin(true);
    Logger.log(".:: WAKE UP Device ::.");

    pinMode(LED_BUILTIN, OUTPUT);

    xMutex = xSemaphoreCreateMutex();

    Logger.log("• DEVICE");
    device = new Device();

    Logger.log("• RETRIEVE DATA FROM RTC");  // Get data from RTC
    device->retrieveErrorsFromRTC(&errorsRTC);
    device->retrieveEnvDataFromRTC(&lastEnvDataRTC);

    Logger.log("• TIMEDATA");
    timeDataHandler = TimeDataHandler();

    Logger.log("• DATA HANDLER");
    DeviceDataHandler::begin();

    Logger.log("• ENV SENSOR");
    device->errorsData->sensor = envSensor.begin();

    Logger.log("• DISPLAY");
    display = Display(DISPLAY_ROTATION);

    float batteryLevel = 0;
    device->errorsData->battery = Battery::getBatteryLevel(&batteryLevel);
    Logger.log("• BATTERY : " + String((float)batteryLevel));

    esp_sleep_enable_ext0_wakeup(UPDATE_BUTTON, HIGH);

    calibrateSensorStatus = STATUS_DEAD;
    updateByTimerStatus = STATUS_DEAD;
    updateByButtonStatus = STATUS_DEAD;

    /////////////// INIT DEVICE IF FIRST TIME /////////////////////
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
        // First seconds wait and check for reset button pressed
        while (esp_timer_get_time() <= TIME_RESET) {
            if (btnReset.checkPress() == LONG_PRESS) {
                Logger.log("• DEVICE RESET");
                display.clear();
                display.drawStringHCentered(5, RESET_STRING, Cousine_Bold_21);
                display.paint();
                // Reset device
                device->init = false;
                device->storeInitToPreferences();
                DeviceDataHandler::reset();

                device->endPreferences();
                ESP.restart();
            }
        }

        // Check if already initialized
        device->retrieveInitFromPreferences();
        if (device->init == false) {
            Logger.log("• INIT DATA");

            // Setup device data

            device->setup();
            device->storeIdToPreferences();
            device->errorsData->file =
                DeviceDataHandler::initEntryDataJsonArray();

            display.clear();
            display.drawStringHCentered(5, INIT_STRING, Cousine_Bold_21);
            display.drawStringHCentered(40, ID_STRING + device->id,
                                        Cousine_Bold_16);
            // display.paint();
            device->init = true;
            device->storeInitToPreferences();

        } else {
            display.clear();
            display.drawStringHCentered(5, SETUP_STRING, Cousine_Bold_21);
            // display.paint();
        }

        display.drawString(20, 70, GET_GPS_STRING, Cousine_Bold_18);
        display.paint();

        // Create calibrate sensor task
        xTaskCreatePinnedToCore(
            calibrateSensor, "calibrateSensorTask", CALIBRATE_SENSOR_TASK_WORDS,
            NULL, CALIBRATE_SENSOR_TASK_PRIORITY, &CalibrateSensorTask, CORE_0);

        Logger.log("• GPS");
        gps.begin();
        device->errorsData->gps =
            gps.getGpsData(timeDataHandler, device->position);

        if (device->errorsData->gps == GPS_OK) {
            display.drawString(230, 70, DONE_GPS_STRING, Cousine_Bold_18);
            display.drawStringHCentered(
                90, LAT_STRING + String((float)device->position->latitude, 6),
                Cousine_Bold_16);
            display.drawStringHCentered(
                110, LON_STRING + String((float)device->position->longitude, 6),
                Cousine_Bold_16);
            device->storePositionToPreferences();
            firstTimeGps = true;
        } else {
            display.drawString(210, 70, ERROR_GPS_STRING, Cousine_Bold_18);
            firstTimeGps = false;
        }

        display.paint();

        if(device->errorsData->gps == GPS_OK) delay(INIT_DATA_DISPLAY_TIME);

        // Wait task to finish
        delay(100);
        while (calibrateSensorStatus == STATUS_ALIVE) {
            delay(50);
        }

        display.drawStringHCentered(140, DONE_SETUP_STRING, Cousine_Bold_21);
        display.paint();

        printDataAndQr();

        storeAndSpleep();
    }

    // Get data from Preferences
    device->retrieveIdFromPreferences();
    device->retrievePositionFromPreferences();

    // Create update data task
    xTaskCreatePinnedToCore(
        updateByTimer, "updateByTimerTask", UPDATE_BY_TIMER_TASK_WORDS, NULL,
        UPDATE_BY_TIMER_TASK_PRIORITY, &UpdateByTimerTask, CORE_1);

    xTaskCreatePinnedToCore(
        updateByButton, "updateByButtonTask", UPDATE_BY_BUTTON_TASK_WORDS, NULL,
        UPDATE_BY_BUTTON_TASK_PRIORITY, &UpdateByButtonTask, CORE_0);

    // Wait task to finish
    delay(100);
    while (updateByButtonStatus == STATUS_ALIVE ||
           updateByTimerStatus == STATUS_ALIVE) {
        delay(50);
    }

    gps.modemPowerOff();
    device->endPreferences();

    storeAndSpleep();
}

/* Function called by CalibrateSensorTask to calibrate sensor */
void calibrateSensor(void* parameter) {
    Logger.log("TASK_CALIBRATION_SENSOR: BORN");
    calibrateSensorStatus = STATUS_ALIVE;

    *device->lastEnvData =
        envSensor.getCalibratedEnvData(SENSOR_CALIBRATION_CYCLES);

    delay(50);
    Logger.log("TASK_CALIBRATION_SENSOR: DEAD");
    calibrateSensorStatus = STATUS_DEAD;
    vTaskDelete(CalibrateSensorTask);
}

/* Function called by UpdateByTimerTask to update environmental data hourly */
void updateByTimer(void* parameter) {
    Logger.log("TASK_BY_TIMER: BORN");
    updateByTimerStatus = STATUS_ALIVE;
    while (1) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER ||
            timeDataHandler.getSleepTimeInSeconds() <= 0) {
            // Create calibrate sensor task
            xTaskCreatePinnedToCore(calibrateSensor, "calibrateSensorTask",
                                    CALIBRATE_SENSOR_TASK_WORDS, NULL,
                                    CALIBRATE_SENSOR_TASK_PRIORITY,
                                    &CalibrateSensorTask, CORE_1);

            // get gps data every day to update time and date
            if (device->errorsData->gps != GPS_OK ||
                countGpsRead >= GET_GPS_DATA_COUNT) {
                Logger.log("TASK_BY_TIMER: UPDATE TIME/DATE (" +
                           timeDataHandler.getTime() + ")");
                gps.begin();
                device->errorsData->gps = gps.getGpsData(timeDataHandler);
                countGpsRead = 0;
            } else {
                countGpsRead++;
            }

            if (device->errorsData->gps == GPS_OK) firstTimeGps = true;

            // Wait task to finish
            delay(100);
            while (calibrateSensorStatus == STATUS_ALIVE) {
                delay(50);
            }

            xSemaphoreTake(xMutex, portMAX_DELAY);

            device->storeLastEnvDataToRTC(&lastEnvDataRTC);

            // At least first time gps must be ok to store data
            if (firstTimeGps == true && device->errorsData->file == FILE_OK &&
                device->errorsData->sensor == SENSOR_OK) {
                device->errorsData->file =
                    DeviceDataHandler::writeLastEntryData(
                        EntryData(*device->lastEnvData,
                                  timeDataHandler.getTimestamp()),
                        device);
            }

            printDataAndQr();

            xSemaphoreGive(xMutex);

            Logger.log("TASK_BY_TIMER: DEAD executed");
            updateByTimerStatus = STATUS_DEAD;
            vTaskDelete(UpdateByTimerTask);
        }

        delay(50);
        if (updateByButtonStatus == STATUS_DEAD) {
            Logger.log("TASK_BY_TIMER: DEAD");
            updateByTimerStatus = STATUS_DEAD;
            vTaskDelete(UpdateByTimerTask);
        }
    }
}

/* Function called by UpdateByButtonTask to generate QR code */
void updateByButton(void* parameter) {
    Logger.log("TASK_BY_BUTTON: BORN");
    updateByButtonStatus = STATUS_ALIVE;
    while (1) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0 ||
            btnUpdate.checkPress() == SHORT_PRESS) {
            Logger.log("TASK_BY_BUTTON: GENERATE QR");

            xSemaphoreTake(xMutex, portMAX_DELAY);
            LinkedList<EntryData>* entrydataList = new LinkedList<EntryData>;
            device->errorsData->file =
                DeviceDataHandler::readEntryDataList(entrydataList);

            display.clear();
            printEnvData();
            uint8_t qr[qrcodegen_BUFFER_LEN_MAX];
            String qrText =
                QrCodeHandler::generateStringForQr(device, *entrydataList);
            device->errorsData->qrCode =
                QrCodeHandler::generateQrCode(qrText, qr);
            QrCodeHandler::displayQrCode(qr, display, 5, 105);
            display.paint();

            delay(QR_CODE_DISPLAY_TIME);

            printDataAndQr();

            xSemaphoreGive(xMutex);

            updateByButtonStatus = STATUS_DEAD;
            Logger.log("TASK_BY_BUTTON: DEAD executed");
            vTaskDelete(UpdateByButtonTask);
        }
        delay(50);
        if (updateByTimerStatus == STATUS_DEAD) {
            Logger.log("TASK_BY_BUTTON: DEAD");
            updateByButtonStatus = STATUS_DEAD;
            vTaskDelete(UpdateByButtonTask);
        }
    }
}

/* Print environmental data with frame on top of screen */
void printEnvData() {
    display.drawXBitmap(0, 0, ENV_DATA_BG_WIDTH, ENV_DATA_BG_HEIGHT,
                        envDataBackground);

    (device->errorsData->gps == GPS_OK)
        ? display.drawStringHCentered(0, timeDataHandler.getDate(),
                                      Cousine_Bold_16)
        : display.drawStringHCentered(0, DATA_STRING, Cousine_Bold_16);

    display.drawString(12, 30,
                       TEMPERATURE_STRING +
                           String(device->lastEnvData->temperature) +
                           TEMPERATURE_SYMBOL_STRING);
    display.drawString(167, 30,
                       AIR_QUALITY_STRING +
                           String(device->lastEnvData->airQuality) +
                           PERCENTAGE_SYMBOL_STRING);
    display.drawString(12, 60,
                       HUMIDITY_STRING + String(device->lastEnvData->humidity) +
                           PERCENTAGE_SYMBOL_STRING);
    display.drawString(
        130, 60,
        PRESSURE_STRING + String((float)device->lastEnvData->pressure / 10, 1) +
            PRESSURE_SYMBOL_STRING);
}

/* Print qr code frame */
void printQrBackground() {
    display.drawXBitmap(0, 100, QR_BG_WIDTH, QR_BG_HEIGHT, qrBackground);
    display.drawStringHCentered(200, PRESS_BUTTON_STRING, Cousine_Bold_16);
    display.drawStringHCentered(220, GENERATE_STRING, Cousine_Bold_16);
    display.drawStringHCentered(240, QR_CODE_STRING, Cousine_Bold_16);
    display.drawXBitmap((SCREEN_WIDTH - ARROW_DOWN_WIDTH) / 2, 300,
                        ARROW_DOWN_WIDTH, ARROW_DOWN_HEIGHT, arrowDown);
}

/* Print base interface when sleeping (environmental data and qr background)*/
void printDataAndQr() {
    display.clear();
    printEnvData();
    printQrBackground();
    display.paint();
}

/* Store data to RTC and Sleep time */
void storeAndSpleep() {
    delay(100);

    // Store data to RTC
    Logger.log("• SAVE ERR TO RTC: " + device->errorsData->toString());
    Logger.log("• SAVE DATA TO RTC: " + device->lastEnvData->toString());
    device->storeErrorsToRTC(&errorsRTC);
    device->storeLastEnvDataToRTC(&lastEnvDataRTC);

    Logger.log("• SLEEP REMAINING: " +
               String(timeDataHandler.getSleepTimeInSeconds()));
    esp_sleep_enable_timer_wakeup(timeDataHandler.getSleepTimeInSeconds() *
                                  uS_TO_S_FACTOR);

    Logger.log(".:: SLEEP Device ::.");
    esp_deep_sleep_start();
}

// Loop function never used
void loop() {}