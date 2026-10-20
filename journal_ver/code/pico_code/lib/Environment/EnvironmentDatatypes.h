#ifndef ENVIRONTMENTDATATYPES_H
#define ENVIRONTMENTDATATYPES_H

struct SensorReadings {};   // Actual sensor readings, like temperature or light
struct SensorMetadata {};   // Information about sensors- power level, interestingness values, etc.

struct SensorData {
    SensorReadings sensorReadings;
    SensorMetadata sensorMetadata;
};

struct EnvironmentVars {};  // Battery state, energy usage

// Final object that is passed to the controller
struct EnvironmentData {
    SensorData sensorData;
    EnvironmentVars environmentVars;
}; 

#endif