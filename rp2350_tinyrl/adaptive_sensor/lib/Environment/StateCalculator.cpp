#include <vector>
#include "StateCalculator.h"

StateCalculator::StateCalculator() = default;

State StateCalculator::calculate(
    std::vector<SensorState> sensors,
    float batteryFrac,
    int timeRemaining,
    float energyRemaining)
{
    State s;

    // Get the degree and ID of most over and under-sampled sensors.
    std::tuple<int, float> overSample = findMaxOverSample(sensors);
    s.maxOverSampleID = std::get<0>(overSample);
    s.maxOverSample = getDiscreteSampleLvl(std::get<1>(overSample));

    std::tuple<int, float> underSample = findMaxUnderSample(sensors);
    s.maxUnderSampleID = std::get<0>(underSample);
    s.maxUnderSample = getDiscreteSampleLvl(std::get<1>(underSample));

    s.leastImportantSensorID = getLeastImportantSensor(sensors);

    // Sort continous battery information into discrete bins
    s.currentBattery = getDiscreteBatteryLvl(batteryFrac);

    // Calculate energy usage
    float energyUsed = 0.0f;
    for (SensorState sensor : sensors)
        energyUsed += sensor.flatCost;
    s.energyRemaining = energyRemaining - energyUsed;

    // Reduce time remaining by 1 second
    s.timeRemaining = timeRemaining - 1000;

    // Check if episode is a success, failure or ongoing
    if (s.timeRemaining <= 0 && s.energyRemaining >= 0)
        s.epState = SUCCESS;
    else if (s.energyRemaining <= 0 && s.timeRemaining >= 0)
        s.epState = FAILURE;
    else
        s.epState = INCOMPLETE;

    s.encodedState =
        s.maxUnderSample + 10 * s.maxOverSample + 10 * 10 * s.currentBattery;

    return s;
}

std::tuple<int, float> StateCalculator::findMaxUnderSample(std::vector<SensorState> &sensors)
{

    float max = 0.0f;
    int ID = -1;

    for (SensorState sensor : sensors)
    {

        // If the agent would turn anything up a level, it would be the sensor of most importance
        // That has the highest interestingness as compared to what lvl it is.
        float weightedDiscrepancy = (sensor.interestingness - sensor.scaledLvl) * (sensor.weight / 2.0f);

        if (weightedDiscrepancy > max)
        {
            max = weightedDiscrepancy;
            ID = sensor.sensorId;
        }
    }

    return std::make_tuple(ID, max);
}

std::tuple<int, float> StateCalculator::findMaxOverSample(std::vector<SensorState> &sensors)
{

    float min = 0.0f;
    int ID = -1;

    for (SensorState sensor : sensors)
    {

        // If the agent would turn anything down a level, it would be the sensor of least importance
        // That has the highest level as compared to how interesting the data is.
        float weightedDiscrepancy = (sensor.interestingness - sensor.scaledLvl) * (1 - (sensor.weight / 2.0f));

        if (weightedDiscrepancy < min)
        {
            min = weightedDiscrepancy;
            ID = sensor.sensorId;
        }
    }

    return std::make_tuple(ID, min);
}

int StateCalculator::getDiscreteBatteryLvl(float batteryFrac)
{
    batteryFrac *= 100.0f;

    if (batteryFrac >= 80.0f)
        return 4;
    else if (batteryFrac >= 60.0f)
        return 3;
    else if (batteryFrac >= 40.0f)
        return 2;
    else if (batteryFrac >= 20.0f)
        return 1;
    else
        return 0;
}

int StateCalculator::getDiscreteSampleLvl(float sample)
{

    // Convert negative values to positive
    if (sample < 0.0f)
        sample = -sample;

    if (sample >= 0.9f)
        return 9;
    else if (sample >= 0.8f)
        return 8;
    else if (sample >= 0.7f)
        return 7;
    else if (sample >= 0.6f)
        return 6;
    else if (sample >= 0.5f)
        return 5;
    else if (sample >= 0.4f)
        return 4;
    else if (sample >= 0.3f)
        return 3;
    else if (sample >= 0.2f)
        return 2;
    else if (sample >= 0.1f)
        return 1;
    else
        return 0;
}

int StateCalculator::getLeastImportantSensor(std::vector<SensorState> &sensors)
{

    float mostCostly = 0.0f;
    int ID = -1;

    for (SensorState sensor : sensors)
    {   

        if (sensor.rawLvl == 0) continue;

        float importanceFactor = 0.25f + 0.75f * (1.0f - sensor.weight);
        float calibratedCost = sensor.flatCost * importanceFactor;

        if (calibratedCost > mostCostly)
        {
            mostCostly = calibratedCost;
            ID = sensor.sensorId;
        }
    }
    return ID;
}
