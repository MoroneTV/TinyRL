#include "InertiaSensor.h"

InertiaSensor::InertiaSensor() {
}

bool InertiaSensor :: begin () {
    
    if (!lsm.begin()) return false;

    // Reset to default parameters
    lsm.deviceReset();
    while (!lsm.getDeviceReset()) delay(1);
    
    // Accelerometer and Gyroscope registers will not be updated
    // until read.
    lsm.enableBlockDataUpdate();

    // Set the output data rate and precision of the accelerometer
    lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
    lsm.setAccelFullScale(LSM6DSV16X_16g);

    // Set the output data rate and precision of the gyroscope
    lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_15Hz);
    lsm.setGyroFullScale(LSM6DSV16X_2000dps);

    // Enable filter settling.
    lsm.enableFilterSettling();

    // Turn on the accelerometer's filter and apply settings.
    lsm.enableAccelLP2Filter();
    lsm.setAccelLP2Bandwidth(LSM6DSV16X_XL_STRONG);

    // Turn on the gyroscope's filter and apply settings.
    lsm.enableGyroLP1Filter();
    lsm.setGyroLP1Bandwidth(LSM6DSV16X_GY_ULTRA_LIGHT);

    calibrate();
    updatePrecision(2); // Max quality and power consumption

    Serial.println("Ready.");

    return true;
}

void InertiaSensor :: calibrate() {

    delay(1000); // Wait for the sensor to stabilize

    float avgAx = 0.0f;
    float avgAy = 0.0f;
    float avgAz = 0.0f;
    float avgGx = 0.0f;
    float avgGy = 0.0f;
    float avgGz = 0.0f;

    for (int i = 0; i < 100; i++) {
        sfe_lsm_data_t accelData;
        sfe_lsm_data_t gyroData;

        lsm.getAccel(&accelData);
        lsm.getGyro(&gyroData);

        avgAx += accelData.xData;
        avgAy += accelData.yData;
        avgAz += accelData.zData;

        avgGx += gyroData.xData;
        avgGy += gyroData.yData;
        avgGz += gyroData.zData;

        delay(10);
    }

    offsetAx = avgAx / 100.0f;
    offsetAy = avgAy / 100.0f;
    offsetAz = avgAz / 100.0f;

    offsetGx = avgGx / 100.0f;
    offsetGy = avgGy / 100.0f;
    offsetGz = avgGz / 100.0f;

}

InertiaReadings InertiaSensor :: read() {

    sfe_lsm_data_t accelData;
    sfe_lsm_data_t gyroData;

    lsm.getAccel(&accelData);
    lsm.getGyro(&gyroData);


    return InertiaReadings 
    {
        (accelData.xData - offsetAx) / 100.0f,
        (accelData.yData - offsetAy) / 100.0f,
        (accelData.zData - offsetAz) / 100.0f,

        (gyroData.xData - offsetGx) / 100.0f,   // The library reads in milidegrees per second, so we convert to degrees per second.
        (gyroData.yData - offsetGy) / 100.0f,
        (gyroData.zData - offsetGz) / 100.0f,
    };

    
}

int InertiaSensor :: updatePrecision(int lvl) {

    switch (lvl)
    {
        case 0:
            lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
            lsm.setAccelFullScale(LSM6DSV16X_4g);
            lsm.setGyroDataRate(LSM6DSV16X_ODR_OFF);
            precisionLvl = 0;
            break;

        case 1:
            lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_60Hz);
            lsm.setGyroDataRate(LSM6DSV16X_ODR_OFF);
            precisionLvl = 1;
            break;

        case 2:
            lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_240Hz);
            lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_240Hz);
            precisionLvl = 2;
            break;

        default:
            return -1;
    }

    return precisionLvl;
}

int InertiaSensor :: getTimeBetweenReadings() {

    switch (precisionLvl)
    {
        case 0:
            return 133; // 7.5Hz
            break;

        case 1:
            return 16; // 60Hz
            break;

        case 2:
            return 4; // 240Hz
            break;
    
    default:
        return -1;
        break;
    }

}