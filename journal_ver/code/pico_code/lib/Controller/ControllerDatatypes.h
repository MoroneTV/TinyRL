#ifndef CONTROLLERDATATYPES_H
#define CONTROLLERDATATYPES_H

/*
Absolute instructions set a sensor to a specific level (e.g. Light sensor, 3)
Relative instructions increase or decrease a sensor's level (e.g. Light sensor, UP)
If system is performing perfectly, no instructions are needed, so NONE
*/
enum instructionType {
    ABSOLUTE,
    RELATIVE,
    NONE
};

enum Direction {
    UP,
    DOWN
};

struct ControllerInstruction {
    instructionType type;

    int sensorID;   // Which sensor to control
    Direction direction;  // Direction to change the sensor by, if relative
    int value;      // Value to set or change the sensor by, if absolute
};

#endif