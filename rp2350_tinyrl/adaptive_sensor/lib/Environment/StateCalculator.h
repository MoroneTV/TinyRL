#ifndef STATECALCULATOR_H
#define STATECALCULATOR_H

#include <Environment.h>
#include <vector>
#include <tuple>

class StateCalculator {
public:
    StateCalculator();

    State calculate(
        std::vector<SensorState> sensors,
        float batteryFrac
    );

private:

    std::tuple<int,float> findMaxUnderSample(std::vector<SensorState>& sensors);
    std::tuple<int,float> findMaxOverSample(std::vector<SensorState>& sensors);

    int getDiscreteBatteryLvl(float batteryFrac);
    int getDiscreteBatteryDrain(std::vector<SensorState>& sensors);
    int getDiscreteSampleLvl(float sample);
};

#endif // STATECALCULATOR_H
