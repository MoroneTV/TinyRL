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
  static int episode = 1;

  while (episode < EPISODES) {

    Serial.println("STARTING EPISODE " + String(episode));

    int now = millis();

    int state = env.resetPosition();
    bool done = false;

    Serial.println("1");

    while (!done) {
      int action = agent.selectAction(state);
      std::tuple<int,float,bool> response = env.step(action);

      Serial.println("2");

      int nextState = std::get<0>(response);
      float reward = std::get<1>(response);
      done = std::get<2>(response);

      Serial.println("3");
      Serial.println("Pos: " + String(state));
      agent.update(state, action, reward, nextState);
      state = nextState;

      Serial.println("4");
      Serial.println("Pos: " + String(state));

    }

    Serial.println("Episode " + String(episode) + " complete in " + String(millis() - now) + "ms.");
  }

  halt("COMPLETE");
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
