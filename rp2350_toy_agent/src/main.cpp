#include <Arduino.h>
#include "Display.h"
#include <Wire.h>

// ============ SYSTEM OBJECTS AND VARS ============ //

Display display; // Screen wrapper class

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
