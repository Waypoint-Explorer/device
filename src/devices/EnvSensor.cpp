#include "EnvSensor.h"

// Sensor configuration
const uint8_t bsecConfigIaq[] = {
#include "config/generic_33v_3s_4d/bsec_iaq.txt"
};

// List of sensor needed
bsec_virtual_sensor_t sensorList[] = {
    BSEC_OUTPUT_RAW_TEMPERATURE, BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY, BSEC_OUTPUT_IAQ, BSEC_OUTPUT_RAW_GAS};

SensorError EnvSensor::begin() {
    // Sensor begin and check error
    sensor.begin(BME68X_I2C_ADDR_HIGH, Wire);
    if (!checkSensor()) {
        Logger.log("Failed to init BME680, check wiring!");
        return SENSOR_CANT_INIT;
    }

    // Sensor version
    Logger.log("BSEC version :" + String(sensor.version.major) + "." +
               String(sensor.version.minor) + "." +
               String(sensor.version.major_bugfix) + "." +
               String(sensor.version.minor_bugfix));

    // Set sensor config and check errors
    sensor.setConfig(bsecConfigIaq);
    if (!checkSensor()) {
        Logger.log("Failed to set config!");
        return SENSOR_CANT_CONFIG;
    }

    // Update sensor subscription and check errors
    sensor.updateSubscription(sensorList,
                              sizeof(sensorList) / sizeof(sensorList[0]),
                              BSEC_SAMPLE_RATE_LP);
    if (!checkSensor()) {
        Logger.log("Failed to update subscription!");
        return SENSOR_CANT_SUBSCRIBE;
    }

    // Set sensor state
    sensor.setState(bsecState);
    if (!checkSensor()) {
        Logger.log("Failed to set state!");
        return SENSOR_CANT_SET_STATE;
    }

    return SENSOR_OK;
}

EnvData EnvSensor::getEnvData() {
    EnvData envData = EnvData();
    if (sensor.run()) {
        envData.temperature = sensor.rawTemperature;
        envData.humidity = sensor.rawHumidity - HUMIDITY_CORRECTION;
        envData.pressure = formatPressure(sensor.pressure);
        envData.airQuality = getAirQulity(
            sensor.rawHumidity - HUMIDITY_CORRECTION, sensor.gasResistance);
        envData.gasResistance = sensor.gasResistance;

        // Debug
        // envData.log();
    }
    return envData;
}

bool EnvSensor::checkSensor() {
    if (sensor.bsecStatus < BSEC_OK) {
        Logger.log("BSEC error, status: " + sensor.bsecStatus);
        return false;
        ;
    } else if (sensor.bsecStatus > BSEC_OK) {
        Logger.log("BSEC warning, status: " + sensor.bsecStatus);
    }

    if (sensor.bme68xStatus < BME68X_OK) {
        Logger.log("Sensor error, bme680_status: " + sensor.bme68xStatus);
        return false;
    } else if (sensor.bme68xStatus > BME68X_OK) {
        Logger.log("Sensor warning, status: " + sensor.bme68xStatus);
    }

    return true;
}

float EnvSensor::getHumidityScore(float humidity) {
    float humidityScore;
    float currentHumidity = humidity;
    if (currentHumidity >= 38 && currentHumidity <= 42)
        humidityScore = HUMIDITY_WEIGHT;
    else {
        if (currentHumidity < 38)
            humidityScore =
                HUMIDITY_WEIGHT / HUMIDITY_REFERENCE * currentHumidity;
        else {
            humidityScore = ((-HUMIDITY_WEIGHT / (100 - HUMIDITY_REFERENCE) *
                              currentHumidity) +
                             0.416666);
        }
    }

    // Logger.log(String(humidityScore));
    return humidityScore;
}

float EnvSensor::getGasScore(float gasResistance) {
    float gasScore;
    float gasReference = gasResistance;
    if (gasReference > GAS_UPPER_LIMIT) gasReference = GAS_UPPER_LIMIT;
    if (gasReference < GAS_LOWER_LIMIT) gasReference = GAS_LOWER_LIMIT;
    gasScore =
        (GAS_WEIGHT / (GAS_UPPER_LIMIT - GAS_LOWER_LIMIT) * gasReference -
         (GAS_LOWER_LIMIT *
          (GAS_WEIGHT / (GAS_UPPER_LIMIT - GAS_LOWER_LIMIT))));

    // Logger.log(String(gasScore));
    return gasScore;
}

int16_t EnvSensor::getAirQulity(float humidity, float gasResistance) {
    float airQualityScore =
        getHumidityScore(humidity) + getGasScore(gasResistance);
    return airQualityScore * 100;
}

int16_t EnvSensor::formatPressure(float pressure) { return (int)pressure / 10; }

EnvData EnvSensor::getCalibratedEnvData(int16_t cycles) {
    Logger.log("START CALIBRATION!");
    int16_t count = 0;
    while (count < cycles) {
        count++;
        getEnvData();
        delay(CALIBRATION_DELAY_MILLIS);
    }
    Logger.log("CALIBRATED!");
    return getEnvData();
}