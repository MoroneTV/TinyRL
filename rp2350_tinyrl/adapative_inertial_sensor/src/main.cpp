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

  // 1. Data Collection Period
  int delayBetweenReadings = inertia.getTimeBetweenReadings();
  InertiaReadings dataBuffer[250];
  int idx = 0;

  // Collect data for 1 second
  int dataCollectionTime = millis();
  while (millis() - dataCollectionTime < 1000)
  {
    dataBuffer[idx] = inertia.read();
    idx++;

    // Delay matches the read rate of the sensor.
    delay(delayBetweenReadings);
  }

  // 2. Process Data
  int precisionLvl = inertia.getPrecisionLvl();

  // Get variance of the magnitude of the acceleration vector
  float mean = 0.0f;
  for (int i = 0; i < idx; i++) 
  {
    float accelMag = sqrt(pow(dataBuffer[i].aX, 2) + pow(dataBuffer[i].aY, 2) + pow(dataBuffer[i].aZ, 2));
    mean += accelMag;
  }
  mean /= idx;

  float variance = 0.0f;
  for (int i = 0; i < idx; i++) {
    float accelMag = sqrt(pow(dataBuffer[i].aX, 2) + pow(dataBuffer[i].aY, 2) + pow(dataBuffer[i].aZ, 2));
    variance += pow(accelMag - mean, 2);
  }
  variance /= idx;

  // 3. Reset buffer for next loop
  for (int i = 0; i < idx; i++)
  {
    dataBuffer[i] = InertiaReadings{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  }
  idx = 0;

  Serial.println("VARIANCE: " + String(variance));

  // 3. Agent Decision

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