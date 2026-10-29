#include "SensorHub.h"

void SensorHub::initialize()
{
    // Normalise weights, all weights should sum to 1.
    float weightTotal = config.climate.weight +
                        config.inertia.weight +
                        config.light.weight;

    float climateW = config.climate.weight / weightTotal;
    float inertiaW = config.inertia.weight / weightTotal;
    float lightW = config.light.weight / weightTotal;

    Serial.println("RAW WEIGHTS: Light: " + String(config.light.weight) +
                   " | Inertia: " + String(config.inertia.weight) +
                   " | Climate: " + String(config.climate.weight));
    Serial.println("NORMALISED WEIGHTS: Light: " + String(lightW) + "W" +
                   " | Inertia: " + String(inertiaW) + "W" +
                   " | Climate: " + String(climateW) + "W");
    Serial.println("NORMALISED WEIGHT SUM: " +
                   String(lightW + inertiaW + climateW));

    // Assign IDs and weights then add to sensor list. Only initialise objects with weight abv 0
    int id = 1;
    if (climateW > 0)
    {
        Serial.println("Attempting to initialse climate sensor with weight" + String(climateW));
        climate = new ClimateSensor(id, climateW, config.climate.readingBufferLength,
                                    config.climate.upperTempBound, config.climate.lowerTempBound);
        sensors.push_back(climate);
        id++;
        Serial.println("Climate sensor initalisation complete!");
    }
    else
        Serial.println("Climate equates to 0 (actual: " + String(climateW) + "), skipped init");

    if (inertiaW > 0)
    {
        Serial.println("Attempting to initialse inertial sensor with weight" + String(inertiaW));
        inertia = new InertialSensor(id, inertiaW, config.inertia.readingBufferLength);
        sensors.push_back(inertia);
        id++;
        Serial.println("Inertial sensor initalisation complete!");
    }
    else
        Serial.println("Inertial equates to 0 (actual: " + String(inertiaW) + "), skipped init");

    if (lightW > 0)
    {
        Serial.println("Attempting to initialse light sensor with weight" + String(lightW));
        light = new LightSensor(id, lightW, config.light.readingBufferLength);
        sensors.push_back(light);
        id++;
        Serial.println("Light sensor initalisation complete!");
    }
    else
        Serial.println("Light equates to 0 (actual: " + String(lightW) + "), skipped init");

    // Initialise all sensors that have been added to the list
    for (Sensor *sensor : sensors)
        sensor->initialise();
}

SensorData SensorHub::step(ControllerInstruction controllerInstruction)
{
    int startTime = millis();

    // Run Sensors
    applyInstruction(controllerInstruction);
    readSensors();

    // Create Output Obj
    SensorData data;
    data = aggregateMetadata(data);
    data = aggregateReadings(startTime, data);

    return data;
}

// Apply instruction to sensor with matching ID
void SensorHub::applyInstruction(ControllerInstruction instruction)
{
    for (Sensor *sensor : sensors)
    {
        if (instruction.sensorID == sensor->getID())
            sensor->updateSettings(instruction);
    }
}

// Collect 1 second's worth of data from all sensors that are enabled
void SensorHub::readSensors()
{
    int start = millis();
    while (millis() - start < 1000 /*ms*/)
    {
        int now = millis();
        for (Sensor *sensor : sensors)
            sensor->update(now);
    }
}

// Collect metadata for each sensor in vector
SensorData SensorHub::aggregateMetadata(SensorData &data)
{
    for (Sensor *sensor : sensors)
        data.sensorMetadata.push_back(sensor->getMetadata());

    return data;
}

// Collect all readings generated this timestep
SensorData SensorHub::aggregateReadings(unsigned long since, SensorData &data)
{
    SensorReadings r;
    for (Sensor *sensor : sensors)
    {
        sensor->getNewReadings(since, r);
    }

    data.sensorReadings = r;
    return data;
}