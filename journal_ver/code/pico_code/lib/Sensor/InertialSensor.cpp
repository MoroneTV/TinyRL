#include "InertialSensor.h"

bool InertialSensor::initialise()
{

    if (!lsm.begin())
    {
        Serial.println("Inertia sensor failure!");
        return false;
    }

    if (sensorWeight == 0)
    {
        Serial.println("Inertia sensor weight is 0, will not activate");
        return false;
    }

    // Reset to default parameters
    lsm.deviceReset();
    while (!lsm.getDeviceReset())
        delay(1);

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

    // Calculate sensor offsets and set ODR
    absoluteUpdate(0);
    calibrate();

    Serial.println("Inertia Setup Complete!");
    isEnabled = true;
    lastSampleTime = millis();
    return true;
}

void InertialSensor::update(unsigned long now)
{

    int sampleInterval = std::get<3>(powerSettings);

    // Add a reading to the buffer if enough time has elapsed
    if (now - lastSampleTime > sampleInterval)
    {
        sfe_lsm_data_t accelData;
        sfe_lsm_data_t gyroData;

        lsm.getAccel(&accelData);
        lsm.getGyro(&gyroData);

        // The library reads in milidegrees per second, so we convert to degrees per second.
        InertiaSensorReadings r;

        r.accelX = (accelData.xData - aXoffset) / 100.0f;
        r.accelY = (accelData.yData - aYoffset) / 100.0f;
        r.accelZ = (accelData.zData - aZoffset) / 100.0f;
        r.accelMag = sqrtf(r.accelX * r.accelX + r.accelY * r.accelY + r.accelZ * r.accelZ);

        // Only fill in gyro values if gyroscope is actually enabled.
        r.gyroEnabled = isGyroEnabled;
        if (isGyroEnabled)
        {
            r.gyroX = (gyroData.xData - gXoffset) / 100.0f;
            r.gyroY = (gyroData.yData - gYoffset) / 100.0f;
            r.gyroZ = (gyroData.zData - gZoffset) / 100.0f;
            r.gyroMag = sqrtf(r.gyroX * r.gyroX + r.gyroY * r.gyroY + r.gyroZ * r.gyroZ);
        }

        r.timestamp = now;

        readingBuffer.addReading(r);
        lastSampleTime = now;
    }
}

bool InertialSensor::relativeUpdate(Direction dir)
{
    // Increment or decrement current lvl by one
    int lvl = std::get<1>(powerSettings);
    if (dir == DOWN)
        lvl--;
    else if (dir == UP)
        lvl++;

    // update to new lvl.
    return absoluteUpdate(lvl);
}

bool InertialSensor::absoluteUpdate(int val)
{
    int maxLvl = std::get<0>(powerSettings);
    if (val < 0 || val > maxLvl) // Out of bounds level
        return false;

    updatePowerSettings(static_cast<InertiaPowerMode>(val));
    return true;
}

void InertialSensor::updatePowerSettings(InertiaPowerMode lvl)
{
    switch (lvl)
    { // Max, Current, Scaled, Sampletime
    case HZ_7p5:
        powerSettings = std::make_tuple(4, 0, 0.0f, 133);
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_OFF);
        break;
    case HZ_15:
        powerSettings = std::make_tuple(4, 1, 0.0625f, 67);
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_15Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_15Hz);
        break;
    case HZ_60:
        powerSettings = std::make_tuple(4, 2, 0.25f, 17);
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_60Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_60Hz);
        break;
    case HZ_120:
        powerSettings = std::make_tuple(4, 3, 0.5f, 8);
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_120Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_120Hz);
        break;
    case HZ_240:
        powerSettings = std::make_tuple(4, 4, .0f, 4);
        lsm.setAccelDataRate(LSM6DSV16X_ODR_AT_240Hz);
        lsm.setGyroDataRate(LSM6DSV16X_ODR_AT_240Hz);
        break;
    }

    Serial.println("Inertia ODR set to: " + String(lvl));
}

void InertialSensor::calibrate()
{
    Serial.println("Starting Inertia Calibration");

    int originalPowerMode = std::get<1>(powerSettings);
    updatePowerSettings(HZ_240); // Max res just for this period
    delay(1000);    // Wait for the sensor to stabilize

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

    absoluteUpdate(originalPowerMode); // Revert to whatever power mode we started with
    Serial.println("Inertia Calibration Complete!");
}

float InertialSensor::calculateInterestingness()
{
    // Get variance of magnitudinal accel values in the buffer;
    float mean = 0;
    for (int i = 0; i < readingBuffer.getReadingCount()-1; i++)
    {
        InertiaSensorReadings reading;
        readingBuffer.getReading(i, reading);
        mean += reading.accelMag;
    }

    float variance = 0.0f;
    for (int i = 0; i < readingBuffer.getReadingCount()-1; i++)
    {
        InertiaSensorReadings reading;
        readingBuffer.getReading(i, reading);
        float diff = reading.accelMag - mean;
        variance+= diff*diff;
    }


    const float midpoint = 1;
    const float steepness = 3;

    // Sigmoid-like mapping to an "interestingness" score
    float interestingness = 1.0f / (1.0f + expf(-steepness * (variance - midpoint)));
    return interestingness;
}