#ifndef SENSORDATATYPES_H
#define SENSORDATATYPES_H

#include <Arduino.h>

struct LightSensorReadings {
    float clear;
    float blue;
    float red;
    float green;

    float combined; // Mean of above 4
    int timestamp;
};

struct InertiaSensorReadings {

    // Accelerometer
    float accelX;
    float accelY;
    float accelZ;
    float accelMag; 

    // Gyroscope (if enabled)
    float gyroX;
    float gyroY;
    float gyroZ;
    float gyroMag; 
    bool gyroEnabled = false;

    int timestamp;
};

struct ClimateSensorReadings {

    float temperature;
    float humidity;
    float pressure;

    int timestamp;
};


struct SensorReadings {
    LightSensorReadings lightReadings;
    InertiaSensorReadings inertiaReadings;
    ClimateSensorReadings climateReadings;
};   

// Information about sensors- power level, interestingness values, etc.
struct SensorMetadata {

    // Identifiers & State
    String sensorName;
    int sensorID;
    bool isEnabled;

    // Each sensor has discrete levels for power usage. Number of levels depends on sensor.
    int sensorMaxLvl;       
    int SensorCurrentLvl;

    // Reward function vars
    float sensorScaledLvl;  // Can be betwen [0,1],
    float interestingness;  // Calculation depends on sensor. [0,1] Range.
    float sensorWeight;     // Scales the perceived value of sensor readings.
};   

struct SensorData {
    SensorReadings sensorReadings;
    SensorMetadata sensorMetadata;
};

#endif