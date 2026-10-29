#ifndef CONSTS_H
#define CONSTS_H

#include "ConstStructs.h"

// Individual Sensor Settings
const int CLIMATE_WEIGHT = 1;
const int CLIMATE_BUFFER_LEN = 10;
const int CLIMATE_MIN_BOUND = 5;
const int CLIMATE_MAX_BOUND = 30;

const int LIGHT_WEIGHT = 1;
const int LIGHT_BUFFER_LEN = 10;

const int INERTIA_WEIGHT = 1;
const int INERTIA_BUFFER_LEN = 200;

// Manual | Algorithmic | RL
const ControllerType CONTROLLER_TYPE = RL;

// Training = Random interestingness readings are presented to the system
// Showcase = Real sensor readings are presented to the system.
const OperationMode OPERATION_MODE = SHOWCASE;

// When RL controller is used, this is used to scale the weight of energy depletion.
// 0 = Energy depletion doesnt matter | Really high = system will always use min sensor settings to minimise power loss
const float CONSERVATION_SCALER = 3.0f;


#endif