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
void dsp_loopScreen(int epNo, int agentPos);

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
  dsp_loopScreen(1, 3);
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
void dsp_loopScreen(int epNo, int agentPos) {

  // Display agent on screen
  String environment = "_ _ _ _ _ _ _ _ _ _ G";
  String barometer =   "0 1 2 3 4 5 6 7 8 9 *";
  environment.setCharAt(agentPos*2, 'A');

  display.writeLines(
    "Episode" + String(epNo),
    "",
    "",
    ""
    "",
    environment,
    "",
    barometer
  );

}
