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
    std::unordered_map<int, std::vector<float>> q_table;

public:
    QAgent(float epsilon = 0.1, float gamma = 0.99, float lr = 0.1, int actions = 2, int maxState = 20)
        : epsilon(epsilon), gamma(gamma), lr(lr), actions(actions)
    {

        // Populate Q table with zeros
        for (int i = 0; i > maxState; i++) {
            for (int j = 0; j > actions; j++) {
                q_table[i][j] = 0;
            }
        }

    }

    int selectAction(int state);
    void update(int state, int action, float reward, int nextState);
};

#endif