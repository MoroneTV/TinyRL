#ifndef INERTIA_MODULATOR_H
#define INERTIA_MODULATOR_H

#include <Arduino.h>
#include "InertiaSensor.h"
#include <unordered_map>

struct InertiaFeedback {
    float varianceRaw = 0.0f;
    int varianceLvl = 0;

    float reward = 0.0f;
    int precisionLvl = 0;

    int newState = 0;
    bool isDone = false;

    // These are just for printing
    String avgAX = "X";
    String avgAY = "X";
    String avgAZ = "X";

    String avgGX = "X";
    String avgGY = "X";
    String avgGZ = "X";

    String minsLeftInEp = "X";
    String energyPercentLeft = "X";
};



class InertiaModulator {
    public:
        InertiaModulator(float maxEnergy = 10000.0f) : maxEnergy(maxEnergy) {
        };

        void begin();

        InertiaFeedback agentAction(int action);

        // Resets buffer and sensor to default
        int resetEnvironment();    

    private:
        InertiaSensor sensor;

        // Energy costs. In the worst case of 40 minute runtime:
        std::unordered_map<int,float> energyCosts = {
            {0 , 2.8f},     // Can survive 60 minutes    
            {1, 8.3f},     // Can survive 20 minutes
            {2, 22.2f}     // Can survive 7.5 Minutes
        };

        // Reward value is based on if the precison mode is reasonable given the battery level and activity variance.
        float calculateReward(int varianceLvl, int action);
        
        // Sensor Management
        void fillBuffer();                    // Collect 1 sec of readings
        void resetBuffer();                   // Empty buffer and set maxIdx to 0;
        InertiaReadings rBuffer[250];         // Max sense rate is 240, plus some wiggle room
        int rIdx;                             // last idx of the buffer that has data.
        InertiaFeedback processBuffer();      // Returns variance of all buffer values, as well as averages
        int getBoundVariance(float rawVar);   // Discretises variance level into 3 bins


        // Time and Energy
        int startTime;                   // time that the episode began
        int episodeLength;          // Amount of time left in episode. random between 20 and 40 minutes.
        int timeLeftMs;
        float energyRemaining;    // Amount of energy left in episode. Fuel gauge is unreliable so using virtual energy
        float maxEnergy;
        int getDiscreteEnergyLvl(float energyLevel); // Threshold energy into 10 bins for state.
};

#endif
