#ifndef INERTIA_SENSOR_H
#define INERTIA_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <tuple>
#include <SparkFun_LSM6DSV16X.h>

struct InertiaReadings {
    float aX;
    float aY;
    float aZ;
    float gX;
    float gY;
    float gZ;
};

class InertiaSensor
{

public:
    InertiaSensor();

    bool begin();
    InertiaReadings read();
    void updatePrecision(int lvl);  // 0 = Sleep, 4 = Max quality and power consumption

    int getPrecisionLvl() { return precisionLvl; }
    bool isEnabled() { return enabled; }
    
private:
    SparkFun_LSM6DSV16X lsm;

    bool enabled = false;
    int precisionLvl = 1;

};


#endif