#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <Arduino.h>
#include <config.h>
#include <InertialSensor.h>
#include <SensorWeights.h>
#include <array>
#include <Sensor.h>
#include <AirQualitySensor.h>
#include <LightSensor.h>

class SensorController
{
public:
    SensorController(SensorWeights &weights) 
    :   inertia(HZ_7p5, weights.getInertiaW()), airQuality(SAMPLE_5s, weights.getAirQualityW()), light(SAMPLE_5120ms, weights.getLightW()),
        sensors{&inertia, &airQuality, &light} {}
        
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

    // Lookup sensor by its name. Returns nullptr if not found.
    Sensor *getSensorByName(String name)
    {
        Sensor *target = nullptr;
        for (Sensor *s : sensors)
            if (s->getName() == name)
                target = s;
        return target;
    }

private:
    InertialSensor inertia;
    AirQualitySensor airQuality;
    LightSensor light;
    std::array<Sensor *, 3> sensors;
};

#endif