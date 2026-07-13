#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>
#include "InertiaSensor.h"

// ========= Objects and Globals =========
Adafruit_MAX17048 battery; // Battery Sensor
Display display;           // Screen
InertiaSensor inertia;

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
  inertia.begin();

}

void setupBattery();
void defineRuntimePermutation();

void loop()
{
  delay(500);
  InertiaReadings r = inertia.read();
  Serial.println("Accel: " + String(r.aX) + ", " + String(r.aY) + ", " + String(r.aZ));
  Serial.println("Gyro: " + String(r.gX) + ", " + String(r.gY) + ", " + String(r.gZ));

  display.writeLines(
      "Accel: " + String(r.aX) + ", " + String(r.aY) + ", " + String(r.aZ),
      "Gyro: " + String(r.gX) + ", " + String(r.gY) + ", " + String(r.gZ));
}


// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
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