#ifndef CLIMATESENSOR_H
#define CLIMATESENSOR_H

#include "Sensor.h"
#include "bme68xLibrary.h"
#include <tuple>
#include "SensorReadingBuffer.h"

class ClimateSensor : public Sensor
{
public:
    ClimateSensor(
        int idNo,
        float weight = 0.0f,
        int readingBufferLen = 10,
        float upperTempBound = 30,
        float lowerTempBound = 5
        ) : readingBuffer(readingBufferLen), sensorWeight(weight),
        upperBound(upperTempBound), lowerBound(lowerTempBound)
    {
        id = std::make_tuple("climate", idNo);
    }

    virtual bool initialise();
    virtual void update(unsigned long now);

    virtual int getID() {return std::get<1>(id);}

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

    // Gets all readings from buffer that are from after the provided timestamp.
    SensorReadings getNewReadings(unsigned long timestamp, SensorReadings& r)
    {
        std::vector<ClimateSensorReadings> readings;
        int lastIdx = readingBuffer.getReadingCount() - 1;

        for (int i = 0; i < lastIdx; i++) {
            ClimateSensorReadings reading;
            if (readingBuffer.getReading(i, reading) && reading.timestamp > timestamp) {
                readings.push_back(reading);
            }
        }

        if(readings.empty())
        {
            return r;
        }
        
        r.newClimateReadings = true;
        r.climateReadings = readings;
        return r;
    }


private:
    Bme68x bme;

    // Name | Id number
    std::tuple<String, int> id;

    // Max Lvl | Current Lvl | Scaled Lvl [0,1] | Sample Interval in MS
    std::tuple<int, int, float, int> powerSettings;

    // If weight = 0, Sensor will not be enabled.
    bool isEnabled = false;
    float sensorWeight;

    // Readings & Interestingness
    SensorReadingBuffer<ClimateSensorReadings> readingBuffer;
    int lastSampleTime;
    float calculateInterestingness();

    // Helper methods for updating sensor.
    void updatePowerSettings(ClimatePowerMode lvl);
    bool relativeUpdate(Direction dir);
    bool absoluteUpdate(int value);

    // Min and max temperature bounds where interestingness reaches max.
    float lowerBound;
    float upperBound;

};

#endif // AIRQUALITYSENSOR_H
