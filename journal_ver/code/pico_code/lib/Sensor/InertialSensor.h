#ifndef INERTIALSENSOR_H
#define INERTIALSENSOR_H

#include "Arduino.h"
#include "Sensor.h"
#include <tuple>
#include <vector>
#include "SensorReadingBuffer.h"
#include "SparkFun_LSM6DSV16X.h"

class InertialSensor: public Sensor {
public:
    InertialSensor(
        int idNo,
        float weight = 0.0f,
        int readingBufferLen = 200) : readingBuffer(readingBufferLen), sensorWeight(weight)
    {
        id = std::make_tuple("inertia", idNo);
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

    // Gets all readings from buffer that are from after the provided timestamp.
    std::vector<InertiaSensorReadings> getNewReadings(unsigned long timestamp)
    {
        std::vector<InertiaSensorReadings> readings;
        int lastIdx = readingBuffer.getReadingCount() - 1;

        for (int i = 0; i < lastIdx; i++) {
            InertiaSensorReadings reading;
            if (readingBuffer.getReading(i, reading) && reading.timestamp > timestamp) {
                readings.push_back(reading);
            }
        }

        return readings;
    }



private:
    SparkFun_LSM6DSV16X lsm; // Sensor being wrapped

    // Name | Id number
    std::tuple<String, int> id;

    // Max Lvl | Current Lvl | Scaled Lvl [0,1] | Sample Interval in MS
    std::tuple<int, int, float, int> powerSettings;

    // If weight = 0, Sensor will not be enabled.
    bool isEnabled = false;
    bool isGyroEnabled;
    float sensorWeight;

    // Readings & Interestingness
    SensorReadingBuffer<InertiaSensorReadings> readingBuffer;
    int lastSampleTime;
    float calculateInterestingness();

    // Helper methods for updating sensor.
    void updatePowerSettings(InertiaPowerMode lvl);
    bool relativeUpdate(Direction dir);
    bool absoluteUpdate(int value);

        // OFFSETS AND CALIBRATION
    float aXoffset = 0.0f;                       
    float aYoffset = 0.0f;
    float aZoffset = 0.0f;
    float gXoffset = 0.0f;
    float gYoffset = 0.0f;
    float gZoffset = 0.0f;
    void calibrate();       // Generates offset values. Ensure the device is still for 2 seconds!


    /*
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
            case HZ_7p5:  costPerTimestep = 1.0f; break;
            case HZ_15:   costPerTimestep = 1.5f; break;
            case HZ_60:   costPerTimestep = 3.0f; break;
            case HZ_120:  costPerTimestep = 5.0f; break;
            case HZ_240:  costPerTimestep = 8.0f; break;
        }
    }

    // STATE
    bool status = false; // Is sensor on
    void setStatus(bool s) { status = s; }
    InertiaPowerLevel powerMode;
    float scaledLevel;      // sensor powermode mapped to be between 0 and 1 for reward calculations
    float costPerTimestep;  // How many virtual energy points this sensor costs per timestep, based on powermode
    float weight;           // How much does the agent prioritse this sensor? should be between 0 and 1.
    float interestingness;  // How much movement is happening based on the buffer variance?
    bool idAssigned = false;
    int id = -1;                 // Numerical ID for sensor- used for the agent to be able to index it.
    float flatMaxCost = 32;     // Maximum cost the sensor can incur during a timestep. used to normalise flatcost.
    String name = "inertia"; // Sensor name, primarily used for debugging;

    */
};

#endif // INERTIALSENSOR_H
