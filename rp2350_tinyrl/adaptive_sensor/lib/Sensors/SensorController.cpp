// SensorController.cpp
#include "SensorController.h"

bool SensorController::begin()
{
    // Setup inertia sensor so long as it has above 0 weight
    if (inertia.getWeight() > 0) {
        D_println("SENSORCONTROLLER: Starting Inertia Sensor");
        if (!inertia.begin()) D_println("SENSORCONTROLLER: Inertia Sensor Setup Failed!");
    }

}

SensorObservation SensorController::collectData() {
    
    // Collect 1 second's worth of data from all sensors
    int start = millis();
    while(millis() - start < 1000/*ms*/) {
        int now = millis();
        for (Sensor* sensor : sensors)
        {
            if (sensor->isEnabled()) sensor->update(now);
        }
    }

    // Fill structs
    SensorObservation o;

    o.sensorStateData.push_back(inertia.getState());
    o.inertiaReadings = inertia.getBuffer();
    o.inertiaReadingPtr = inertia.getBufferPtr();

    // Cleanup tasks
    inertia.clearBuffer();
}
