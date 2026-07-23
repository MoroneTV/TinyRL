#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <Sensor.h>
#include <SensorWeights.h>
#include <SensorController.h>

class Environment {
public:
        Environment(SensorWeights weights, float conservatism, float energyBudget, int msPerEpisode)
        
        : conservatism(conservatism), energyBudget(energyBudget), msPerEpisode(msPerEpisode),
            sensorController(new SensorController(weights)) {
        }
        ;

private:

float conservatism;
float batteryFrac;
float energyBudget = 0.0f;
int msPerEpisode = 0;

SensorController sensorController;


};

#endif // ENVIRONMENT_H
