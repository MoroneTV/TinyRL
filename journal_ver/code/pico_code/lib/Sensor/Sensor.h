#ifndef SENSOR_H
#define SENSOR_H

#include "SensorDatatypes.h"
#include "ControllerDatatypes.h"

class Sensor {

    public:
    virtual bool initialize() = 0;  // Enable sensor
    virtual bool updateSettings(ControllerInstruction instructions) = 0;    // Update sensor Lvl

    virtual SensorMetadata getMetadata() = 0;

    virtual void update(int now) = 0;   // Take reading if needed.

    private:
};

#endif