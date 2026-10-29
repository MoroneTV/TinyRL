#ifndef ALGORITHMICCONTROLLER_H
#define ALGORITHMICCONTROLLER_H

#include "Controller.h"

class AlgorithmicController : public Controller
{
public:
    AlgorithmicController() {};
    ~AlgorithmicController() {};

    virtual void initialize() {};  // Initialize the controller

    virtual ControllerInstruction step(EnvironmentData envData) {return {};};  // Look at the environment data and return a controller instruction

private:
};

#endif