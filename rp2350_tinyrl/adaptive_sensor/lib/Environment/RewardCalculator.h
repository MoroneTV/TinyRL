#ifndef REWARD_CALCULATOR_H
#define REWARD_CALCULATOR_H

#include <Environment.h>

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
