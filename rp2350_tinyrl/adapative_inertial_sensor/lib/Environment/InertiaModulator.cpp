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

    // Get new readings after updating the sensor
    fillBuffer();

    // Process buffer readings to get the variance, also add the precision
    InertiaFeedback feedback = processBuffer();

    // Clip variance into different thresholds between 0 and 2
    feedback.varianceLvl = getBoundVariance(feedback.varianceRaw);

    // Using an appropriate precision with an appropriate var should make the difference 0, so 2 reward.
    // Off by 1 is 2-1 reward, so 1 reward
    // Completely disparate is 2, so 2-2, 0 reward
    feedback.reward = 2 - abs(feedback.varianceLvl - action);

    // State is a combination of the precison level and the variance level
    feedback.newState = feedback.varianceLvl;

    return feedback;
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
    for (int i = 0; i <= rIdx; i++)
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
    runtimeMs = random((20 * 1000 * 60), (40 * 1000 * 60)); // 20-40 Mins
    energyRemaining = 10000;

    // variance lvl (0 at start)
    return 0;
}