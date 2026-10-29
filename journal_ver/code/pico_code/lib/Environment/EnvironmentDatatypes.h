#ifndef ENVIRONMENTDATATYPES_H
#define ENVIRONMENTDATATYPES_H

#include "SensorDatatypes.h"

// Battery state, energy usage
struct EnvironmentVars {

    float powerAtStart; // Power lvl at beginning of timestep  
    float powerAtEnd;   // Power lvl at end of timestep
    float powerDelta;   // Power loss over timestep

    float batteryLvl;       // [0,1] Range
};  

// Final object that is passed to the controller
struct EnvironmentData {
    SensorData sensorData;
    EnvironmentVars environmentVars;
}; 

#endif