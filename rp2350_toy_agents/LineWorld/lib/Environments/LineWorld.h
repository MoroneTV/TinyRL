#ifndef LINEWORLD_H
#define LINEWORLD_H

#include <Arduino.h>
#include "Directions.h"

struct EnvResponse {
    int newPos;
    float reward;
    bool isDone;

};


class LineWorld {

    private:
        int pos = 0; // agent position in world
        int goal;    // position that must be reached before episode ends

    public:
        LineWorld(int goal) : goal(goal) {}

        int resetPosition() { pos = 0; return pos;}   // reset agent position
        
        // Take a step in a direction, returns EnvResponse (newPos, reward, isDone)
        EnvResponse step(Directions1D dir);

};

#endif