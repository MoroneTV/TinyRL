#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>
#include <SD.h>

#include "AdaptiveSystem.h"

#define READ_INTERVAL_MS 1000

Display display;
Adafruit_MAX17048 battery;
float startingVoltage = 0.0;

void setup();
void loop();
void halt(String reason = "UNSPECIFIED");

bool setupBattery();
void dsp_setupScreen();
void dsp_loopScreen();

//////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINITIONS ///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

// Setup objects in order of necessity.
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Serial & Wire Initialised.");

}

void loop()
{
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  display.writeLines
  (
      "Program Halted!",
      "",
      "Reason:",
      reason,
      ""
    );

  while (1)
    sleep_ms(1000);
}
