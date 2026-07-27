#ifndef INERTIALSENSOR_H
#define INERTIALSENSOR_H

#include <SparkFun_LSM6DSV16X.h>
#include "config.h"
#include "Sensor.h"
#include "SensorStructs.h"

class InertialSensor: public Sensor {
public:
    InertialSensor(InertiaPowerLevel powerMode = HZ_7p5,
               float weight = 0.0f)
    : powerMode(powerMode),
      weight(weight),
      scaledLevel(0.0f),
      costPerTimestep(0.0f),
      interestingness(0.0f),
      lastSampleTime(0),
      sampleInterval(133)
{
    updateScaledLevel(powerMode);
    updateCostPerTimestep(powerMode);
    updateSampleInterval(powerMode);
}

    bool begin();
    void update(int now); // Add reading to buffer if necessary, takes time in ms
    // Empty the buffer and reset the buffer pointer to 0
    void clearBuffer() { for (int i = 0; i < 250; ++i) buffer[i] = InertiaReading(); bPtr = 0; }

    void setLvl(int lvl);   // 0-5 for each ODR level
    void setId(int id) { this->id = id; }

    // Class-specific getters
    InertiaReading* getBuffer() { return buffer; }  // Returns buffer of readings. Max size 250.
    int getBufferPtr() { return bPtr; }             // Returns the ptr to the last meaningful reading in the buffer

    // Inherited Sensor getters
    bool isEnabled()  { return status; }            // Is sensor on
    int getRawLvl()  { return powerMode; }
    float getWeight() { return weight; }
    float getInterestingness(); // Calculate and return 'interestingness' of data in buffer.
    float getScaledLvl() { return scaledLevel; }
    float getFlatCost() { return costPerTimestep; }
    int getId()   { return id; }
    String getName() { return name; }

    SensorState getState() {
        SensorState state;
        state.sensorId = id;
        state.sensorName = name;
        state.isEnabled = status;
        state.weight = weight;
        state.interestingness = getInterestingness();
        state.scaledLvl = scaledLevel;
        state.rawLvl = static_cast<int>(powerMode);
        state.flatCost = costPerTimestep;
        return state;
    }

private:
    SparkFun_LSM6DSV16X lsm; // Sensor being wrapped

    // READINGS AND DATA MANAGEMENT
    InertiaReading buffer[250];         // Fastest mode is 240hz, so can contain 1 sec
    int bPtr = 0;                           // Biggest idx that contains meaningful information
    int lastSampleTime;     // Time that the last sample was taken, ms
    int sampleInterval;     // How often to add samples to the buffer, in ms

    void updateSampleInterval(InertiaPowerLevel lvl) {
        switch (lvl) {
            case HZ_7p5: sampleInterval = 133; break;
            case HZ_15: sampleInterval = 67; break;
            case HZ_60: sampleInterval = 17; break;
            case HZ_120: sampleInterval = 8; break;
            case HZ_240: sampleInterval = 4; break;
            default: sampleInterval = 133; break;
        }
    }

    void updateScaledLevel(InertiaPowerLevel lvl) {
        switch (lvl) {
            case HZ_7p5: scaledLevel = 0.0f; break;
            case HZ_15: scaledLevel = 0.0625f; break;
            case HZ_60: scaledLevel = 0.25f; break;
            case HZ_120: scaledLevel = 0.5f; break;
            case HZ_240: scaledLevel = 1.0f; break;
            default: scaledLevel = 0.0f; break;
        }
    }

    void updateCostPerTimestep(InertiaPowerLevel lvl) {
        switch (lvl) {
            case HZ_7p5: costPerTimestep = 1.0f; break;
            case HZ_15: costPerTimestep = 2.0f; break;
            case HZ_60: costPerTimestep = 8.0f; break;
            case HZ_120: costPerTimestep = 16.0f; break;
            case HZ_240: costPerTimestep = 32.0f; break;
            default: costPerTimestep = 1.0f; break;
        }
    }

    // OFFSETS AND CALIBRATION
    float aXoffset = 0.0f;                       
    float aYoffset = 0.0f;
    float aZoffset = 0.0f;
    float gXoffset = 0.0f;
    float gYoffset = 0.0f;
    float gZoffset = 0.0f;
    void calibrate();       // Generates offset values. Ensure the device is still for 2 seconds!

    // STATE
    bool status = false; // Is sensor on
    void setStatus(bool s) { status = s; }
    InertiaPowerLevel powerMode;
    float scaledLevel;      // sensor powermode mapped to be between 0 and 1 for reward calculations
    float costPerTimestep;  // How many virtual energy points this sensor costs per timestep, based on powermode
    float weight;           // How much does the agent prioritse this sensor? should be between 0 and 1.
    float interestingness;  // How much movement is happening based on the buffer variance?
    int id = -1;                 // Numerical ID for sensor- used for the agent to be able to index it.
    String name = "inertia"; // Sensor name, primarily used for debugging;

};

#endif // INERTIALSENSOR_H
