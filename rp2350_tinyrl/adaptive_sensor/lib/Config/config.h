#ifndef CONFIG_H
#define CONFIG_H

#include "SensorWeights.h"

// Runtime Consts
static const bool LOAD_LATEST_Q_TABLE = false;

static const int MINS_PER_EP = 40;
static const int MS_PER_EP = (MINS_PER_EP * 60000);
static const int EP_COUNT = 5; // Training concludes after this many episodes
static const float ENERGY_PER_EP = 10000.0f;

// Weights per sensor, affects which sensor/s the agent priortises. 0 = sensor disabled.
static const float INERTIA_WEIGHT = 1.0f;
static const float AIR_QUALITY_WEIGHT = 0.0f;
static const float OBSTACLE_WEIGHT = 0.0f;

// Training hyperparameters (Env)
static const float CONSERVATISM = 2.0f; // How much does the agent priortise saving energy? 0.0f = no consideration for energy

// Training hyperparameters (Agent)
static const float EPSILON = 0.1f;
static const float GAMMA = 0.99f;
static const float LR = 0.1f;
static const int NO_OF_ACTIONS = 4;
static const int NO_OF_STATES = 500;

#endif