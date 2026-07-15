#include "InertiaModulator.h"

void InertiaModulator::begin()
{
    sensor.begin();
}

InertiaFeedback InertiaModulator::agentAction(int action)
{

    // Agent updates the precison level of the sensor (If it isn't the same)
    if (action != sensor.getPrecisionLvl())
        sensor.updatePrecision(action);

    // Get new readings after updating the sensor. This takes 1 second.
    fillBuffer();

    // Process buffer readings to get the variance, also add the precision
    InertiaFeedback feedback = processBuffer();
    feedback.precisionLvl = action;

    // Clip variance into different thresholds between 0 and 2
    feedback.varianceLvl = getBoundVariance(feedback.varianceRaw);

    // Update the battery level and time left
    energyRemaining -= energyCosts[action];
    timeLeftMs = episodeLength - (millis() - startTime);

    feedback.minsLeftInEp = String(int(timeLeftMs / 1000 / 60));
    feedback.energyPercentLeft = String(int((energyRemaining / 10000) * 100));

    // Calculate reward
    feedback.reward = calculateReward(feedback.varianceLvl, action);

    // Check if the episode should be over;
    if (timeLeftMs <= 0 || energyRemaining <= 0)
        feedback.isDone = true;

    // State is a combination of the variance level (3 exist), and the energy remaining (10 bins), for 30 states
    feedback.newState = getDiscreteEnergyLvl(energyRemaining) * 3 + feedback.varianceLvl;

    return feedback;
}

float InertiaModulator::calculateReward(int varianceLvl, int action)
{

    float reward = 0.0f;

    bool isEnergyDepleted = (energyRemaining <= 0);
    bool isOutOfTime = (timeLeftMs <= 0);

    // If battery is empty and time isn't over, big punishment
    if (isEnergyDepleted && !isOutOfTime)
        reward -= 200;

    // If time is over and battery isn't empty, big reward
    if (isOutOfTime && !isEnergyDepleted)
        reward += 200;

    // Both variance and action have 3 possible choices, and the best option for each is
    // a 1-1 mapping (lowest variance, lowest action), so absolute difference works.
    reward += 2 - abs(varianceLvl - action);

    // If the energy level is below the time level (e.g. 80% time left but 10% energy left)
    // punish the agent with a a punishment relative to the level of the deficit, to discourage wasting battery.
    float energyFrac = energyRemaining / maxEnergy;
    float timeFrac = timeLeftMs / episodeLength;
    if (energyFrac < timeFrac)
    {
        reward -= 5.0f * (timeFrac - energyFrac);
    }

    return reward;
}

int InertiaModulator::getBoundVariance(float rawVar)
{
    int varLvl;
    if (rawVar <= 0.5)
        varLvl = 0;
    else if (rawVar > 0.5 && rawVar <= 5)
        varLvl = 1;
    else
        varLvl = 2;

    return varLvl;
}

void InertiaModulator::fillBuffer()
{

    // Clear the buffer, then fill it again with new values
    resetBuffer();

    // Collect readings for 1 second (1000ms)
    int start = millis();
    while (millis() - start < 1000)
    {
        rBuffer[rIdx] = sensor.read();
        rIdx++;

        // how often readings should be taken depends on the sensor Hz.
        delay(sensor.getTimeBetweenReadings());
    }
}

void InertiaModulator::resetBuffer()
{
    // All non-empty values in reading buffer (up to rIdx) set to 0
    for (int i = 0; i < rIdx; i++)
        rBuffer[i] = InertiaReadings{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    rIdx = 0;
}

InertiaFeedback InertiaModulator::processBuffer()
{

    InertiaFeedback feedback;

    // Maginitudinal mean and individual means
    float mean = 0.0f;
    float avgAX = 0.0f;
    float avgAY = 0.0f;
    float avgAZ = 0.0f;
    float avgGX = 0.0f;
    float avgGY = 0.0f;
    float avgGZ = 0.0f;

    for (int i = 0; i < rIdx; i++)
    {
        float accelMag = sqrt(pow(rBuffer[i].aX, 2) + pow(rBuffer[i].aY, 2) + pow(rBuffer[i].aZ, 2));
        mean += accelMag;
        avgAX += rBuffer[i].aX;
        avgAY += rBuffer[i].aY;
        avgAZ += rBuffer[i].aZ;

        // Only get these if Gyro is enabled
        if (sensor.getPrecisionLvl() <= 2)
        {
            avgGX += rBuffer[i].gX;
            avgGY += rBuffer[i].gY;
            avgGZ += rBuffer[i].gZ;
        }
    }

    // Calculate Averages
    mean /= rIdx;
    avgAX /= rIdx;
    avgAY /= rIdx;
    avgAZ /= rIdx;
    avgGX /= rIdx;
    avgGY /= rIdx;
    avgGZ /= rIdx;

    // Fill feedback struct with averages
    feedback.avgAX = String(avgAX);
    feedback.avgAY = String(avgAY);
    feedback.avgAZ = String(avgAZ);

    // Only get these if Gyro is enabled
    if (sensor.getPrecisionLvl() <= 2)
    {
        feedback.avgGX = String(avgGX);
        feedback.avgGY = String(avgGY);
        feedback.avgGZ = String(avgGZ);
    }

    // Calculate Magnitudnial Variance
    float variance = 0.0f;
    for (int i = 0; i < rIdx; i++)
    {
        float accelMag = sqrt(pow(rBuffer[i].aX, 2) + pow(rBuffer[i].aY, 2) + pow(rBuffer[i].aZ, 2));
        variance += pow(accelMag - mean, 2);
    }
    variance /= rIdx;
    feedback.varianceRaw = variance;

    return feedback;
}

int InertiaModulator::resetEnvironment()
{
    resetBuffer();
    sensor.resetToDefaults();
    startTime = millis();
    episodeLength = random((20 * 1000 * 60), (40 * 1000 * 60)); // 20-40 Mins
    timeLeftMs = episodeLength;
    energyRemaining = 10000;

    // variance lvl (0 at start)
    return getDiscreteEnergyLvl(energyRemaining) * 3;
}

int InertiaModulator::getDiscreteEnergyLvl(float energyLevel)
{
    int energyBin;

    if (energyLevel <= 1000.0f)
        energyBin = 0;
    else if (energyLevel <= 2000.0f)
        energyBin = 1;
    else if (energyLevel <= 3000.0f)
        energyBin = 2;
    else if (energyLevel <= 4000.0f)
        energyBin = 3;
    else if (energyLevel <= 5000.0f)
        energyBin = 4;
    else if (energyLevel <= 6000.0f)
        energyBin = 5;
    else if (energyLevel <= 7000.0f)
        energyBin = 6;
    else if (energyLevel <= 8000.0f)
        energyBin = 7;
    else if (energyLevel <= 9000.0f)
        energyBin = 8;
    else
        energyBin = 9;

    return energyBin;
}