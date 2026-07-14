#ifndef RL_STRUCTS_H
#define RL_STRUCTS_H

// All actions the agent can take
enum AgentActions
{
    SET_SCAN_FQ_10000_MS = 0,
    SET_SCAN_FQ_5000_MS = 1,
    SET_SCAN_FQ_1000_MS = 2,
    SET_SCAN_FQ_500_MS = 3,

    SCAN_MODE_HIGH = 4,
    SCAN_MODE_MID = 5,
    SCAN_MODE_LOW = 6
};

struct Rewards
{
    int highQualityScan = 5;
    int midQualityScan = 3;
    int lowQualityScan = 1;

    int outOfBattery = -100;
};

#endif