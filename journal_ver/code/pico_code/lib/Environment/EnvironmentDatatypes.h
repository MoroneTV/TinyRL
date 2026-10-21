#ifndef ENVIRONMENTDATATYPES_H
#define ENVIRONMENTDATATYPES_H

#include <Arduino.h>



struct EnvironmentVars {};  // Battery state, energy usage

// Final object that is passed to the controller
struct EnvironmentData {
    SensorData sensorData;
    EnvironmentVars environmentVars;
}; 

#endif