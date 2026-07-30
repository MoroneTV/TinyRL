#ifndef SENSOR_WEIGHTS_H
#define SENSOR_WEIGHTS_H

#include <debug.h>
#include <Arduino.h>
#include <array>


class SensorWeights
{
private:

    bool valid = false;
    float inertiaW;
    float airQualityW;
    float lightW;
    
public:

    // Weight array in order {Inertia, AirQ, Obstacle}. Weights must not be negative and at least one must be above 0.
    SensorWeights(std::array<float, 3> weights = {0.0f, 0.0f, 0.0f}) {

        // Weights cannot be negative, default to 0
        // Add all weights together- at least one weight must be above 0.
        float sum = 0.0f;
        for (float& weight : weights) {
            
            if (weight < 0.0f) {
                weight = 0.0f;
            }
            sum += weight;
        }

        if (sum <= 0.0f) {
            return;
        }

        else {

            inertiaW    = weights[0] / sum;
            airQualityW = weights[1] / sum;
            lightW   = weights[2] / sum;

            valid = true;
        }
    };

    float getInertiaW() { return inertiaW; }
    float getAirQualityW() { return airQualityW; } 
    float getLightW() { return lightW; } 
    bool isValid() { return valid; } 
};

#endif
