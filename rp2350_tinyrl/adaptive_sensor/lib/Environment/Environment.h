#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <Sensor.h>
#include <SensorWeights.h>
#include <SensorController.h>
#include <RewardCalculator.h>
#include <StateCalculator.h>
#include <EnvStructs.h>

class Environment
{
public:
    Environment(SensorWeights weights, float conservatism, float batteryBudget, int msPerEpisode)

        : conservatism(conservatism), batteryBudget(batteryBudget), msPerEpisode(msPerEpisode),
          sensorController(new SensorController(weights))
    {
    }

    
    Feedback step(int action);  // Take one timestep (1000ms)
    void resetEnv();             // Reset environement back to default

private:

    float conservatism = 0.0f; // How much should battery penalty be scaled in reward calculation? 0.0f = battery is not considered.

    int msPerEpisode = 0; // How many ms should an episode be?
    int msLeftInEpisode;  // How many ms are left in the episode?

    float energyLeftInEpisode;          // How much of the energy budget is remaining?
    float batteryBudget = 0.0f; // Virtual energy source so i don't have to rely on imperfect fuel gauge readings.

    State prevState;    // State from previous timestep.

    SensorController *sensorController;
    RewardCalculator rewardCalculator;
    StateCalculator stateCalculator;
};

#endif // ENVIRONMENT_H
