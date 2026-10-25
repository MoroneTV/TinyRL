#include "AirQualitySensor.h"
#include <stdlib.h>

bool AirQualitySensor::begin()
{

    bme.begin(0x76, Wire);

    // Check for errors
    if (bme.checkStatus())
    {
        if (bme.checkStatus() == BME68X_ERROR)
        {
            D_println("Sensor error:" + bme.statusString());
            return false;
        }
        else if (bme.checkStatus() == BME68X_WARNING)
        {
            D_println("Sensor Warning:" + bme.statusString());
            return false;
        }
    }

    // Start in forced mode
    bme.setOpMode(BME68X_FORCED_MODE);

    /* Set the default configuration for temperature, pressure and humidity */
    bme.setTPH();

    /* Set the heater configuration to 300 deg C for 100ms for Forced mode */
    bme.setHeaterProf(300, 100);

    /* Clear buffer timestamps from junk data */
    for (int i = 0; i < historySize; i++)
    {
        history[i] = {};
    }
    head = 0;
    count = 0;

    setStatus(true);
    lastSampleTime = millis();
    return true;
}

void AirQualitySensor::update(int now)
{

    // Add a reading to the buffer if enough time has elapsed
    if (now - lastSampleTime > sampleInterval)
    {
        bme68xData data;
        bme.setOpMode(BME68X_FORCED_MODE);
        delay(150);

        if (bme.fetchData())
        {
            bme.getData(data);
        }

        AirQualityReading r = {
            data.temperature,
            data.pressure,
            data.humidity,
            data.gas_resistance,
            now};

        history[head] = r;
        head = (head + 1) % historySize;

        if (count < historySize)
            count++;

        lastSampleTime = now;

    }
}

void AirQualitySensor::setLvl(int lvl)
{

    // Keep lvl in bounds
    if (lvl < 0)
        lvl = 0;
    if (lvl > 4)
        lvl = 4;

    AirQualityPowerLevel lvl_ = static_cast<AirQualityPowerLevel>(lvl);

    powerMode = lvl;
    updateCostPerTimestep(lvl_);
    updateSampleInterval(lvl_);
    updateScaledLevel(lvl_);
}

float AirQualitySensor::getInterestingness()
{

    if (count < 2)
        return 0.0f; // 0 interestingness if no readings yet.

    AirQualityReading current = getMostRecentReading();
    AirQualityReading *old = nullptr;

    int now = millis();
    uint32_t best30sError = UINT32_MAX;

    // Find the reading closest to 30s ago and the most recent one
    for (int i = 0; i < count; i++)
    {
        int error30s = abs((int)((history[i].timestamp + 30000UL) - now));
        if (error30s < best30sError)
        {
            best30sError = error30s;
            old = &history[i];
        }
    }
    // Calculate the normalised euclidian distances of readings
    float tempChange = abs(current.temperature - old->temperature) / 2.0f;                          // 2C change in 30s
    float humChange = abs(current.humidity - old->humidity) / 10.0f;                                // 10% humidity change in 30s
    float gasChange = abs(current.gasResistance - old->gasResistance) / old->gasResistance / 0.25f; // 25% gas change in 30s

    float distance = sqrt(
        tempChange * tempChange +
        humChange * humChange +
        gasChange * gasChange);


    return constrain(distance / sqrt(3.0f), 0.0f, 1.0f);
}
