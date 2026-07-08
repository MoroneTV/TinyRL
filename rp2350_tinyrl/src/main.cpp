#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>

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
  int scanInterval = 1000; // Time between sensor scans in milliseconds
  int displayInterval = 1000; // Time between display updates in milliseconds
};
Intervals intervals;

// ========= Most Recent Scans =========
struct Scans
{
  int batteryPercentage = -1; // Battery percentage, -1 if not available
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
int getBatteryPercentage();
void setSensorResolution(int lvl);

// ========= Display Functions =========
void dsp_setup();
void dsp_loop(int Batterylvl);
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
    int batteryPercentage = getBatteryPercentage();
    scans.batteryPercentage = batteryPercentage;
    lastScanTime = millis();
  }

  if (millis() - lastDisplayTime >= intervals.displayInterval)
  {
    
    dsp_loop(scans.batteryPercentage);
    lastDisplayTime = millis();
  }
  
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  dsp_halt(reason);
  while (1)
    sleep_ms(1000);
}

int getBatteryPercentage()
{

  // Early exit if battery sensor is not enabled
  if (!flags.batterySensorEnabled)
    return -1;

  // Battery sensor's actual percentage calculation is incorrect, calculate manually instead
  if ((battery.cellVoltage()) / 3.6 * 100 > 100)
    return 100; // Cap at 100
  return (battery.cellVoltage()) / 3.6 * 100;
};

void setupBattery()
{
  battery.begin();
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

void dsp_loop(int batteryLevel)
{
  display.clear();
  display.writeLines(
      "Battery: " + String(batteryLevel) + "%",
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