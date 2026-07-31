#include "Environment.h"

// Empty/skeleton implementations based on header file.

void Environment::begin()
{
    sensorController = new SensorController(weights);
    sensorController->begin();
}
Feedback Environment::step(int action)
{
    D_println("");
    D_println("========== ENV STEP ==========");

    bool invalidAction = false;
    Sensor *target = nullptr;
    int lvl = -1;

    switch (action)
    {
    case 0:
    {
        // Turn up sensor with highest negative M value
        D_println("ACTION TYPE: INCREASE SENSOR");
        D_println("TARGET SENSOR: " + String(prevState.maxUnderSampleID));

        target = sensorController->getSensorByID(prevState.maxUnderSampleID);

        // No valid sensor available to increase
        if (target == nullptr)
        {
            invalidAction = true;
            D_println("INVALID ACTION: NO SENSOR TO INCREASE");
            break;
        }

        // Store current level so we can check whether the action actually changed anything
        lvl = target->getRawLvl();

        sensorController->updateSensorSettings(
            prevState.maxUnderSampleID,
            UP);

        // If the level did not change, the sensor was already at maximum
        if (target->getRawLvl() == lvl)
        {
            invalidAction = true;
            D_println("INVALID ACTION: SENSOR ALREADY AT MAX LEVEL");
        }

        break;
    }

    case 1:
    {
        // Turn down sensor with highest positive M value
        D_println("ACTION TYPE: DECREASE SENSOR");
        D_println("TARGET SENSOR: " + String(prevState.maxOverSampleID));

        target = sensorController->getSensorByID(prevState.maxOverSampleID);

        // No valid sensor available to decrease
        if (target == nullptr)
        {
            invalidAction = true;
            D_println("INVALID ACTION: NO SENSOR TO DECREASE");
            break;
        }

        // Store current level so we can check whether the action actually changed anything
        lvl = target->getRawLvl();

        sensorController->updateSensorSettings(
            prevState.maxOverSampleID,
            DOWN);

        // If the level did not change, the sensor was already at minimum
        if (target->getRawLvl() == lvl)
        {
            invalidAction = true;
            D_println("INVALID ACTION: SENSOR ALREADY AT MIN LEVEL");
        }

        break;
    }

    case 2:
    {
        // Turn down most costly sensor with cost adjusted by importance
        D_println("ACTION TYPE: SAVE ENERGY");
        D_println("TARGET SENSOR: " + String(prevState.leastImportantSensorID));

        target = sensorController->getSensorByID(prevState.leastImportantSensorID);

        // No valid sensor available to decrease
        if (target == nullptr)
        {
            invalidAction = true;
            D_println("INVALID ACTION: NO SENSOR AVAILABLE FOR ENERGY SAVING");
            break;
        }

        // Store current level so we can check whether the action actually changed anything
        lvl = target->getRawLvl();

        sensorController->updateSensorSettings(
            prevState.leastImportantSensorID,
            DOWN);

        // If the level did not change, the sensor was already at minimum
        if (target->getRawLvl() == lvl)
        {
            invalidAction = true;
            D_println("INVALID ACTION: SENSOR ALREADY AT MIN LEVEL");
        }

        break;
    }

    default:
    {
        // Do nothing
        D_println("ACTION TYPE: DO NOTHING");
        break;
    }
    }

    // ================= OBSERVATION =================
    SensorObservation o = sensorController->collectData();

    /*
    for (SensorState sensor : o.sensorStateData)
    {
        D_println("SENSOR NAME: " + String(sensor.name));
        D_println("SENSOR RAW LVL: " + String(sensor.rawLvl));
        D_println("SENSOR SCALED LVL: " + String(sensor.scaledLvl));
        D_println("SENSOR INTERESTINGNESS: " + String(sensor.interestingness));
        D_println("SENSOR COST: " + String(sensor.flatCost));
    }
    */

    // ================= STATE =================

    State s = stateCalculator.calculate(
        o.sensorStateData,
        (energyLeftInEpisode / energyPerEpisode),
        msLeftInEpisode,
        energyLeftInEpisode);
    
    /*
    D_println("UNDER SAMPLE LVL: " + String(s.maxUnderSample));
    D_println("OVER SAMPLE LVL: " + String(s.maxOverSample));
    D_println("BATTERY LVL: " + String(s.currentBattery));

    D_println("ENERGY REMAINING: " + String(s.energyRemaining));
    D_println("TIME REMAINING: " + String(s.timeRemaining));
    */

    energyLeftInEpisode = s.energyRemaining;
    msLeftInEpisode = s.timeRemaining;

    // ================= REWARD =================

    float r = rewardCalculator.calculate(
        o.sensorStateData,
        conservatism,
        (energyLeftInEpisode / energyPerEpisode),
        s.epState,
        invalidAction);

    D_println("REWARD: " + String(r));
    
    // ================= OUTPUT =================
    OutputData op;

    op.energyRemaining = energyLeftInEpisode;
    op.msRemaining = msLeftInEpisode;

    for (SensorState sensor : o.sensorStateData) {
        if ((sensor.name).equals("inertia")) op.inertiaLvl = sensor.rawLvl;
        if ((sensor.name).equals("airQuality")) op.airQLvl = sensor.rawLvl;
        if ((sensor.name).equals("light")) op.lightLvl = sensor.rawLvl;
    }

    // ================= FEEDBACK =================

    Feedback f;

    f.isDone = (s.epState != INCOMPLETE);
    f.newState = s.encodedState;
    f.reward = r;

    f.outputData = op;

    prevState = s;

    D_println("========== END ENV STEP ==========");
    D_println("");

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
