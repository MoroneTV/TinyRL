#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>


// TODO:
// 1. Add writing to an SD card or uploading to DB
// 2. Add sensors, Temperature and air quality are probably the best
// 3. Start Figuring out how to insert RL into the code, and how to make it work with the sensors



// ========= Objects and Globals =========
Adafruit_MAX17048 battery; // Battery Sensor
Display display;           // Screen

int voltzPerEpisode;

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

void smoothVoltage();
void completeEpisode();
void exportQTable();
void updateResultsFile();

// ========= Display Functions =========
void dsp_setup();
void dsp_loop(float powerConsumption);
void dsp_halt(String reason = "UNSPECIFIED");

//
// FUNCTION DEFINITIONS
//

void setup()
{
  // Start Serial, allow time to connect
  Serial.begin(115200);
  sleep_ms(3000);

  // Start Display and show message
  display.begin();
  dsp_setup();

}

void setupBattery();
void setupSDcard();
void defineRuntimePermutation();
void importQtable();

void loop()
{

}

// Converts voltage measurement into a rolling average to smooth it out.
void smoothVoltage();

// Runs an episode to completion
void completeEpisode();

// Exports the contents to a Q file to an SD card
void exportQTable();

// Writes information about each timestep to an SD card
void updateResultsFile();

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  dsp_halt(reason);
  while (1)
    sleep_ms(1000);
}

void dsp_setup()
{
  display.clear();
  display.writeLines(
      "TinyRL",
      "Setup in Progress",
      "",
      "",
      "");
}

void dsp_loop(float powerConsumption)
{
  display.clear();
  display.writeLines(
      "Charge: " + String(powerConsumption) + "%/hr",
      "Voltage: " + String(battery.cellVoltage()) + "V",
      "",
      "",
      "");
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