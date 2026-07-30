
#include "SensorController.h"
#include "EnvStructs.h"
#include "debug.h"

bool SensorController::begin()
{

    int idAssignment = 1;

    D_println("SENSOR CONTROLLER BEGIN");

    for (Sensor* sensor : sensors)
    {
        D_println("Attempting to set up sensor: " + sensor->getName());

        if (!sensor->begin())
        {
            D_println("Sensor setup failed!: " + sensor->getName());
            return false;
        }
        sensor->setId(idAssignment);
        D_println("Sensor setup complete: " + sensor->getName() + ", with ID: " + String(idAssignment));
        idAssignment++;
    }

    D_println("ALL SENSORS COMPLETE");
    return true;
}

SensorObservation SensorController::collectData()
{

    // Collect 1 second's worth of data from all sensors
    int start = millis();
    while (millis() - start < 1000 /*ms*/)
    {
        int now = millis();
        for (Sensor *sensor : sensors)
        {
            if (sensor->isEnabled())
                sensor->update(now);
        }
    }

    // Fill structs
    SensorObservation o;

    o.sensorStateData.push_back(inertia.getState());
    o.inertiaReadings = inertia.getBuffer();
    o.inertiaReadingPtr = inertia.getBufferPtr();

    o.sensorStateData.push_back(airQuality.getState());
    o.airQualityReading = airQuality.getMostRecentReading();

    o.sensorStateData.push_back(light.getState());
    o.lightReading = light.getMostRecentReading();

    // Cleanup tasks
    inertia.clearBuffer();

    return o;
}

void SensorController::updateSensorSettings(int id, SensorDir dir)
{

    for (Sensor *sensor : sensors)
    {
        if (sensor->getId() == id)
        {
            if (dir == UP)
                sensor->setLvl(sensor->getRawLvl() + 1);
            else
                sensor->setLvl(sensor->getRawLvl() - 1);
        }
    }
}

void SensorController::resetSensors() {
    inertia.setLvl(0);
    inertia.clearBuffer();
}
