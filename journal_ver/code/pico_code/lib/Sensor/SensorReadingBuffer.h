#ifndef SENSORREADINGBUFFER_H
#define SENSORREADINGBUFFER_H

#include "SensorDatatypes.h"
#include <Arduino.h>
#include <vector>

template <typename ReadingType>
class SensorReadingBuffer
{

public:
    SensorReadingBuffer(int len) : len(len), readingBuffer(len > 0 ? len : 0) {
        
        // Catch bad buffer length error.
        if (len <= 0) {
            while (true)
            {
                Serial.println("sensor reading buffer must have a length greater than 0!");
                delay(1000);
            }
        }
    }

    bool addReading(const ReadingType &reading)
    {
        if (len == 0)
            return false;

        readingBuffer[nextReadingIndex] = reading;
        nextReadingIndex = (nextReadingIndex + 1) % len;
        if (readingCount < len)
            ++readingCount;
        return true;
    }

    // Index 0 returns the oldest available reading.
    bool getReading(int index, ReadingType &reading)
    {
        if (index < 0 || index >= readingCount)
            return false;

        const int oldestIndex = (nextReadingIndex - readingCount + len) % len;
        reading = readingBuffer[(oldestIndex + index) % len];
        return true;
    }

    int getReadingCount()
    {
        return readingCount;
    }

    void clearReadings()
    {
        nextReadingIndex = 0;
        readingCount = 0;
    }

private:
    int len;
    std::vector<ReadingType> readingBuffer;
    int nextReadingIndex = 0;
    int readingCount = 0;
};

using LightSensorReadingBuffer = SensorReadingBuffer<LightSensorReadings>;
using InertiaSensorReadingBuffer = SensorReadingBuffer<InertiaSensorReadings>;
using ClimateSensorReadingBuffer = SensorReadingBuffer<ClimateSensorReadings>;
using PowerSensorReadingBuffer = SensorReadingBuffer<PowerSensorReadings>;

#endif