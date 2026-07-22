#include "EnvReward.h"
#include <algorithm>

EnvReward::EnvReward()
    : reward(0.0f)
{
}

float EnvReward::calculateReward(
    std::unordered_map<SensorType, float> weights,
    std::unordered_map<SensorType, float> interestingness,
    std::unordered_map<SensorType, float> scaledLevel,
    std::unordered_map<SensorType, float> energyCosts,
    float conservatism,
    float batteryFrac
)
{
    // Basic reward: for each sensor, value = weight * (interestingness * scaledLevel)
    // cost is conservatism * energyCosts. batteryFrac scales the final reward.
    float total = 0.0f;

    for (const auto &p : weights) {
        SensorType s = p.first;
        float w = p.second;

        float inte = 0.0f;
        float scale = 0.0f;
        float cost = 0.0f;

        auto itI = interestingness.find(s);
        if (itI != interestingness.end()) inte = itI->second;

        auto itS = scaledLevel.find(s);
        if (itS != scaledLevel.end()) scale = itS->second;

        auto itC = energyCosts.find(s);
        if (itC != energyCosts.end()) cost = itC->second;

        float benefit = inte * scale;
        float sensorReward = w * (benefit - conservatism * cost);
        total += sensorReward;
    }

    // scale by battery fraction in range [0,1]
    batteryFrac = std::clamp(batteryFrac, 0.0f, 1.0f);
    reward = total * batteryFrac;
    return reward;
}

void EnvReward::reset()
{
    reward = 0.0f;
}
