#ifndef SENSOR_H
#define SENSOR_H

#include <SensorStructs.h>

class Sensor {
public:

    virtual bool begin() = 0;
    virtual void update(int now) = 0;
    virtual void setLvl(int lvl) = 0;
    virtual void setId(int id) = 0;

    virtual int getRawLvl() = 0;
    virtual float getWeight() = 0;
    virtual float getInterestingness() = 0;
    virtual float getScaledLvl() = 0;
    virtual float getFlatCost() = 0;
    virtual int getId() = 0;
    virtual String getName() = 0;
    virtual bool isEnabled() = 0;

    virtual SensorState getState() = 0;
};



#endif
