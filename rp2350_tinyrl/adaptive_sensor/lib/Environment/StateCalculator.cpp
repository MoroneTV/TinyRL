#include <vector>
#include "StateCalculator.h"

StateCalculator::StateCalculator() = default;

State StateCalculator::calculate(
    std::vector<SensorState> sensors,
    float batteryFrac
) {
    State s;
    
    std::tuple<int, float> overSample = findMaxOverSample(sensors);
    s.maxOverSampleID = std::get<0>(overSample);
    s.maxOverSample = getDiscreteSampleLvl(std::get<1>(overSample));
    
    std::tuple<int, float> underSample = findMaxUnderSample(sensors);
    s.maxUnderSampleID = std::get<0>(underSample);
    s.maxUnderSample = getDiscreteSampleLvl(std::get<1>(underSample));

    s.currentBattery = getDiscreteBatteryLvl(batteryFrac);
    s.drainRate =  getDiscreteBatteryDrain(sensors);

    int encodedState = s.maxUnderSample * s.maxOverSample * s.currentBattery + s.drainRate;

    return s;
}

std::tuple<int, float> StateCalculator::findMaxUnderSample(std::vector<SensorState>& sensors) {

    float max = 0.0f;
    int ID = -1;

    for (SensorState sensor : sensors) {

        // If the agent would turn anything up a level, it would be the sensor of most importance
        // That has the highest interestingness as compared to what lvl it is. 
        float weightedDiscrepancy = (sensor.interestingness - sensor.scaledLvl) * sensor.weight;

        if (weightedDiscrepancy > max)
        {
            max = weightedDiscrepancy;
            ID = sensor.sensorId;
        }
    }

    return std::make_tuple(ID, max);

}

std::tuple<int, float> StateCalculator::findMaxOverSample(std::vector<SensorState>& sensors) {
    
    float min = 0.0f;
    int ID = -1;

    for (SensorState sensor : sensors) {

        // If the agent would turn anything down a level, it would be the sensor of least importance
        // That has the highest level as compared to how interesting the data is. 
        float weightedDiscrepancy = (sensor.interestingness - sensor.scaledLvl) * (1-sensor.weight);

        if (weightedDiscrepancy < min)
        {
            min = weightedDiscrepancy;
            ID = sensor.sensorId;
        }
    }

    return std::make_tuple(ID, min);
}

int StateCalculator::getDiscreteBatteryLvl(float batteryFrac) {
    if (batteryFrac >= 80.0f) return 4;
    else if (batteryFrac >= 60.0f) return 3;
    else if (batteryFrac >= 40.0f) return 2;
    else if (batteryFrac >= 20.0f) return 1;
    else return 0;
}

int StateCalculator::getDiscreteBatteryDrain(std::vector<SensorState>& sensors) {

    float drain = 0.0f;
    for (SensorState sensor : sensors) drain += sensor.flatCost;

    if (drain >= 60.0f) return 4;
    else if (drain >= 40.0f) return 3;
    else if (drain >= 20.0f) return 2;
    else if (drain >= 10.0f) return 1;
    else return 0;
}

int StateCalculator::getDiscreteSampleLvl(float sample) {

    // Convert negative values to positive
    if (sample < 0.0f) sample = -sample;

    if (sample >= 0.9f) return 9;
    else if (sample >= 0.8f) return 8;
    else if (sample >= 0.7f) return 7;
    else if (sample >= 0.6f) return 6;
    else if (sample >= 0.5f) return 5;
    else if (sample >= 0.4f) return 4;
    else if (sample >= 0.3f) return 3;
    else if (sample >= 0.2f) return 2;
    else if (sample >= 0.1f) return 1;
    else return 0;
}

