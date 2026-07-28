#ifndef STATECALCULATOR_H
#define STATECALCULATOR_H

#include <vector>
#include <tuple>
#include "EnvStructs.h"
#include "SensorStructs.h"

class StateCalculator {
public:
    StateCalculator();

    State calculate(
        std::vector<SensorState> sensors,
        float batteryFrac,
        int timeRemaining,
        float energyRemaining
    );

private:

    std::tuple<int,float> findMaxUnderSample(std::vector<SensorState>& sensors);
    std::tuple<int,float> findMaxOverSample(std::vector<SensorState>& sensors);

    int getDiscreteBatteryLvl(float batteryFrac);
    int getDiscreteBatteryDrain(std::vector<SensorState>& sensors);
    int getDiscreteSampleLvl(float sample);
    int getLeastImportantSensor(std::vector<SensorState>& sensors);
};

#endif // STATECALCULATOR_H
