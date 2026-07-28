#ifndef ENV_STRUCTS_H
#define ENV_STRUCTS_H

// States for an episode to be in
enum EpisodeCompletion {
    INCOMPLETE,
    SUCCESS,
    FAILURE
};

struct State {
    int maxOverSample = 0;    // Highest resolution as compared to interestingness (10 Bins)
    int  maxOverSampleID = 0;   // Corresponding Sensor ID

    int maxUnderSample = 0;   // Highest resolution as compared to interestingness (10 bins)
    int maxUnderSampleID = 0;   // Corresponding Sensor ID

    int leastImportantSensorID = -1; // Sensor that should be turned down first in order to save battery

    int currentBattery = 5;   // Current battery level (5 x 20% bins)

    int timeRemaining;     // Time left in episode (MS)
    float energyRemaining; // energy units left in episode

    EpisodeCompletion epState;  // Is episode complete? if it is, did it succeed or fail?

    int encodedState = 2500;   // Integer representing overall state
};

// What is passed back to the agent?
struct Feedback {
    int newState;
    float reward;
    bool isDone;
};

#endif