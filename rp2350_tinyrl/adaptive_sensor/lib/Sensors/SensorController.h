#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <Arduino.h>
#include <config.h>
#include <InertialSensor.h>
#include <SensorWeights.h>

class SensorController
{
public:
    SensorController(SensorWeights weights) : inertia(new InertialSensor(HZ_7p5, weights.getInertiaW())) {}
    bool begin();

private:
    InertialSensor* inertia;
};

#endif