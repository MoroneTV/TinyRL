#include <Arduino.h>
#include "Display.h"
#include "Environment.h"
#include "QAgent.h"
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include "config.h"           // Global consts 
#include "debug.h"
#include "SensorWeights.h"
#include <Wire.h>

// ===========================================================================================
// GLOBAL OBJECTS ============================================================================
// ===========================================================================================

//Adafruit_MAX17048 battery;
//Display display;

QAgent* agent;
Environment* env;

// ===========================================================================================
// FUNCTION DECLARATIONS =====================================================================
// ===========================================================================================

void setup();
void setup_display();
void setup_battery();
void setup_weights();
void setup_env();

void loop();                 // Agent completes (EP_COUNT) number of episodes, then halts
void loop_completeEpisode(); // Agent completes one full episode

void halt(String reason = "UNSPECIFIED");

// ===========================================================================================
// FUNCTION DEFINITIONS ======================================================================
// ===========================================================================================

void setup()
{
  // Start Serial, allow time to connect
  Wire.begin();
  Serial.begin(115200);
  sleep_ms(2000);

  

  SensorWeights weights({
    INERTIA_WEIGHT,
    AIR_QUALITY_WEIGHT,
    OBSTACLE_WEIGHT
  });

  D_println("STARTING ENV SETUP");
  env = new Environment(weights, CONSERVATISM, ENERGY_PER_EP, MS_PER_EP);
  env->begin();
  D_println("ENV SETUP COMPLETE");

  D_println("BEFORE QAGENT");
  Serial.print("Free heap: ");
  Serial.println(rp2040.getFreeHeap());

  agent = new QAgent(EPSILON, GAMMA, LR, NO_OF_ACTIONS, NO_OF_STATES);

  D_println("AFTER QAGENT");
}

void loop()
{
  for (int i = 1; i <= EP_COUNT; i++) loop_completeEpisode();
  halt("Training Complete.");
}

void loop_completeEpisode()
{

  D_println("HELLO");

  // Create fresh environment
  int state = env->resetEnv().encodedState;
  bool isDone = false;

  // Training loop
  while (!isDone)
  {
    // Take action based on state, get response
    int action = agent->selectAction(state);
    Feedback response = env->step(action);

    // Agent learns based on how environment responds
    agent->update(
        state,
        action,
        response.reward,
        response.newState);

    // Update state and check if the episode is over.
    state = response.newState;
    isDone = response.isDone;
  }
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{

  Serial.println("Program Halted! Reason: ");
  Serial.println(reason);

  while (1)
    sleep_ms(1000);
}
