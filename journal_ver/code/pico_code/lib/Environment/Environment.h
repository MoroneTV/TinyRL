#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "EnvironmentDatatypes.h"

class Environment {

    public:
        Environment() {};
        ~Environment() {};

        EnvironmentData step(SensorData sensorData);  // Takes in sensor data, updates environment state, and returns new environment data

    private:

};

#endif