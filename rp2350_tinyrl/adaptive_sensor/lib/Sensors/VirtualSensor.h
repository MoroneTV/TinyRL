#ifndef VIRTUALSENSOR_H
#define VIRTUALSENSOR_H

#include <InertialSensor.h>

class VirtualSensor {
public:
    virtual ~VirtualSensor() = default;

    virtual bool begin() = 0;
    virtual void update(int now) = 0;

    virtual InertiaReading* getBuffer() = 0;
    virtual int getBufferPtr() = 0;

    virtual bool isEnabled() = 0;
    virtual InertiaPowerLevel getPowerMode() = 0;

    virtual void updatePowerMode(InertiaPowerLevel lvl) = 0;
    virtual void clearBuffer() = 0;

};

#endif // VIRTUALSENSOR_H
