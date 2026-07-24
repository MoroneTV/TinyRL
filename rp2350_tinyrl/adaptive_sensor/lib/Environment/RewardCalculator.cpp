#include "RewardCalculator.h"
#include <algorithm>


float RewardCalculator::calculate(
    std::vector<SensorState> sensors,
    float conservatism,
    float batteryFrac,
    CompletionState completionState
)
{  

    for (SensorState& s : sensors) {

        float value = 0.0f; // Value the sensor has brought this timestep
        float match = 0.0f; // Match between sensor power lvl and interestingness lvl
        
        match = (1-abs(s.interestingness - s.scaledLvl));    // good match = 1, poor match = 0
        value = s.weight * match;   // Value is the match scaled by the sensor's weight.

        float cost = 0.0f;  // Overall cost of using the sensor at this timestep
        float power = 1 - batteryFrac;  // Cost is scaled by the amount of battery left in episode

        cost = conservatism * s.flatCost


        total += v-c;
    }


}


    float total = 0.0f;
    float v = 0.0f;
    float c = 0.0f;

    for (Sensor& s : sensors) {

        v = s.getWeight() * (1-abs(s.getInterestingness() - s.getScaledLvl()));
        c = conservatism * (1-batteryFrac) * s.getFlatCost();

        total += v-c;
    }

    if (successfulCompletion) total += 200;

    return total;