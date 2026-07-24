#ifndef SENSOR_OBSERVATION_H
#define SENSOR_OBSERVATION_H

#include <Arduino.h>
#include <vector>
#include <InertialSensor.h>

// Fields that we can expect all sensors to have, making it iterable
struct SensorState {
    int sensorId;
    String sensorName = "NULL";
    bool isEnabled = false;

    float weight;
    float interestingness;
    float scaledLvl;
    int rawLvl;
    float flatCost;
};

struct SensorObservation {
    // State data, used for guiding program flow and updating the model
    std::vector<SensorState> sensorStateData;

    // Individual readings, goes to output sources like screen, SD, or network.
    InertiaReading* inertiaReadings;
    int inertiaReadingPtr;
};


#endif