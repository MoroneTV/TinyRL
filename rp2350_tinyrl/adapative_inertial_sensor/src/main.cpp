#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>
#include "InertiaModulator.h"
#include "QAgent.h"
#include <SD.h>

#define EPISODE_COUNT 5
#define MS_PER_EPISODE 60000

// ========= Objects and Globals =========
Adafruit_MAX17048 battery; // Battery Sensor
Display display;           // Screen

QAgent agent(0.1f, 0.0f, 0.1f, 3, 30);  // 3 actions and 30 states
InertiaModulator env;

String runtimePermutation = ""; // Used to define the Log and Q Table file numbers

// ========= Main Functions =========
void setup();
void loop();
void halt(String reason = "UNSPECIFIED");

// ========= Setup Functions =========
void setupBattery();
void setupSDcard();
void defineRuntimePermutation();
void importQtable();

// ========= Loop Functions =========

void exportQTable();
void updateResultsFile();

// ========= Display Functions =========
void dsp_setup();
void dsp_loop(InertiaFeedback environmentInformation, float batteryLvl, int episodeNo);
void dsp_halt(String reason = "UNSPECIFIED");

//
// FUNCTION DEFINITIONS
//

void setup()
{
  // Start Serial, allow time to connect
  Serial.begin(115200);
  sleep_ms(3000);

  // Start Display and battery
  display.begin();
  battery.begin();
  env.begin();
}

void loop()
{

  for (int i = 1; i < EPISODE_COUNT+1; i++) {

    int state = env.resetEnvironment();
    bool isDone = false;
    int start = millis();

    while (!isDone) {

      // Take action, get response
      int action = agent.selectAction(state);
      InertiaFeedback response = env.agentAction(action);

      // Learn from action and response
      agent.update(
        state,
        action,
        response.reward,
        response.newState
      );

      Serial.println("Variance level " + String(response.varianceLvl) + " when in state " + String(state)+ ". Moving to state " + String(response.newState));

      // Update state
      state = response.newState;
      isDone = response.isDone;

      // Display information to screen
      dsp_loop(response, battery.cellVoltage(), i);
    }

    std::vector<std::vector<float>> qTable = agent.getQTable();

    Serial.println();
    Serial.println("Q TABLE AFTER EPISODE" + String(i));
    for (int i = 0; i < qTable.size(); i++) {
      Serial.print("STATE " + String(i)+ ": ");
      for(float weight : qTable[i]) {
        Serial.print ("| " + String(weight));
      }
      Serial.print("\n");
    }
    Serial.println();

  }

  halt("Training Complete!");
}


// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{

  SD.begin(SS);
  File f = SD.open("agent.txt", FILE_WRITE);
  f.println("QTABLE:");

  std::vector<std::vector<float>> qTable = agent.getQTable();
    for (int i = 0; i < qTable.size(); i++) {
      for(float weight : qTable[i]) {
        f.print(String(weight) + ",");
      }
      f.print("\n");
    }

  dsp_halt(reason);
  while (1)
    sleep_ms(1000);
}

void dsp_halt(String reason)
{
  display.clear();
  display.writeLines(
      "PROGRAM HALTED",
      "",
      "REASON:",
      reason,
      "");
}

void dsp_loop(InertiaFeedback envInfo, float batteryLvl, int episodeNo) {

  String precisionLvl = "";
  switch (envInfo.precisionLvl)
  {
  case 0:
    precisionLvl = "LOW";
    break;
  case 1:
    precisionLvl = "MID";
    break;
  case 2:
    precisionLvl = "HIGH";
    break;
  }

  display.writeLines(

    "EP: " + String(episodeNo) + "   ||   " + "V: " + String(batteryLvl),
    "",
    "Sensor Mode: " + precisionLvl,
    "",
    envInfo.minsLeftInEp + " Mins Remaining",
    envInfo.energyPercentLeft + "\% Energy Budget"
  );
}