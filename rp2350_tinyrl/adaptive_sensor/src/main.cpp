#include <Arduino.h>
#include "Display.h"
#include "Environment.h"
#include "QAgent.h"
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include "config.h"

// ===========================================================================================
// GLOBAL OBJECTS ============================================================================
// ===========================================================================================

Adafruit_MAX17048 battery;
Display display;

QAgent agent(EPSILON, GAMMA, LR, NO_OF_ACTIONS, NO_OF_STATES);
Environment env(SENSOR_WEIGHTS, CONSERVATISM, ENERGY_PER_EP, MS_PER_EP);

// ===========================================================================================
// FUNCTION DECLARATIONS =====================================================================
// ===========================================================================================

void setup();

void loop();            // Agent completes (EP_COUNT) number of episodes, then halts
void completeEpisode(); // Agent completes one full episode

void halt(String reason = "UNSPECIFIED");

// ===========================================================================================
// FUNCTION DEFINITIONS ======================================================================
// ===========================================================================================

void setup()
{
  // Start Serial, allow time to connect
  Serial.begin(115200);
  sleep_ms(2000);
}

void loop()
{
  for (int i = 1; i <= EP_COUNT; i++) completeEpisode();
  halt("Training Complete.");
}

void completeEpisode()
{
  // Create fresh environment
  int state = env.resetEnv().encodedState;
  bool isDone = false;

  // Training loop
  while (!isDone)
  {
    // Take action based on state, get response
    int action = agent.selectAction(state);
    Feedback response = env.step(action);

    // Agent learns based on how environment responds
    agent.update(
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
