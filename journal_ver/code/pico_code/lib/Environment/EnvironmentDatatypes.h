#ifndef ENVIRONMENTDATATYPES_H
#define ENVIRONMENTDATATYPES_H

#include "SensorDatatypes.h"

// Battery state, energy usage
struct EnvironmentVars {

    float currentBaseline;  // AVG current of system at rest
    float currentActual;    // Current current (haha)

    float batteryLvl;       // [0,1] Range
};  

// Final object that is passed to the controller
struct EnvironmentData {
    SensorData sensorData;
    EnvironmentVars environmentVars;
}; 

#endif