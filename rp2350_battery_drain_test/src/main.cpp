#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include <Wire.h>
#include <SD.h>

#define RUNTIME_MS 60 * 60000
#define READ_INTERVAL_MS 1000

Adafruit_MAX17048 battery; // Battery Sensor
Display display;           // Screen

String fileName = "";
float startingVoltage;

// ============ MAIN FUNCTIONS ============ //

void setup();
void loop();
void halt(String reason = "UNSPECIFIED");

// ============ SETUP FUNCTIONS ============ //
bool setupBattery();
bool setupSDcard();
void initialiseFile();

// ============ HELPER FUNCTIONS ============ //
void writeToFile(int time, float voltage);

void dsp_setupScreen();
void dsp_loopScreen(float timeLeft, float voltage);
void dsp_haltScreen(String reason);

// ============ ================== ============ //
// =========== FUNCTION DEFINITIONS =========== //
// ============ ================== ============ //

void setup()
{
  Serial.begin(115200);
  delay(2000);

  Serial.println("SERIAL CONNECTED");

  display.begin();
  dsp_setupScreen();

  Serial.println("DISPLAY CONNECTED");

  setupBattery();
  setupSDcard();
  initialiseFile();

  Serial.println("SETUP COMPLETE");
  startingVoltage = battery.cellVoltage();
}

void loop()
{
  delay(READ_INTERVAL_MS);

  int now = millis();
  float voltage = battery.cellVoltage();

  float minsLeft = (RUNTIME_MS - now) / 60000;
  if (minsLeft < 0)
    halt("Time Limit Reached!");

  dsp_loopScreen(minsLeft, voltage);
  writeToFile(now, voltage);
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  dsp_haltScreen(reason);
  while (1)
    sleep_ms(1000);
}

bool setupBattery()
{
  if (!battery.begin())
    return false;
  return true;
}

bool setupSDcard()
{
  if (!SD.begin(SS))
  {
    halt("SD CARD FAILURE");
    return false;
  };
  return true;
}

void initialiseFile()
{

  int runNo = 1;
  bool freeSpace = false;
  fileName = String(String(runNo) + ".txt");

  while (SD.exists(fileName))
  {
    runNo++;
    fileName = String(String(runNo) + ".txt");
  }

  File data = SD.open(fileName, FILE_WRITE);
  data.println("RUNTIME (MS), VOLTAGE (V)");
  data.close();
}

void writeToFile(int time, float voltage)
{
  File data = SD.open(fileName, FILE_WRITE);

  String dataEntry = String(time) + "," + String(voltage);

  data.println(dataEntry);
  data.close();
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

void dsp_loopScreen(float timeLeft, float voltage)
{
  display.writeLines(
      "Starting Voltz: " + String(startingVoltage) + "V",
      "Current Voltsz: " + String(voltage) + "V",
      "",
      String(timeLeft) + " Mins Remaining",
      "Write to: " + fileName,
      "Every " + String(READ_INTERVAL_MS) + "ms");
}

void dsp_haltScreen(String reason)
{
  display.writeLines(
      "Program Halted!",
      "",
      "Reason:",
      reason,
      "");
}