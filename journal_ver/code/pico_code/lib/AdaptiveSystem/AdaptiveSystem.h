#ifndef ADAPTIVESYSTEM_H
#define ADAPTIVESYSTEM_H

#include "AdaptiveSystemDatatypes.h"
#include "Environment.h"
#include "Controller.h"
#include "SensorHub.h"
#include "ConstStructs.h"

#include "RlController.h"
#include "ManualController.h"
#include "AlgorithmicController.h"

class AdaptiveSystem
{

public:
    AdaptiveSystem(SystemConfig config)
        : environment(), sensorHub(config.sensorConfig)
    {
        switch (config.controllerType)
        {
        case RL:
            controller = new RlController();
            break;
        case MANUAL:
            controller = new ManualController();
            break;
        case ALGORITHMIC:
            controller = new AlgorithmicController();
            break;
        }
    };


    ~AdaptiveSystem();

    void initialize();
    StepOverview step(); // Complete one complete control/sensor/processing cycle

private:
    Controller *controller = nullptr;
    Environment environment;
    SensorHub sensorHub;

    StepOverview prevStepOverview; // Previous step's overview,
};

#endif // ADAPTIVESYSTEM_H