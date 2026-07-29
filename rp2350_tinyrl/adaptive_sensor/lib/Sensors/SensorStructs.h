#ifndef SENSOR_STRUCTS_H
#define SENSOR_STRUCTS_H

#include <Arduino.h>
#include <vector>

// Inertia Sensor Structs
struct InertiaReading {
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
};

enum InertiaPowerLevel {
    HZ_7p5 = 0,
    HZ_15 = 1,
    HZ_60 = 2,
    HZ_120 = 3,
    HZ_240 = 4
};

// Air Quality Sensor Structs
struct AirQualityReading {
    float temperature;
    float pressure;
    float humidity;
    float gasResistance;
    uint32_t timestamp;
};

enum AirQualityPowerLevel {
    SAMPLE_30s = 0,
    SAMPLE_15s = 1,
    SAMPLE_10s = 2,
    SAMPLE_5s = 3,
    SAMPLE_1s = 4
};

// Fields that we can expect all sensors to have, making it iterable
struct SensorState {
    int sensorId;
    String sensorName = "NULL";
    bool isEnabled = false;

    float weight;
    float interestingness;
    float scaledLvl;
    int rawLvl;
    float flatCost;
    float maxFlatCost;
};

struct SensorObservation {
    // State data, used for guiding program flow and updating the model
    std::vector<SensorState> sensorStateData;

    // Individual readings, goes to output sources like screen, SD, or network.
    InertiaReading* inertiaReadings;
    int inertiaReadingPtr;

    AirQualityReading airQualityReading;
};

// Increase or decrease sensor lvl?
enum SensorDir {
    UP = 0,
    DOWN = 1
};


#endif