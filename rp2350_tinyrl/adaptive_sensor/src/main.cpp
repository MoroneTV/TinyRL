#include <Arduino.h>
#include "Display.h"



// ===========================================================================================
// FUNCTION DEFINITIONS ======================================================================
// ===========================================================================================

void setup()
{
  // Start Serial, allow time to connect
  Serial.begin(115200);
  sleep_ms(2000);
}

void loop()
{
}


// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{

  Serial.println("Program Halted! Reason: ");
  Serial.println(reason);

  while (1)
    sleep_ms(1000);
}

