#include "QAgent.h"
#include <Arduino.h>
#include <algorithm>

int QAgent::selectAction(int state)
{
    // Random Chance for exploration (if lower than epsilon)
    float r = (random(0, 100) / 100.0);
    if (r < epsilon)
    {
        int randomIdx = random(actions);
        return randomIdx;
    }

    // If not, choose the highest rated action for that state.
    else
    {   
        // Vector for multiple idxs in case there's a tie and we need to choose randomly
        float highestValue = 0.0f;
        std::vector<int> bestIdxs = {0};

        for (int actionIdx = 0; actionIdx < qTable[state].size(); actionIdx++) {

            // if new best action is found, flush vector and start again, if a tie, just add.
            if (qTable[state][actionIdx] > highestValue) {
                bestIdxs.clear();
                bestIdxs.push_back(actionIdx);
            }
            else if (qTable[state][actionIdx] == highestValue) {
                bestIdxs.push_back(actionIdx);
            }

        }

        return bestIdxs[random(bestIdxs.size())];
    }
}

void QAgent::update(int state, int action, float reward, int nextState)
{

    // Find highest value next action and propogate backwards

    float highestValueNext = qTable[nextState][0];
    for (float value : qTable[nextState]) {
        if (value > highestValueNext) highestValueNext = value;
    }

    qTable[state][action] = lr * (reward + gamma * highestValueNext - qTable[state][action]);
}