#ifndef INERTIALSENSOR_H
#define INERTIALSENSOR_H

#include <unordered_map>
#include <SparkFun_LSM6DSV16X.h>
#include <config.h>

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

class InertialSensor {
public:
    InertialSensor(InertiaPowerLevel powerMode = HZ_7p5) : powerMode(powerMode) {}

    bool begin();
    void update(int now); // Add reading to buffer if necessary, takes time in ms

    InertiaReading* getBuffer() { return buffer; }  // Returns buffer of readings. Max size 250.
    int getBufferPtr() { return bPtr; }             // Returns the ptr to the last meaningful reading in the buffer

    bool isEnabled()  { return status; }            // Is sensor on
    InertiaPowerLevel getPowerMode()  { return powerMode; }

    void updatePowerMode(InertiaPowerLevel lvl);   // 0-5 for each ODR level

    // Empty the buffer and reset the buffer pointer to 0
    void clearBuffer() { for (int i = 0; i < 250; ++i) buffer[i] = InertiaReading(); bPtr = 0; }

private:
    SparkFun_LSM6DSV16X lsm; // Sensor being wrapped

    // READINGS AND DATA MANAGEMENT
    InertiaReading buffer[250];         // Fastest mode is 240hz, so can contain 1 sec
    int bPtr;                           // Biggest idx that contains meaningful information
    int lastSampleTime;     // Time that the last sample was taken, ms
    int sampleInterval;     // How often to add samples to the buffer, in ms

    // Mapping of ODR (Hz) to time between readings (ms)
    std::unordered_map<InertiaPowerLevel,int> HzToMs = {
        {HZ_7p5, 133},   
        {HZ_15, 67},   
        {HZ_60, 17},
        {HZ_120, 8},
        {HZ_240, 4}    
    };
    void updateSampleInterval(InertiaPowerLevel lvl) { sampleInterval = HzToMs[lvl]; }

    // OFFSETS AND CALIBRATION
    float aXoffset = 0.0f;                       
    float aYoffset = 0.0f;
    float aZoffset = 0.0f;
    float gXoffset = 0.0f;
    float gYoffset = 0.0f;
    float gZoffset = 0.0f;
    void calibrate();       // Generates offset values. Ensure the device is still for 2 seconds!

    // STATE
    bool status = false; // Is sensor on
    void setStatus(bool s) { status = s; }
    InertiaPowerLevel powerMode;

};

#endif // INERTIALSENSOR_H
