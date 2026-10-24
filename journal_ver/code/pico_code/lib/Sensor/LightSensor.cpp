#include "LightSensor.h"

bool LightSensor::initialise()
{
    if (!bh17.begin(0x38))
    {
        Serial.println("Light Sensor Begin Failure!");
        return false;
    }

    if (sensorWeight == 0)
    {
        Serial.println("Light sensor weight is 0, will not activate");
        return false;
    }

    // Set initial level to minimum.
    updatePowerSettings(SAMPLE_2560ms);

    bh17.setGain(bh17.Gain1X);

    isEnabled = true;
    lastSampleTime = millis(); // Intiial reference point for update function
    return true;
}

void LightSensor::update(unsigned long now)
{

    int sampleInterval = std::get<3>(powerSettings);

    // Add a reading to the buffer if enough time has elapsed
    if (now - lastSampleTime > sampleInterval)
    {

        if (!bh17.read())
            Serial.println("Light Sensor Read Failure!");

        LightSensorReadings r = {
            bh17.clear,
            bh17.red,
            bh17.blue,
            bh17.green,
            (bh17.red + bh17.blue + bh17.green + bh17.clear) / 4.0f,
            now};

        readingBuffer.addReading(r);

        lastSampleTime = now;
    }
}

bool LightSensor::updateSettings(ControllerInstruction instructions)
{
    if (instructions.type == RELATIVE)
    {
        return relativeUpdate(instructions.direction);
    }

    else /* if instruction is absolule */
    {
        return absoluteUpdate(instructions.value);
    }
}

bool LightSensor::relativeUpdate(Direction dir)
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

bool LightSensor::absoluteUpdate(int val)
{
    int maxLvl = std::get<0>(powerSettings);
    if (val < 0 || val > maxLvl) // Out of bounds level
        return false;

    updatePowerSettings(static_cast<LightPowerMode>(val));
    return true;
}

void LightSensor::updatePowerSettings(LightPowerMode lvl)
{
    switch (lvl)
    { // Max, Current, Scaled, Sampletime
    case SAMPLE_160ms:
        powerSettings = std::make_tuple(4, 4, 1.0f, 160);
        bh17.setMeasurementTime(bh17.Time160ms);
        break;
    case SAMPLE_320ms:
        powerSettings = std::make_tuple(4, 3, 0.75f, 320);
        bh17.setMeasurementTime(bh17.Time320ms);
        break;
    case SAMPLE_640ms:
        powerSettings = std::make_tuple(4, 2, 0.5f, 640);
        bh17.setMeasurementTime(bh17.Time640ms);
        break;
    case SAMPLE_1280ms:
        powerSettings = std::make_tuple(4, 1, 0.25f, 1280);
        bh17.setMeasurementTime(bh17.Time1280ms);
        break;
    case SAMPLE_2560ms:
        powerSettings = std::make_tuple(4, 0, 0.0f, 2560);
        bh17.setMeasurementTime(bh17.Time2560ms);
        break;
    }
}

// Interestingness defined by variance of combined light values across buffer.
float LightSensor::calculateInterestingness()
{

    float mean = 0;
    for (int i = 0; i < readingBuffer.getReadingCount()-1; i++)
    {
        LightSensorReadings reading;
        readingBuffer.getReading(i, reading);
        mean += reading.combined;
    }

    float variance = 0.0f;
    for (int i = 0; i < readingBuffer.getReadingCount()-1; i++)
    {
        LightSensorReadings reading;
        readingBuffer.getReading(i, reading);
        float diff = reading.combined - mean;
        variance+= diff*diff;
    }

    float stdDev = sqrt(variance);

    return constrain(stdDev / 2000.0f, 0.0f, 1.0f);
}

