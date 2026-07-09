#include "LineWorld.h"

std::tuple<int,float,bool> LineWorld ::step(int dir)
{
    if (dir == 0) pos--;
    else pos++;

    // Slightly punish not reaching the goal, big reward if goal is met
    if (pos >= goal) return std::make_tuple(pos, 10.0f, true);
    else return std::make_tuple(pos, -0.01f, false);
}