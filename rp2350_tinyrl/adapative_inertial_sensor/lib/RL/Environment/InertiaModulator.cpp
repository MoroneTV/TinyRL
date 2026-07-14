#include "InertiaModulator.h"

InertiaFeedback InertiaModulator::agentAction(int action)
{

    // Agent updates the precison level of the sensor (If it isn't the same)
    if (action != sensor.getPrecisionLvl())
        sensor.updatePrecision(action);

    // Get new readings after updating the sensor
    fillBuffer();

    // Process buffer readings to get the variance, also add the precision
    InertiaFeedback feedback = processBuffer();
    feedback.precisionLvl = sensor.getPrecisionLvl();

    // Clip variance into different thresholds
    int varLvl;
    if (feedback.varianceRaw)
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

        // how often readings should be taken depends on the sensor Hz.
        delay(sensor.getTimeBetweenReadings());
    }
}

void InertiaModulator::resetBuffer()
{
    // All non-empty values in reading buffer (up to maxIdx) set to 0
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
            avgGX += rBuffer[i].aX;
            avgGY += rBuffer[i].aX;
            avgGZ += rBuffer[i].aX;
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

void InertiaModulator::resetEnvironment()
{
    resetBuffer();
    sensor.resetToDefaults();
}