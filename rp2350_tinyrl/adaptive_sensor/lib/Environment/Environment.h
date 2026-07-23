#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <Sensor.h>
#include <SensorWeights.h>
#include <SensorController.h>
#include <EnvReward.h>
#include <EnvState.h>

class Environment
{
public:
    Environment(SensorWeights weights, float conservatism, float batteryBudget, int msPerEpisode)

        : conservatism(conservatism), batteryBudget(batteryBudget), msPerEpisode(msPerEpisode),
          sensorController(new SensorController(weights))
    {
    }

    // Take one timestep (1000ms)
    EnvState step(int action);

private:
    EnvReward calculateReward();
    EnvState calculateState();

    float conservatism = 0.0f; // How much should battery penalty be scaled in reward calculation? 0.0f = battery is not considered.

    int msPerEpisode = 0; // How many ms should an episode be?
    int msLeftInEpisode;  // How many ms are left in the episode?

    float batteryFrac;          // How much of the energy budget is remaining?
    float batteryBudget = 0.0f; // Virtual energy source so i don't have to rely on imperfect fuel gauge readings.

    SensorController *sensorController;
};

#endif // ENVIRONMENT_H
