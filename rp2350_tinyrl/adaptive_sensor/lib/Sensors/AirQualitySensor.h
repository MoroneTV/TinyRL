#ifndef AIRQUALITYSENSOR_H
#define AIRQUALITYSENSOR_H

#include "VirtualSensor.h"

class AirQualitySensor : public VirtualSensor {
public:
    AirQualitySensor();
    ~AirQualitySensor();

    bool begin() override;
    void update(int now) override;

    InertiaReading* getBuffer() override;
    int getBufferPtr() override;

    bool isEnabled() override;
    InertiaPowerLevel getPowerMode() override;

    void updatePowerMode(InertiaPowerLevel lvl) override;
    void clearBuffer() override;
};

#endif // AIRQUALITYSENSOR_H
