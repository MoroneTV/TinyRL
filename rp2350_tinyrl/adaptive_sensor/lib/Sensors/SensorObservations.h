#ifndef SENSOR_OBSERVATION_H
#define SENSOR_OBSERVATION_H

#include <Arduino.h>
#include <vector>
#include <InertialSensor.h>

// Fields that we can expect all sensors to have, making it iterable
struct SensorState {
    int sensorIdx;
    String sensorName = "NULL";
    bool isEnabled = false;

    float weight;
    float interestingness;
    float scaledLvl;
    int rawLvl;
    float FlatCost;
};

struct SensorObservation {
    std::vector<SensorState
}






#endif