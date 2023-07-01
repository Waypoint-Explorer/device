#ifndef __ERRORS__
#define __ERRORS__

/* Errors enumerators */

// Battery error
enum BatteryError { BATTERY_OK, BATTERY_LOW };

// Initialization error
enum InitError { INIT_OK, INIT_FAIL };

// File management error
enum FileError { FILE_OK, FILE_CANT_OPEN, FILE_CANT_WRITE, FILE_CANT_READ };

// Gps data error
enum GpsError {
    GPS_OK,
    GPS_CANT_INIT,
    GPS_CANT_LOCATE,
    GPS_CANT_ENABLE,
    GPS_CANT_DISABLE
};

// QR code errors
enum QrCodeError { QR_OK, QR_CANT_CREATE };

// Sensor error
enum SensorError {
    SENSOR_OK,
    SENSOR_CANT_INIT,
    SENSOR_CANT_CONFIG,
    SENSOR_CANT_SUBSCRIBE,
    SENSOR_CANT_SET_STATE
};

#endif