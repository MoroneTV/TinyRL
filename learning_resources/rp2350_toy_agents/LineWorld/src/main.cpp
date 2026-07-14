#include <Arduino.h>
#include "Display.h"
#include <Wire.h>
#include <QAgent.h>
#include <LineWorld.h>
#include <tuple>

// ============ CONSTANTS ============ //

#define EPISODES 500

// ============ SYSTEM OBJECTS AND VARS ============ //

Display display; // Screen wrapper class

LineWorld env(10);
QAgent agent;

// ============ MAIN FUNCTIONS ============ //

void setup();
void loop();
void halt(String reason = "UNSPECIFIED");

// =========== HELPER FUNCTIONS =========== //
void dsp_loopScreen(int epNo, int agentPos, int shortestEp);

// ================================================================ //
// ================================================================ //
// ================================================================ //

void setup()
{
  Serial.begin(115200);
  delay(2000);

  display.begin();
}

void loop()
{

  int fastestTime = 1000000000;

  for (int episode = 0; episode < EPISODES; episode++) {

    int state = env.resetPosition();
    bool done = false;

    int start = millis();

    while(!done) {

      // Choose action and apply it to environment
      Directions1D action = static_cast<Directions1D>(agent.selectAction(state));
      EnvResponse response = env.step(action);

      // Update the agent's understanding based on environement response to action
      agent.update(
        state,
        action,
        response.reward,
        response.newPos
      );

      // Update position and check if we're done.
      state = response.newPos;
      done = response.isDone;

      // Display
      dsp_loopScreen(episode, state, fastestTime);

    }

    int timeTaken = millis() - start;
    if (timeTaken < fastestTime) fastestTime = timeTaken;

  }
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  display.writeLines(
      "Program Halted!", "",
      "Reason:",
      reason);

  while (1)
    sleep_ms(1000);
}

// Screen shown during the RL's main loop, showing the agent's position on a line.
void dsp_loopScreen(int epNo, int agentPos, int shortestEp) {

  String shortestEpisodeString = "";
  if (shortestEp == 1000000000) shortestEpisodeString = ("No data yet");
  else shortestEpisodeString = (String(shortestEp) + "ms");

  // Display agent on screen
  String environment = "_ _ _ _ _ _ _ _ _ _ G";
  String barometer =   "0 1 2 3 4 5 6 7 8 9 *";
  environment.setCharAt(agentPos*2, 'A');

  display.writeLines(
    "EPISODE " + String(epNo),
    "SHORTEST EP: " + shortestEpisodeString,
    "",
    ""
    "",
    environment,
    "",
    barometer
  );

}
