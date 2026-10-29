#ifndef MANUALCONTROLLER_H
#define MANUALCONTROLLER_H

#include "Controller.h"

class ManualController : public Controller
{
public:
    ManualController() {};
    ~ManualController() {};

    virtual void initialize() {};  // Initialize the controller

    virtual ControllerInstruction step(EnvironmentData envData) {return {};};  // Look at the environment data and return a controller instruction

private:
};

#endif