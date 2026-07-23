#ifndef SENSOR_WEIGHTS_H
#define SENSOR_WEIGHTS_H

#include <config.h>
#include <Arduino.h>
#include <array>

class SensorWeights
{
private:

    bool isValid = false;
    float inertiaW;
    float airQualityW;
    float obstacleW;
    
public:
    SensorWeights(std::array<float,3> weights = {0.0f, 0.0f, 0.0f}) {
        inertiaW = weights[0];
        airQualityW = weights[1];
        obstacleW = weights[2];
        isValid = true;
    }

    float getInertiaW() { return inertiaW; }
    float getAirQualityW() { return airQualityW; } 
    float getObstacleW() { return obstacleW; } 
    bool validWeights() { return isValid; } 





};

#endif
