#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <Arduino.h>
#include <config.h>
#include <InertialSensor.h>
#include <SensorWeights.h>
#include <array>
#include <Sensor.h>
#include <AirQualitySensor.h>

class SensorController
{
public:
    SensorController(SensorWeights &weights) 
    :   inertia(HZ_7p5, weights.getInertiaW()), airQuality(SAMPLE_5s, weights.getAirQualityW()),
        sensors{&inertia, &airQuality} {}
        
    bool begin();

    // Collects 1 second's worth of information
    SensorObservation collectData();

    // Reset Sensors back to default levels
    void resetSensors();

    // Adjust sensor resolution either up or down one level.
    void updateSensorSettings(int id, SensorDir dir);

    Sensor *getSensorByID(int ID)
    {
        Sensor *target = nullptr;
        for (Sensor *s : sensors)
            if (s->getId() == ID)
                target = s;
        return target;
    }

private:
    InertialSensor inertia;
    AirQualitySensor airQuality;
    std::array<Sensor *, 2> sensors;
};

#endif