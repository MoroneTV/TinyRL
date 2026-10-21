#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ControllerDatatypes.h"
#include "EnvironmentDatatypes.h"

// Virtual class for controller objects, like RL, manual, algorithmic, etc.
class Controller {

    public:

    virtual void initialize() = 0;  // Initialize the controller

    virtual ControllerInstruction step(EnvironmentData envData) = 0;  // Look at the environment data and return a controller instruction
    private:

};

#endif
