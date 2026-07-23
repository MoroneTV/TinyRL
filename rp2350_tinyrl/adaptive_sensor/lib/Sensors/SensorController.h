#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <Arduino.h>
#include <config.h>
#include <InertialSensor.h>
#include <SensorWeights.h>

class SensorController
{
public:
    SensorController(SensorWeights weights) : inertia(HZ_7p5, weights.getInertiaW(), 1) {}
    bool begin();

    // Collects 1 second's worth of information
    SensorObservation collectData();

    // Adjust sensor resolution either up or down one level.
    void updateSensorSettings(int id, int dir);

private:
    InertialSensor inertia;
};

#endif