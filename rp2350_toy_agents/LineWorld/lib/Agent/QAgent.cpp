#include "QAgent.h"
#include <Arduino.h>
#include <algorithm>

int QAgent::selectAction(int state)
{
    Serial.println("State: " + String(state));
    // Random Chance for exploration (if lower than epsilon)
    float r = (random(0, 100) / 100.0);
    if (r < epsilon)
    {
        int randomIdx = random(actions);
        Serial.println("Random Action:" + String(randomIdx));
        return randomIdx;
    }

    // If not, choose the highest rated action for that state.
    else
    {
        auto it = std::max_element(q_table[state].begin(), q_table[state].end());
        int argMaxIdx = std::distance(q_table[state].begin(), it);
   
        return argMaxIdx;
    }
}

void QAgent::update(int state, int action, float reward, int nextState)
{

    // Find best action for next state
    auto it = std::max_element(q_table[nextState].begin(), q_table[nextState].end());
    int bestNext = std::distance(q_table[nextState].begin(), it);
    // update value of current state accordingly
    float tdTarget = reward + gamma * bestNext;
    q_table[state][action] += lr * (tdTarget - q_table[state][action]);
;
}