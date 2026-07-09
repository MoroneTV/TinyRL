#ifndef LINEWORLD_H
#define LINEWORLD_H

#include <tuple>

class LineWorld {

    private:
        int pos = 0; // agent position in world
        int goal;    // position that must be reached before episode ends

    public:
        LineWorld(int goal) : goal(goal) {}

        int resetPosition() { pos = 0; return pos;}   // reset agent position
        
        // Take a step in a direction, returns tuple <(position), (reward), (is done?)>
        std::tuple<int,float,bool> step(int dir);

};

#endif