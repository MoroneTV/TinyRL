#ifndef ADAPTIVESYSTEM_H
#define ADAPTIVESYSTEM_H

#include "AdaptiveSystemDatatypes.h"
#include "Environment/Environment.h"
#include "Controller/Controller.h"
#include "Sensor/SensorHub.h"

class AdaptiveSystem {

    public:
        AdaptiveSystem(
            Controller& controller,
            Environment& environment,
            SensorHub& sensorHub
        )
        : controller(controller), environment(environment), sensorHub(sensorHub) {};
        ~AdaptiveSystem();

        void initialize();
        StepOverview step();      // Complete one complete control/sensor/processing cycle

    private:
        Controller& controller;
        Environment& environment;
        SensorHub& sensorHub;

        StepOverview prevStepOverview;  // Previous step's overview,
};

#endif // ADAPTIVESYSTEM_H