#ifndef SENSORHUB_H
#define SENSORHUB_H

#include "Arduino.h"
#include <vector>

#include "ControllerDatatypes.h"
#include "EnvironmentDatatypes.h"

#include "ClimateSensor.h"
#include "LightSensor.h"
#include "InertialSensor.h"

#include "ConstStructs.h"

class SensorHub {

    public:
        SensorHub(SensorConfig config): config(config)
        {};

        ~SensorHub() {};
    
        void initialize();

        // Parses instructions, takes 1 second of data, and returns the sensor readings and metadata
        SensorData step(ControllerInstruction controllerInstruction);

    private:

        void applyInstruction(ControllerInstruction instruction);
        void readSensors();
        SensorData aggregateMetadata(SensorData &data);
        SensorData aggregateReadings(unsigned long since, SensorData &data);

        LightSensor* light;
        ClimateSensor* climate;
        InertialSensor* inertia;

        std::vector<Sensor*> sensors;

        SensorConfig config;

};

#endif