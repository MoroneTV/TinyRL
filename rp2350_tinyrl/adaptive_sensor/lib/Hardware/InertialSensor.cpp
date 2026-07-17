#include "InertialSensor.h"

bool InertialSensor::begin()
{   
    Serial.println("BEGIN");
    if (!lsm.begin())
    {
        D_println("Inertia sensor failure!");
        return false;
    }

    Serial.println("RESET");
    // Reset to default parameters
    lsm.deviceReset();
    while (!lsm.getDeviceReset())
        delay(1);

    Serial.println("CONFIG");
    // Accelerometer and Gyroscope registers will not be updated
    // until read.
    lsm.enableBlockDataUpdate();

    // Set the output scale
    lsm.setAccelFullScale(LSM6DSV16X_4g);
    lsm.setGyroFullScale(LSM6DSV16X_2000dps);

    // Apply filtering to outputs
    lsm.enableFilterSettling();
    lsm.enableAccelLP2Filter();
    lsm.setAccelLP2Bandwidth(LSM6DSV16X_XL_STRONG);
    lsm.enableGyroLP1Filter();
    lsm.setGyroLP1Bandwidth(LSM6DSV16X_GY_ULTRA_LIGHT);

    Serial.println("CALIBRATE");
    // Calculate sensor offsets and set ODR
    updatePowerMode(getPowerMode());
    calibrate();
    
    D_println("Inertia Setup Complete!");
    setStatus(true);
    return true;
}

void InertialSensor::update(int now)
{

    // Add a reading to the buffer if enough time has elapsed
    if (now - lastSampleTime > sampleInterval)
    {
        sfe_lsm_data_t accelData;
        sfe_lsm_data_t gyroData;

        lsm.getAccel(&accelData);
        lsm.getGyro(&gyroData);

        buffer[bPtr] = InertiaReading {
            // The library reads in milidegrees per second, so we convert to degrees per second.
            (accelData.xData - aXoffset) / 100.0f,
            (accelData.yData - aYoffset) / 100.0f,
            (accelData.zData - aZoffset) / 100.0f,
            (gyroData.xData - gXoffset) / 100.0f, 
            (gyroData.yData - gYoffset) / 100.0f,
            (gyroData.zData - gZoffset) / 100.0f,
        };

        bPtr++;
        lastSampleTime = now;
    }
}

void InertialSensor::calibrate()
{
    D_println("Starting Inertia Calibration");

    InertiaPowerLevel originalPowerMode = getPowerMode();
    updatePowerMode(HZ_240); // Max res just for this period
    delay(1000);             // Wait for the sensor to stabilize

    // Collect 100 readings at slight increment to get average offsets
    float avgAx = 0.0f;
    float avgAy = 0.0f;
    float avgAz = 0.0f;
    float avgGx = 0.0f;
    float avgGy = 0.0f;
    float avgGz = 0.0f;

    for (int i = 0; i < 100; i++)
    {
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

    // Store offsets
    aXoffset = avgAx / 100.0f;
    aYoffset = avgAy / 100.0f;
    aZoffset = avgAz / 100.0f;
    gXoffset = avgGx / 100.0f;
    gYoffset = avgGy / 100.0f;
    gZoffset = avgGz / 100.0f;

    updatePowerMode(originalPowerMode); // Revert to whatever power mode we started with

    D_println("Inertia Calibration Complete!");
}

void InertialSensor::updatePowerMode(InertiaPowerLevel lvl)
{
    // Different ODR rates per level, Gyro off on the lowest mode.
    switch (lvl)
    {
    case HZ_7p5:
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_OFF);
        break;

    case HZ_15:
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_15Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_15Hz);
        break;

    case HZ_60:
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_60Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_60Hz);
        break;

    case HZ_120:
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_120Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_120Hz);
        break;

    default:
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_240Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_240Hz);
        break;
    }
    // Save powermode and update sample interval
    powerMode = lvl;
    updateSampleInterval(lvl);

    D_println("Inertia ODR set to: " + String(powerMode));
}