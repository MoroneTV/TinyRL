#include "AdaptiveSystem.h"

void AdaptiveSystem::initialize()
{
    controller.initialize();
    // TODO: Fill in
}

StepOverview AdaptiveSystem::step() {
    
    // Observe environment, get controller instruction, execute instruction on sensor hub, 
    // which generates new data for next step
    ControllerInstruction instruction = controller.step(prevStepOverview.envData);
    SensorData sensorData = sensorHub.step(instruction);
    EnvironmentData envData = environment.step(sensorData);

    // Results of current step are stored in prevStepOverview for next step, return for output/logging
    prevStepOverview = StepOverview{instruction, envData};
    return prevStepOverview;
}
