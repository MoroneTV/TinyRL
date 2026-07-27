#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <Arduino.h>
#include <config.h>
#include <InertialSensor.h>
#include <SensorWeights.h>
#include <array>
#include <Sensor.h>

class SensorController
{
public:
    SensorController(SensorWeights& weights) : inertia(HZ_7p5, weights.getInertiaW()),
    sensors{&inertia} {}
    bool begin();

    // Collects 1 second's worth of information
    SensorObservation collectData();

    // Reset Sensors back to default levels
    void resetSensors();

    // Adjust sensor resolution either up or down one level.
    void updateSensorSettings(int id, SensorDir dir);

private:
    InertialSensor inertia;
    std::array<Sensor*, 1> sensors;
};

#endif