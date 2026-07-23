#include "EnvReward.h"
#include <algorithm>

EnvReward::EnvReward()
    : reward(0.0f)
{
}

float EnvReward::calculateReward(
    std::vector<Sensor> sensors,
    float conservatism,
    float batteryFrac
)
{
    float total = 0.0f;
    float v = 0.0f;
    float c = 0.0f;

    for (Sensor& s : sensors) {

        v = s.getWeight() * (1-abs(s.getInterestingness() - s.getScaledLvl()));
        c = conservatism * (1-batteryFrac) * s.getFlatCost();

        total += v-c;
    }

    return total;

}

