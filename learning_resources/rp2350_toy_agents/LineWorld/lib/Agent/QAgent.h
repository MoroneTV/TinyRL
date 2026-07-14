#ifndef Q_AGENT_H
#define Q_AGENT_H

#include <unordered_map>
#include <vector>
#include <Arduino.h>

class QAgent
{
private:
    float epsilon;
    float gamma;
    float lr;
    int actions;
    std::vector<std::vector<float>> qTable;

public:
    QAgent(float epsilon = 0.1, float gamma = 0.99, float lr = 0.1, int actions = 2, int maxState = 20)
        : epsilon(epsilon), gamma(gamma), lr(lr), actions(actions), qTable(maxState, std::vector<float>(actions, 0.0f))
    {
    
    }

    int selectAction(int state);
    void update(int state, int action, float reward, int nextState);
};

#endif