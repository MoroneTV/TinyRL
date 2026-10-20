#ifndef ADAPTIVESYSTEMDATATYPES_H
#define ADAPTIVESYSTEMDATATYPES_H

#include "EnvironmentDatatypes.h"

/*
Contains all information about sensors, metadata, controller, environment
To be passed to output functions for logging, visualization, etc.
*/
struct StepOverview {
    ControllerInstruction instruction;
    EnvironmentData envData;
};

#endif