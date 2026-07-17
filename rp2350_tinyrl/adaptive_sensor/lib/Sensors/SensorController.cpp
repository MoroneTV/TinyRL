// SensorController.cpp
#include "SensorController.h"

SensorController::SensorController()
{
    // ctor - nothing to do for now
}

bool SensorController::begin()
{
    // initialize sensors if applicable
    // If InertialSensor has a begin() method, call it. Otherwise assume ready.
#ifdef InertialSensor_h
    // try to call begin() if available
    // This block is defensive; if InertialSensor provides begin(), it will be used.
    // Otherwise the preprocessor guard will skip this.
    // Note: If the actual InertialSensor API differs, update accordingly.
    return inertia.begin();
#else
    (void)inertia;
    return true;
#endif
}

SensorBuffer SensorController::takeSample()
{
    SensorBuffer buf;
    // Default: no data collected yet
    buf.inertiabuffer = nullptr;
    return buf;
}
