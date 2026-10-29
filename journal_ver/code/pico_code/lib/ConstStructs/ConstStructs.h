#ifndef CONSTSTRUCTS_H
#define CONSTSTRUCTS_H

struct LightSensorConfig {
    int weight = 1;                 // Scales the perceived value of each reading in timestep.
    int readingBufferLength = 10;   // Max number of readings in buffer, default 10.
};

struct InertialSensorConfig {
    int weight = 1;                 // Scales the perceived value of each reading in timestep.
    int readingBufferLength = 200;   // Max number of readings in buffer, default 200.
};

struct ClimateSensorConfig {
    int weight = 1;                 // Scales the perceived value of each reading in timestep.
    int readingBufferLength = 10;   // Max number of readings in buffer, default 10.
    
    // Value in degrees C where interestingness hits 1.00
    float upperTempBound = 30;
    float lowerTempBound = 5;
};

struct SensorConfig {
    LightSensorConfig light;
    InertialSensorConfig inertia;
    ClimateSensorConfig climate;
};
SensorConfig sensorConfig;

enum ControllerType {
    RL,
    MANUAL,
    ALGORITHMIC
};

enum OperationMode {
    SHOWCASE,
    TRAINING
};

struct SystemConfig {
    SensorConfig sensorConfig;
    ControllerType controllerType;
    OperationMode operationMode;
    float conservation;
};

#endif