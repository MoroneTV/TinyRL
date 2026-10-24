#ifndef SENSORDATATYPES_H
#define SENSORDATATYPES_H

#include <Arduino.h>

struct LightSensorReadings
{
    uint16_t clear;
    uint16_t red;
    uint16_t blue;
    uint16_t green;

    float combined; // Mean of above 4
    unsigned long timestamp;
};

struct InertiaSensorReadings
{

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

    unsigned long timestamp;
};

struct ClimateSensorReadings
{
    float temperature;
    float humidity;
    float pressure;

    unsigned long timestamp;
};

struct PowerSensorReadings
{

    float current;
    float batteryLvl;

    unsigned long timestamp;
};

struct SensorReadings
{
    // Array of readings, with pointer to where they stop.
    LightSensorReadings *lightReadings = nullptr;
    int lightSensorPtr = 0;

    InertiaSensorReadings *inertiaReadings = nullptr;
    int inertiaSensorPtr = 0;

    ClimateSensorReadings *climateReadings = nullptr;
    int climateSensorPtr = 0;

    // Power sensor is always on and cannot be configured, unlike above three
    PowerSensorReadings *powerReadings = nullptr;
    int powerSensorPtr = 0;
};

// Information about sensors- power level, interestingness values, etc.
struct SensorMetadata
{
    // Identifiers & State
    String sensorName;
    int sensorID;
    bool isEnabled;

    // Each sensor has discrete levels for power usage. Number of levels depends on sensor.
    int sensorMaxLvl;
    int SensorCurrentLvl;

    // Reward function vars
    float sensorScaledLvl; // Can be between [0,1]
    float interestingness; // Calculation depends on sensor. [0,1] Range.
    float sensorWeight;    // Scales the perceived value of sensor.
};

struct SensorData
{
    SensorReadings sensorReadings;
    SensorMetadata sensorMetadata;
};

enum LightPowerMode {
    SAMPLE_160ms = 4,
    SAMPLE_320ms = 3,
    SAMPLE_640ms = 2,
    SAMPLE_1280ms = 1,
    SAMPLE_2560ms = 0,
};

enum InertiaPowerMode {
    HZ_7p5 = 0,
    HZ_15 = 1,
    HZ_60 = 2,
    HZ_120 = 3,
    HZ_240 = 4
};

#endif