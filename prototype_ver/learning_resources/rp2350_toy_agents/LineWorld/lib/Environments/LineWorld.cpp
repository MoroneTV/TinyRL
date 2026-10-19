#include "LineWorld.h"

EnvResponse LineWorld ::step(Directions1D dir)
{
    if (dir == LEFT) pos--;
    else pos++;

    // Keep position in bounds
    if (pos > 10) pos = 10;
    if (pos < 0) pos = 0;

    // Slightly punish not reaching the goal, big reward if goal is met
    if (pos >= goal) 
        return EnvResponse{pos,10.0,true};
    else 
        return EnvResponse{pos,-0.01,false};
}