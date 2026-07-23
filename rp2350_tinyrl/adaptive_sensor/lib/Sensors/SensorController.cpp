// SensorController.cpp
#include "SensorController.h"

bool SensorController::begin()
{
    // Setup inertia sensor so long as it has above 0 weight
    if (inertia.getWeight() > 0) {
        D_println("SENSORCONTROLLER: Starting Inertia Sensor");
        if (!inertia.begin()) D_println("SENSORCONTROLLER: Inertia Sensor Setup Failed!");
    }
    return true;
}

SensorObservation SensorController::collectData() {

    int start = millis();

    // Collect 1 second's worth of data from all sensors
    while(millis() - start < 1000) {
        int now = millis();

        inertia.update(now);
    }

    // Fill structs
    SensorObservation o;

    o.sensorStateData.push_back(inertia.getState());
    // Copy buffer contents into observation array (arrays are not assignable)
    auto *buf = inertia.getBuffer();
    size_t len = sizeof(o.inertiaReadings) / sizeof(o.inertiaReadings[0]);
    for (size_t i = 0; i < len; ++i) {
        o.inertiaReadings[i] = buf[i];
    }
    o.inertiaReadingPtr = inertia.getBufferPtr();

    // Cleanup tasks
    return o;
}
