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
    std::vector<float> qTable;
    int maxStates;

    bool validState(int state)
    {
        return state >= 0 && state < maxStates;
    }

    float &Q(int state, int action)
    {
        return qTable[state * actions + action];
    }

public:
    QAgent(float epsilon = 0.1,
           float gamma = 0.99,
           float lr = 0.1,
           int actions = 2,
           int maxState = 20)
        : epsilon(epsilon),
          gamma(gamma),
          lr(lr),
          actions(actions),
          maxStates(maxState),
          qTable(maxState * actions, 0.0f)
    {
        Serial.println("QAGENT CONSTRUCTOR START");
        Serial.println("FLAT TABLE CREATED");
    }

    int selectAction(int state);
    void update(int state, int action, float reward, int nextState);

    std::vector<float> &getQTable()
    {
        return qTable;
    }
};

#endif