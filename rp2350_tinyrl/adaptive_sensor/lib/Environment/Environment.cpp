#include "Environment.h"

// Empty/skeleton implementations based on header file.

void Environment::begin()
{
    sensorController = new SensorController(weights);
    sensorController->begin();
}
Feedback Environment::step(int action)
{
    D_println("========== ENV STEP ==========");

    D_println("ACTION: " + String(action));

    switch (action)
    {
    case 0:
        D_println("ACTION TYPE: INCREASE SENSOR");
        D_println("TARGET SENSOR: " + String(prevState.maxUnderSampleID));

        sensorController->updateSensorSettings(
            prevState.maxUnderSampleID,
            UP);
        break;

    case 1:
        D_println("ACTION TYPE: DECREASE SENSOR");
        D_println("TARGET SENSOR: " + String(prevState.maxOverSampleID));

        sensorController->updateSensorSettings(
            prevState.maxOverSampleID,
            DOWN);
        break;

    default:
        D_println("ACTION TYPE: DO NOTHING");
        break;
    }

    // ================= OBSERVATION =================

    D_println("COLLECTING DATA");

    SensorObservation o = sensorController->collectData();

    D_println("SENSOR COUNT: " + String(o.sensorStateData.size()));

    for (SensorState sensor : o.sensorStateData)
    {
        D_println("SENSOR ID: " + String(sensor.sensorId));
        D_println("SENSOR NAME: " + sensor.sensorName);
        D_println("SENSOR ENABLED: " + String(sensor.isEnabled));
        D_println("SENSOR RAW LVL: " + String(sensor.rawLvl));
        D_println("SENSOR SCALED LVL: " + String(sensor.scaledLvl));
        D_println("SENSOR INTERESTINGNESS: " + String(sensor.interestingness));
        D_println("SENSOR WEIGHT: " + String(sensor.weight));
        D_println("SENSOR COST: " + String(sensor.flatCost));
    }

    // ================= STATE =================

    D_println("CALCULATING STATE");

    State s = stateCalculator.calculate(
        o.sensorStateData,
        (energyLeftInEpisode / energyPerEpisode),
        msLeftInEpisode,
        energyPerEpisode);

    D_println("NEW STATE: " + String(s.encodedState));
    D_println("UNDER SAMPLE LVL: " + String(s.maxUnderSample));
    D_println("OVER SAMPLE LVL: " + String(s.maxOverSample));
    D_println("BATTERY LVL: " + String(s.currentBattery));
    D_println("DRAIN RATE: " + String(s.drainRate));

    D_println("ENERGY REMAINING: " + String(s.energyRemaining));
    D_println("TIME REMAINING: " + String(s.timeRemaining));
    D_println("EPISODE STATE: " + String(s.epState));

    energyLeftInEpisode = s.energyRemaining;
    msLeftInEpisode = s.timeRemaining;

    // ================= REWARD =================

    D_println("CALCULATING REWARD");

    float r = rewardCalculator.calculate(
        o.sensorStateData,
        conservatism,
        (energyLeftInEpisode / energyPerEpisode),
        s.epState);

    D_println("REWARD: " + String(r));

    // ================= FEEDBACK =================

    Feedback f;

    f.isDone = (s.epState != INCOMPLETE);
    f.newState = s.encodedState;
    f.reward = r;

    D_println("DONE: " + String(f.isDone));
    D_println("NEXT STATE: " + String(f.newState));

    prevState = s;

    D_println("========== END ENV STEP ==========");

    return f;
}

/*
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
    */

State Environment::resetEnv()
{

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
        energyLeftInEpisode);

    // return state
    return prevState;
}
