#ifndef AIRQUALITYSENSOR_H
#define AIRQUALITYSENSOR_H

#include "Sensor.h"
#include "SensorStructs.h"
#include "bme68xLibrary.h"
#include "debug.h"


class AirQualitySensor: public Sensor {
public:
    AirQualitySensor(int powerLevel = 0,
                    float weight = 0.0f)
    : powerMode(powerLevel),
      weight(weight),
      scaledLevel(0.0f),
      costPerTimestep(0.0f),
      interestingness(0.0f),
      status(false),
      idAssigned(false),
      id(-1),
      flatMaxCost(16.0f),
      name("airQuality")
    {
        updateScaledLevel(static_cast<AirQualityPowerLevel>(powerMode));
        updateCostPerTimestep(static_cast<AirQualityPowerLevel>(powerMode));
        updateSampleInterval(static_cast<AirQualityPowerLevel>(powerMode));
    }

    bool begin();
    void update(int now);
    void setLvl(int lvl);

    void setId(int id) {
        if (!idAssigned) {
            this->id = id;
            idAssigned = true;
        }
    }

    AirQualityReading getMostRecentReading() { return history[(head + historySize - 1) % historySize]; }

    static const int historySize = 30;
    AirQualityReading history[historySize];
    int head = 0;
    int count = 0;

    bool isEnabled()  { return status; }
    int getRawLvl()   { return powerMode; }
    float getWeight() { return weight; }
    float getInterestingness();
    float getScaledLvl() { return scaledLevel; }
    float getFlatCost() { return costPerTimestep; }
    float getFlatMaxCost() { return flatMaxCost; }
    int getId()       { return id; }
    String getName()  { return name; }

    SensorState getState() {
        SensorState state;
        state.sensorId = id;
        state.name = name;
        state.isEnabled = status;
        state.weight = weight;
        state.interestingness = getInterestingness();
        state.scaledLvl = scaledLevel;
        state.rawLvl = powerMode;
        state.flatCost = costPerTimestep;
        state.maxFlatCost = flatMaxCost;
        return state;
    }

private:

    Bme68x bme;

    int powerMode;
    float scaledLevel;
    float weight;
    float interestingness;
    bool status;
    bool idAssigned;
    int id;
    float flatMaxCost;
    String name;

    void setStatus(bool s) { status = s; }

    void updateSampleInterval(AirQualityPowerLevel lvl) {
        switch (lvl) {
            case SAMPLE_30s: sampleInterval = 30000; break;
            case SAMPLE_15s: sampleInterval = 15000; break;
            case SAMPLE_10s: sampleInterval = 10000; break;
            case SAMPLE_5s: sampleInterval = 5000; break;
            case SAMPLE_1s: sampleInterval = 1000; break;
            default: sampleInterval = 30000; break;
        }
    }
    int sampleInterval;
    int lastSampleTime;     // Time that the last sample was taken, ms

    void updateScaledLevel(AirQualityPowerLevel lvl) {
        switch (lvl) {
            case SAMPLE_30s: scaledLevel = 0.0f; break;
            case SAMPLE_15s: scaledLevel = 0.075; break;
            case SAMPLE_10s: scaledLevel = 0.1; break;
            case SAMPLE_5s: scaledLevel = 0.2; break;
            case SAMPLE_1s: scaledLevel = 1.0f; break;
            default: scaledLevel = 0.0f; break;
        }
    }

    void updateCostPerTimestep(AirQualityPowerLevel lvl) {
        switch (lvl) {
            case SAMPLE_30s: costPerTimestep = 1; break;
            case SAMPLE_15s: costPerTimestep = 2; break;
            case SAMPLE_10s: costPerTimestep = 3; break;
            case SAMPLE_5s: costPerTimestep = 6; break;
            case SAMPLE_1s: costPerTimestep = 30; break;
            default: costPerTimestep = 1.0f; break;
        }
    }
    float costPerTimestep;
};

#endif // AIRQUALITYSENSOR_H
