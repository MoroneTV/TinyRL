#ifndef ENV_REWARD_H
#define ENV_REWARD_H

#include <Environment.h>

class EnvReward
{
public:
    EnvReward();

    float getReward() { return reward; }

    float calculateReward(
        std::vector<Sensor> sensors,
        float conservatism,
        float batteryFrac
    );

    void reset() { reward = 0.0f; }

private:
    float reward;
};

#endif // ENV_REWARD_H
