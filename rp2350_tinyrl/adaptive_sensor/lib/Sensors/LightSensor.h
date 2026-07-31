#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "Sensor.h"
#include "SensorStructs.h"

#include "BH1745NUC.h" // Light Sensor to be Wrapped.

class LightSensor: public Sensor {
public:
    LightSensor(int powerLevel = 0,
                float weight = 0.0f)
    : powerMode(powerLevel),
      scaledLevel(0.0f),
      weight(weight),
      interestingness(0.0f),
      costPerTimestep(0.0f),
      status(false),
      idAssigned(false),
      id(-1),
      flatMaxCost(8.0f),
      name("light")
    {
        updateScaledLevel(static_cast<LightPowerLevel> (powerMode));
        updateCostPerTimestep(static_cast<LightPowerLevel> (powerMode));
        updateSampleInterval(static_cast<LightPowerLevel> (powerMode));
    }

    bool begin();
    void update(int now);
    void setLvl(int lvl);

    void setStatus(bool status) { this->status = status; }
    bool getStatus() { return status; }

    void setId(int id) {
        if (!idAssigned) {
            this->id = id;
            idAssigned = true;
        }
    }

    LightReading getMostRecentReading() { return history[(head + historySize - 1) % historySize]; }

    static const int historySize = 5;
    LightReading history[historySize];
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
    BH1745NUC bh17;

    int powerMode;
    float scaledLevel;
    float weight;
    float interestingness;
    float costPerTimestep;
    bool status;
    bool idAssigned;
    int id;
    float flatMaxCost;
    String name;

    int sampleInterval;
    int lastSampleTime;

    void updateSampleInterval(LightPowerLevel lvl) {
        switch (lvl) {
            case SAMPLE_160ms: sampleInterval = 160; break;
            case SAMPLE_320ms: sampleInterval = 320; break;
            case SAMPLE_640ms: sampleInterval = 640; break;
            case SAMPLE_1280ms: sampleInterval = 1280; break;
            case SAMPLE_2560ms: sampleInterval = 2560; break;
            case SAMPLE_5120ms: sampleInterval = 5120; break;
        }
    }

    void updateScaledLevel(LightPowerLevel lvl) {
        switch (lvl) {
            case SAMPLE_160ms: scaledLevel = 1.0f; break;
            case SAMPLE_320ms: scaledLevel = 0.8f; break;
            case SAMPLE_640ms: scaledLevel = 0.6f; break;
            case SAMPLE_1280ms: scaledLevel = 0.4f; break;
            case SAMPLE_2560ms: scaledLevel = 0.2f; break;
            case SAMPLE_5120ms: scaledLevel = 0.0f; break;
        }
    }

    void updateCostPerTimestep(LightPowerLevel lvl) {
        switch (lvl) {
            case SAMPLE_160ms: costPerTimestep = 8; break;
            case SAMPLE_320ms: costPerTimestep = 5; break;
            case SAMPLE_640ms: costPerTimestep = 3; break;
            case SAMPLE_1280ms: costPerTimestep = 2; break;
            case SAMPLE_2560ms: costPerTimestep = 1.2; break;
            case SAMPLE_5120ms: costPerTimestep = 0.6; break;
        }
    }
};

#endif // LIGHTSENSOR_H
