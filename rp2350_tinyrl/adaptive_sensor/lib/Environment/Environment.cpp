#include "Environment.h"

// Empty/skeleton implementations based on header file.

RawState Environment::step(int action)
{
    switch (action)
    {
    case 0:
        // Turn up sensor with highest negative M value
        break;
    case 1:
        // Turn down sensor with highest positive M value
        break;
    default:
        // Do nothing
        break;
    }

    // Collect observation with new settings
    SensorObservation o = sensorController->collectData();

    // Determine new state
    State s = stateCalculator.c


    // Calculate reward
    float r = rewardCalculator.calculate(
        o.sensorStateData,
        conservatism,
        batteryFrac,
    );
    

}

