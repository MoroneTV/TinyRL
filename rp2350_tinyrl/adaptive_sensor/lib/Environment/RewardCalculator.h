#ifndef REWARD_CALCULATOR_H
#define REWARD_CALCULATOR_H

#include <Environment.h>

class RewardCalculator
{
public:
    RewardCalculator();

    float getReward() { return reward; }

    float calculateReward(
        std::vector<Sensor> sensors,
        float conservatism,
        float batteryFrac,
        bool successfulCompletion
    );

    void reset() { reward = 0.0f; }

private:
    float reward;
};

#endif // REWARD_CALCULATOR_H
