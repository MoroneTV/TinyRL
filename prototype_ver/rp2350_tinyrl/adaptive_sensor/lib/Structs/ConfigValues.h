#ifndef CONFIG_VALUES_H
#define CONFIG_VALUES_H

#include <array>
#include <vector>

struct ConfigValues {
    // Env
    float conservatism;
    std::array<float, 3> weights;
    float energyPerEp;
    int msPerEp;

    // Agent
    float epsilon;
    float gamma;
    float lr;
    float noOfActions;
    float noOfStates;

    std::vector<float> qTable;
};

#endif