#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>
#include <SD.h>

// ============ CONSTANTS ============ //

#define READ_INTERVAL_MS 1000

// ============ GLOBAL VARS ============ //
Display display;
Adafruit_MAX17048 battery;
float startingVoltage = 0.0;

// ============ MAIN FUNCTIONS ============ //

void setup();
void loop();
void halt(String reason = "UNSPECIFIED");


// ============ HELPER FUNCTIONS ============ //
bool setupBattery();
void dsp_setupScreen();
void dsp_loopScreen();


// ============ ================== ============ //
// ===========  CORE FUNCTION DEFINITIONS =========== //
// ============ ================== ============ //

// Setup objects in order of necessity.
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  delay(2000);  // Serial can take a few seconds to start
  Serial.println("SERIAL & WIRE CONNECTED");

  display.begin();
  Serial.println("DISPLAY CONNECTED");

  setupBattery();
  Serial.println("SETUP COMPLETE");
  startingVoltage = battery.cellVoltage();
}

void loop()
{
  delay(READ_INTERVAL_MS);
  dsp_loopScreen();
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  display.writeLines(
      "Program Halted!",
      "",
      "Reason:",
      reason,
      "");
  while (1)
    sleep_ms(1000);
}

// ============ ================== ============ //
// ===========  HELPER FUNCTION DEFINITIONS =========== //
// ============ ================== ============ //

bool setupBattery()
{
  if (!battery.begin())
    return false;
  return true;
}

void dsp_setupScreen()
{
  display.writeLines(
      "",
      "SETUP IN PROGRESS",
      "",
      "",
      "");
}

void dsp_loopScreen()
{
  display.writeLines(
      "Battery Voltage:",
      String(battery.cellVoltage()) + " V",
      "Battery Percent:",
      String(battery.cellPercent()) + " %",
      "");
}
