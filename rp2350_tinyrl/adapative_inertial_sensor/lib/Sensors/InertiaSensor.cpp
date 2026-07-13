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

    Serial.println("Ready.");
    return true;
}

InertiaReadings InertiaSensor :: read() {

    sfe_lsm_data_t accelData;
    sfe_lsm_data_t gyroData;

    lsm.getAccel(&accelData);
    lsm.getGyro(&gyroData);

    return InertiaReadings 
    {
        accelData.xData,
        accelData.yData,
        accelData.zData,
        
        gyroData.xData,
        gyroData.yData,
        gyroData.zData,
    };
}

void InertiaSensor :: updatePrecision(int lvl) {

    switch (lvl)
    {
        case 0:
            lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
            lsm.setAccelFullScale(LSM6DSV16X_4g);

            lsm.setGyroDataRate(LSM6DSV16X_ODR_OFF);
            break;


        case 1:
            lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_60Hz);
            lsm.setAccelFullScale(LSM6DSV16X_4g);

            lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_60Hz);
            lsm.setGyroFullScale(LSM6DSV16X_500dps);
            break;


        case 2:
            lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_480Hz);
            lsm.setAccelFullScale(LSM6DSV16X_16g);

            lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_480Hz);
            lsm.setGyroFullScale(LSM6DSV16X_2000dps);
            break;
    
    default:
        break;
    }

}
