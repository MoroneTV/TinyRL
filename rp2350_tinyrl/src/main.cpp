#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>

// TODO:
// 1. Add writing to an SD card or uploading to DB
// 2. Add sensors, Temperature and air quality are probably the best
// 3. Start Figuring out how to insert RL into the code, and how to make it work with the sensors


// ========= Objects =========
Adafruit_MAX17048 battery; // Battery Sensor
Display display;           // Screen

// ========= System Flags =========
struct Flags
{
  bool batterySensorEnabled = false;
};
Flags flags;

// ========= System Intervals =========
struct Intervals
{
  int scanInterval = 2000; // Time between sensor scans in milliseconds
  int displayInterval = 50; // Time between display updates in milliseconds
};
Intervals intervals;

// ========= Most Recent Scans =========
struct Scans
{
  float powerConsumption = -1; // Batter dis/charge rate in %/hr, -1 if battery sensor disabled
};
Scans scans;

// ========= Main Functions =========
void setup();
void loop();
void halt(String reason = "UNSPECIFIED");

// ========= Setup Functions =========
void setupBattery();

// ========= Loop Functions =========
void readSensors();
float getPowerConsumption();
void setSensorResolution(int lvl);

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

  // Setup components
  setupBattery();
}

void loop()
{
  sleep_ms(10);

  static int lastScanTime = millis();
  static int lastDisplayTime = millis();

  if (millis() - lastScanTime >= intervals.scanInterval)
  {
    scans.powerConsumption = getPowerConsumption();
    lastScanTime = millis();
  }

  if (millis() - lastDisplayTime >= intervals.displayInterval)
  {
    dsp_loop(scans.powerConsumption);
    lastDisplayTime = millis();
  }

  // RL Stuff goes here, ability to edit which sensors are on, the intervals
  // and sensor resolution maybe?
  
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  dsp_halt(reason);
  while (1)
    sleep_ms(1000);
}

// Get the battery
float getPowerConsumption()
{
  // Early exit if battery sensor is not enabled
  if (!flags.batterySensorEnabled)
    return -1;

  Serial.println("Battery Charge Rate: " + String(battery.chargeRate()) + "%/hr");
  return battery.chargeRate();
};

void setupBattery()
{
  battery.begin();
  battery.quickStart();
  flags.batterySensorEnabled = true;
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