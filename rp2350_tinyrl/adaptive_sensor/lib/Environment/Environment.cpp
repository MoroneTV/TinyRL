#include "Environment.h"

// Empty/skeleton implementations based on header file.

Feedback Environment::step(int action)
{
    switch (action)
    {
    case 0:
        // Turn up sensor with highest negative M value
        sensorController->updateSensorSettings(prevState.maxUnderSampleID, UP);
        break;
    case 1:
        // Turn down sensor with highest positive M value
        sensorController->updateSensorSettings(prevState.maxOverSampleID, DOWN);
        break;
    default:
        // Do nothing
        break;
    }

    // Collect observation with new settings
    SensorObservation o = sensorController->collectData();

    // Determine new state, update time and energy values
    State s = stateCalculator.calculate(
        o.sensorStateData, 
        (energyLeftInEpisode / energyPerEpisode),
        msLeftInEpisode,
        energyPerEpisode
    );
    energyPerEpisode = s.energyRemaining;
    msLeftInEpisode = s.timeRemaining;

    // Calculate reward
    float r = rewardCalculator.calculate(
        o.sensorStateData,
        conservatism,
        (energyLeftInEpisode / energyPerEpisode),
        s.epState
    );

    // Create feedback object that is given back to agent (is ep done, new encoded state, reward value)
    Feedback f;
    (s.epState == INCOMPLETE) ? f.isDone = false : f.isDone = true; 
    f.newState = s.encodedState;
    f.reward = r;

    // State becomes the new previous state
    prevState = s;

    return f;
}

State Environment::resetEnv() {
    
    // Reset energy and time
    msLeftInEpisode = msPerEpisode;
    energyLeftInEpisode = energyPerEpisode;

    // Reset Sensors
    sensorController->resetSensors();

    // Make fresh state
    SensorObservation o = sensorController->collectData();
    prevState = stateCalculator.calculate(
        o.sensorStateData,
        (energyLeftInEpisode / energyPerEpisode),
        msLeftInEpisode,
        energyLeftInEpisode
    );

    // return state
    return prevState;
}

