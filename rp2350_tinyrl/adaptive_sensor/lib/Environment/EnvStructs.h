#ifndef ENV_STRUCTS_H
#define ENV_STRUCTS_H

// States for an episode to be in
enum EpisodeCompletion {
    INCOMPLETE,
    SUCCESS,
    FAILURE
};

struct State {
    int maxOverSample;    // Highest resolution as compared to interestingness (10 Bins)
    int  maxOverSampleID;   // Corresponding Sensor ID

    int maxUnderSample;   // Highest resolution as compared to interestingness (10 bins)
    int maxUnderSampleID;   // Corresponding Sensor ID

    int drainRate;        // How many energy units are being used per timestep (5 bins)
    int currentBattery;   // Current battery level (5 x 20% bins)

    int timeRemaining;     // Time left in episode (MS)
    float energyRemaining; // energy units left in episode

    EpisodeCompletion epState;  // Is episode complete? if it is, did it succeed or fail?

    int encodedState;   // Integer representing overall state
};

// What is passed back to the agent?
struct Feedback {
    float newState;
    float reward;
    bool isDone;
};

// Increase or decrease sensor lvl?
enum SensorDir {
    UP = 0,
    DOWN = 1
};

#endif