#include "LightSensor.h"
#include <stdlib.h>
#include <debug.h>

bool LightSensor::begin()
{

    if (!bh17.begin(0x38))
    {
        D_println("Light Sensor Begin Failure!");
        return false;
    }

    /* Clear buffer timestamps from junk data */
    for (int i = 0; i < historySize; i++)
    {
        history[i] = {};
    }
    head = 0;
    count = 0;

    // Set initial level.
    setLvl(powerMode);

    bh17.setGain(bh17.Gain1X);

    setStatus(true);
    lastSampleTime = millis(); // Intiial reference point for update function
    return true;
}

void LightSensor::update(int now)
{

    // Add a reading to the buffer if enough time has elapsed
    if (now - lastSampleTime > sampleInterval)
    {

        if (!bh17.read())
            D_println("Light Sensor Read Failure!");

        LightReading r = {
            bh17.red,
            bh17.blue,
            bh17.green,
            bh17.clear,
            now};

        history[head] = r;
        head = (head + 1) % historySize;

        if (count < historySize)
            count++;

        lastSampleTime = now;
    }
}

void LightSensor::setLvl(int lvl)
{

    // Keep lvl in bounds
    if (lvl < 0)
        lvl = 0;

    if (lvl > 5)
        lvl = 5;

    LightPowerLevel lvl_ = static_cast<LightPowerLevel>(lvl);

    switch (lvl_)
    {
    case SAMPLE_160ms:
        bh17.setMeasurementTime(bh17.Time160ms);
        break;
    case SAMPLE_320ms:
        bh17.setMeasurementTime(bh17.Time320ms);
        break;
    case SAMPLE_640ms:
        bh17.setMeasurementTime(bh17.Time640ms);
        break;
    case SAMPLE_1280ms:
        bh17.setMeasurementTime(bh17.Time1280ms);
        break;
    case SAMPLE_2560ms:
        bh17.setMeasurementTime(bh17.Time2560ms);
        break;
    case SAMPLE_5120ms:
        bh17.setMeasurementTime(bh17.Time5120ms);
        break;
    }

    powerMode = lvl;
    updateCostPerTimestep(lvl_);
    updateSampleInterval(lvl_);
    updateScaledLevel(lvl_);
}

float LightSensor::getInterestingness()
{

    float mean = 0;
    for (int i = 0; i < count; ++i) mean += history[i].clear;
    mean = mean /= count;

    float variance = 0.0f;
    for (int i = 0; i < count; ++i) {
        float diff = float(history[i].clear) - mean;
        variance += diff*diff;
    }

    float stdDev = sqrt(variance);

    return constrain(stdDev / 2000.0f, 0.0f, 1.0f);
}
