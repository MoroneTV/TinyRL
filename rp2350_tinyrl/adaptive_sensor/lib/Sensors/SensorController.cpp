
#include "SensorController.h"
#include "EnvStructs.h"
#include "debug.h"

bool SensorController::begin()
{

    int idNo = 1;

    for (Sensor *sensor : sensors)
    {
        if (sensor->getWeight() > 0.0f)
        {
            D_println("Attempting to set up sensor: " + sensor->getName());
            if (!sensor->begin())
            {
                D_println("Sensor setup failed!: " + sensor->getName());
                return false;
            }
            else
            {
                sensor->setId(idNo);
                idNo++;
            }
        }
        else D_println("(Sensor: " + sensor->getName() + ") Weight is set to 0, not setting up");
    }

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
