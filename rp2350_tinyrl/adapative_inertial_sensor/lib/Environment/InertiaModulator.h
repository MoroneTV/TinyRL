#ifndef INERTIA_MODULATOR_H
#define INERTIA_MODULATOR_H

#include <Arduino.h>
#include "InertiaSensor.h"

struct InertiaFeedback {
    float varianceRaw = 0.0f;
    int varianceLvl = 0;

    int reward = 0.0f;
    int precisionLvl = 0;

    int newState = 0;

    // These are just for printing
    String avgAX = "X";
    String avgAY = "X";
    String avgAZ = "X";

    String avgGX = "X";
    String avgGY = "X";
    String avgGZ = "X";
};

class InertiaModulator {
    public:
        InertiaModulator() {
        };

        void begin();

        InertiaFeedback agentAction(int action);

        // Resets buffer and sensor to default
        int resetEnvironment();    

    private:
        InertiaSensor sensor;
        
        void fillBuffer();                    // Collect 1 sec of readings
        void resetBuffer();                   // Empty buffer and set maxIdx to 0;
        InertiaReadings rBuffer[250];         // Max sense rate is 240, plus some wiggle room
        int rIdx;                             // last idx of the buffer that has data.

        InertiaFeedback processBuffer();  // Returns variance of all buffer values, as well as averages
};

#endif
