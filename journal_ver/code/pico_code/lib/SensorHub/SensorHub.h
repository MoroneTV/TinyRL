#ifndef SENSORHUB_H
#define SENSORHUB_H

#include "ControllerDatatypes.h"
#include "EnvironmentDatatypes.h"

class SensorHub {

    public:
        SensorHub();
        ~SensorHub();
    
        void initialize();

        // Parses instructions, takes 1 second of data, and returns the sensor readings and metadata
        SensorData step(ControllerInstruction controllerInstruction);

    private:

};

#endif