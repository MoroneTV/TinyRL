#include "RewardCalculator.h"
#include <algorithm>


float RewardCalculator::calculate(
    std::vector<SensorState> sensors,
    float conservatism,
    float batteryFrac,
    EpisodeCompletion completionState
)
{  
    float reward = 0.0f;

    for (SensorState& s : sensors) {

        float value = 0.0f; // Overall value the sensor has brought this timestep
        float match = 0.0f; // Match between sensor power lvl and interestingness lvl
        
        match = (1-abs(s.interestingness - s.scaledLvl));    // good match = 1, poor match = 0
        value = s.weight * match;   // Value is the match scaled by the sensor's weight.

        float cost = 0.0f;  // Overall cost of using the sensor at this timestep
        float costScale = conservatism * (1-batteryFrac); // Weight of cost determined by battery level and conservatism modifier

        cost = s.flatCost * costScale;

        // Overall sensor reward is (value the sensor brings) - (the cost it takes to run it)
        reward += value - cost;
    }

    // If the episode is over, punish or reward the agent with a bonus.
    switch (completionState)
    {
    case SUCCESS:
        reward += 200;
        break;
    case FAILURE:
        reward -= 200;
        break;
    default:    // Incomplete episode
        break;
    }

    return reward;
}
