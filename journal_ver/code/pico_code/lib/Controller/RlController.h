#ifndef RLCONTROLLER_H
#define RLCONTROLLER_H

#include "Controller.h"

class RlController : public Controller
{
public:
    RlController() {};
    ~RlController() {};

    virtual void initialize() {};  // Initialize the controller

    virtual ControllerInstruction step(EnvironmentData envData) {return {};};  // Look at the environment data and return a controller instruction

private:
};

#endif