#ifndef SENSORDATATYPES_H
#define SENSORDATATYPES_H

#include <Arduino.h>
#include <vector>

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
    float gas_resistance;

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
    bool newLightReadings = false;
    std::vector<LightSensorReadings> lightReadings;

    bool newInertiaReadings = false;
    std::vector<InertiaSensorReadings> inertiaReadings;

    bool newClimateReadings = false;
    std::vector<ClimateSensorReadings> climateReadings;
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
    std::vector<SensorMetadata> sensorMetadata;
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

enum ClimatePowerMode {
    SAMPLE_30s = 0,
    SAMPLE_15s = 1,
    SAMPLE_10s = 2,
    SAMPLE_5s = 3,
    SAMPLE_1s = 4
};

#endif