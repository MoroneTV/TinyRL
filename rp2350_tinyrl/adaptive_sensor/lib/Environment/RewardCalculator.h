#ifndef REWARD_CALCULATOR_H
#define REWARD_CALCULATOR_H

#include <EnvStructs.h>
#include <SensorStructs.h>
#include <vector>

class RewardCalculator
{
public:
    RewardCalculator() {};
    float calculate(
        std::vector<SensorState> sensors,
        float conservatism,
        float batteryFrac,
        EpisodeCompletion successfulCompletion
    );

private:
};

#endif // REWARD_CALCULATOR_H
