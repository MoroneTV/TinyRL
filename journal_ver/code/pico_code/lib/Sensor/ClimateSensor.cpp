#include "ClimateSensor.h"
#include <stdlib.h>

bool ClimateSensor::initialise()
{

    bme.begin(0x76, Wire);

    // Check for errors
    if (bme.checkStatus())
    {
        if (bme.checkStatus() == BME68X_ERROR)
        {
            Serial.println("Sensor error:" + bme.statusString());
            return false;
        }
        else if (bme.checkStatus() == BME68X_WARNING)
        {
            Serial.println("Sensor Warning:" + bme.statusString());
            return false;
        }
    }

    if (sensorWeight == 0)
    {
        Serial.println("Climate sensor weight is 0, will not activate");
        return false;
    }

    // Start in forced mode
    bme.setOpMode(BME68X_FORCED_MODE);

    /* Set the default configuration for temperature, pressure and humidity */
    bme.setTPH();

    /* Set the heater configuration to 300 deg C for 100ms for Forced mode */
    bme.setHeaterProf(300, 100);

    // Set initial level to 5s.
    absoluteUpdate(SAMPLE_5s);

    isEnabled = true;
    lastSampleTime = millis();
    return true;
}

void ClimateSensor::update(unsigned long now)
{  
    int sampleInterval = std::get<3>(powerSettings);

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

        ClimateSensorReadings r = {
            data.temperature,
            data.pressure,
            data.humidity,
            data.gas_resistance,
            now
        };

        readingBuffer.addReading(r);
        lastSampleTime = now;

    }
}

bool ClimateSensor::updateSettings(ControllerInstruction instructions)
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

bool ClimateSensor::relativeUpdate(Direction dir)
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

bool ClimateSensor::absoluteUpdate(int val)
{
    int maxLvl = std::get<0>(powerSettings);
    if (val < 0 || val > maxLvl) // Out of bounds level
        return false;

    updatePowerSettings(static_cast<ClimatePowerMode>(val));
    return true;
}

void ClimateSensor::updatePowerSettings(ClimatePowerMode lvl)
{
    switch (lvl)
    { // Max, Current, Scaled, Sampletime
    case SAMPLE_30s:
        powerSettings = std::make_tuple(4, 0, 0.0f, 30000);
        break;
    case SAMPLE_15s:
        powerSettings = std::make_tuple(4, 1, 0.075f, 15000);
        break;
    case SAMPLE_10s:
        powerSettings = std::make_tuple(4, 2, 0.1f, 10000);
        break;
    case SAMPLE_5s:
        powerSettings = std::make_tuple(4, 3, 0.2f, 5000);
        break;
    case SAMPLE_1s:
        powerSettings = std::make_tuple(4, 4, 1.0f, 1000);
        break;
    }
}


float ClimateSensor::calculateInterestingness()
{

    // Get most recent reading.
    ClimateSensorReadings r;
    readingBuffer.getReading(readingBuffer.getReadingCount()-1, r);
    float temperature = r.temperature;

    // Find how close it is to the upper and lower bound in terms of temperature
    float midpoint = (lowerBound + upperBound) / 2.0;
    float half_range = (upperBound - lowerBound) / 2.0;

    if (half_range <= 0.0)
        return 1.0;

    return constrain(
        std::abs(temperature - midpoint) / half_range,
        0.0,
        1.0
    );

}
