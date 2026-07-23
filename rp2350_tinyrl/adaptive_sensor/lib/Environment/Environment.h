#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <VirtualSensor.h>

class Environment {
public:
    Environment();

private:

std::vector<Sensor> sensors;
float conservatism;
float batteryFrac;


};

#endif // ENVIRONMENT_H
