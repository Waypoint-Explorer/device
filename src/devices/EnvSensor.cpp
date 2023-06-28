#include "EnvSensor.h"

const uint8_t bsec_config_iaq[] = {
#include "config/generic_33v_3s_4d/bsec_iaq.txt"
};

// List of sensor needed
bsec_virtual_sensor_t sensor_list[] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_IAQ,
};

bool EnvSensor::init() {
    // Sensor begin and check error
    sensor.begin(BME68X_I2C_ADDR_HIGH, Wire);
    if (!checkSensor()) {
        Logger.log("Failed to init BME680, check wiring!");
        return false;
    }

    // Sensor version
    Logger.log("BSEC version :" + String(sensor.version.major) + "." +
               String(sensor.version.minor) + "." +
               String(sensor.version.major_bugfix) + "." +
               String(sensor.version.minor_bugfix));

    // Set sensor config and check errors
    sensor.setConfig(bsec_config_iaq);
    if (!checkSensor()) {
        Logger.log("Failed to set config!");
        return false;
    }

    // Update sensor subscription and check errors
    sensor.updateSubscription(sensor_list,
                              sizeof(sensor_list) / sizeof(sensor_list[0]),
                              BSEC_SAMPLE_RATE_LP);
    if (!checkSensor()) {
        Logger.log("Failed to update subscription!");
        return false;
    }

    sensor.setState(bsecState);
      if (!checkSensor()) {
        Logger.log("Failed to set state!");
        return false;
    }

    return true;
}

EnvData EnvSensor::getEnvData() {
    EnvData envData = EnvData();
    if (sensor.run()) {
        envData.temperature = sensor.rawTemperature;
        envData.humidity = sensor.rawHumidity;
        envData.pressure = sensor.pressure;
        envData.airQuality = sensor.iaq;

        Logger.log(envData.toString() +
                   " | iaqAccuracy: " + String(sensor.iaqAccuracy));
    }
    return envData;
}

// Check sensor for errors
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