#include "QAgent.h"
#include <Arduino.h>
#include <algorithm>
#include <debug.h>

int QAgent::selectAction(int state)
{
    if (!validState(state)) {
        D_println("INVALID STATE: ");
        D_println(state);
        return 0;
    }

    // Exploration
    float r = (random(0, 100) / 100.0);

    if (r < epsilon)
    {
        return random(actions);
    }

    // Exploitation
    else
    {
        float highestValue = Q(state, 0);

        // Store tied actions without allocating
        int bestIdxs[actions];
        int bestCount = 1;
        bestIdxs[0] = 0;

        for (int actionIdx = 1; actionIdx < actions; actionIdx++)
        {
            float value = Q(state, actionIdx);

            if (value > highestValue)
            {
                highestValue = value;
                bestCount = 1;
                bestIdxs[0] = actionIdx;
            }
            else if (value == highestValue)
            {
                bestIdxs[bestCount] = actionIdx;
                bestCount++;
            }
        }

        return bestIdxs[random(bestCount)];
    }
}

void QAgent::update(int state, int action, float reward, int nextState)
{
    if (!validState(state) || !validState(nextState)) {
        D_println("INVALID STATE IN UPDATE");
        return;
    }

    if (action < 0 || action >= actions) {
        D_println("INVALID ACTION");
        return;
    }

    float highestValueNext = Q(nextState, 0);

    for (int actionIdx = 1; actionIdx < actions; actionIdx++)
    {
        float value = Q(nextState, actionIdx);

        if (value > highestValueNext)
            highestValueNext = value;
    }

    Q(state, action) += lr * 
        (reward + gamma * highestValueNext - Q(state, action));
}