#ifndef INERTIALSENSOR_H
#define INERTIALSENSOR_H

#include <unordered_map>
#include <SparkFun_LSM6DSV16X.h>

struct InertiaReading {
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
};

class InertialSensor {
public:
    InertialSensor();

    bool begin();
    void update();

    InertiaReading* getBuffer() { return buffer; }  // Returns buffer of readings. Max size 250.
    int getBufferPtr() { return bPtr; }             // Returns the ptr to the last meaningful reading in the buffer

    bool isEnabled()  { return status; }          // Is sensor on
    bool isGyroEnabled() { return gyroEnabled; } // Is gyroscope enabled
    int getPowerMode()  { return powerMode; }

    void toggleGyros(bool toggle);
    void setODR(int lvl);   // 0-5 for each ODR level

    // Empty the buffer and reset the buffer pointer to 0
    void clearBuffer() { for (int i = 0; i < 250; ++i) buffer[i] = InertiaReading(); bPtr = 0; }

private:
    SparkFun_LSM6DSV16X lsm; // Sensor being wrapped

    // READINGS AND DATA MANAGEMENT
    InertiaReading buffer[250];         // Fastest mode is 240hz, so can contain 1 sec
    int bPtr;                           // Biggest idx that contains meaningful information

    // Mapping of ODR (Hz) to time between readings (ms)
    std::unordered_map<int, int> delayInterval = {
        {7, 133},   
        {15, 67},   
        {30, 33}, 
        {60, 17},
        {120, 8},
        {240, 4}    
    };  

    // OFFSETS AND CALIBRATION
    float aXoffset = 0.0f;                       
    float aYoffset = 0.0f;
    float aZoffset = 0.0f;
    float gXoffset = 0.0f;
    float gYoffset = 0.0f;
    float gZoffset = 0.0f;

    // STATE
    bool status = false; // Is sensor on
    bool gyroEnabled = false;  // Is gyroscope enabled
    int powerMode;
};

#endif // INERTIALSENSOR_H
