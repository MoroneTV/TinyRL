#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "Arduino.h"
#include "Sensor.h"
#include "BH1745NUC.h" // Light Sensor to be Wrapped.
#include <tuple>
#include "SensorReadingBuffer.h"

class LightSensor : public Sensor
{
public:
    LightSensor(
        int idNo,
        float weight = 0.0f,
        int readingBufferLen = 10) : readingBuffer(readingBufferLen), sensorWeight(weight)
    {
        id = std::make_tuple("light", idNo);
    }

    virtual bool initialise();
    virtual void update(unsigned long now);

    virtual bool updateSettings(ControllerInstruction instructions); // Update sensor Lvl
    virtual SensorMetadata getMetadata()
    {
        SensorMetadata md;

        md.sensorName = std::get<0>(id);
        md.sensorID = std::get<1>(id);

        md.isEnabled = isEnabled;
        md.sensorWeight = sensorWeight;

        md.sensorMaxLvl = std::get<0>(powerSettings);
        md.SensorCurrentLvl = std::get<1>(powerSettings);
        md.sensorScaledLvl = std::get<2>(powerSettings);

        md.interestingness = calculateInterestingness();

        return md;
    }

    virtual int getID() {return std::get<1>(id);}

    // Gets all readings from buffer that are from after the provided timestamp.
    SensorReadings getNewReadings(unsigned long timestamp, SensorReadings& r)
    {
        std::vector<LightSensorReadings> readings;
        int lastIdx = readingBuffer.getReadingCount() - 1;

        for (int i = 0; i < lastIdx; i++) {
            LightSensorReadings reading;
            if (readingBuffer.getReading(i, reading) && reading.timestamp > timestamp) {
                readings.push_back(reading);
            }
        }

        if(readings.empty())
        {
            return r;
        }
        
        r.newLightReadings = true;
        r.lightReadings = readings;
        return r;
    }

private:
    BH1745NUC bh17;

    // Name | Id number
    std::tuple<String, int> id;

    // Max Lvl | Current Lvl | Scaled Lvl [0,1] | Sample Interval in MS
    std::tuple<int, int, float, int> powerSettings;

    // If weight = 0, Sensor will not be enabled.
    bool isEnabled = false;
    float sensorWeight;

    // Readings & Interestingness
    SensorReadingBuffer<LightSensorReadings> readingBuffer;
    int lastSampleTime;
    float calculateInterestingness();

    // Helper methods for updating sensor.
    void updatePowerSettings(LightPowerMode lvl);
    bool relativeUpdate(Direction dir);
    bool absoluteUpdate(int value);
};

#endif // LIGHTSENSOR_H
