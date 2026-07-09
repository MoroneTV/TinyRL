#include <Arduino.h>
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include "SensorInformation.h"
#include <Wire.h>

// TODO:
// 1. Add writing to an SD card or uploading to DB
// 2. Add sensors, Temperature and air quality are probably the best
// 3. Start Figuring out how to insert RL into the code, and how to make it work with the sensors

//Adafruit_MAX17048 battery; // Battery Sensor
Display display;           // Screen

SensorInformation sensorData;

// ============ MAIN FUNCTIONS ============ //

void setup();
void loop();
void halt(String reason = "UNSPECIFIED");

// ============ AGENT CONTROLS ============ //
void scanSensors();
void updateScanRate(int ms);

void enableLightSensor();
void disableLightSensor();

void enableAirSensor();
void disableAirSensor();

void enableObstacleSensor();
void disableObstacleSensor();

void enableAngleSensor();
void disableAngleSensor();

// ============ AGENT OBSERVATIONS ============ //

float getBatteryLvl();
float getBatteryDrainRate();


// ============ ================== ============ //
// =========== FUNCTION DEFINITIONS =========== //
// ============ ================== ============ //

Adafruit_MAX17048 maxlipo;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);    // wait until serial monitor opens

  Serial.println(F("\nAdafruit MAX17048 simple demo"));

  while (!maxlipo.begin()) {
    Serial.println(F("Couldnt find Adafruit MAX17048?\nMake sure a battery is plugged in!"));
    delay(2000);
  }
  Serial.print(F("Found MAX17048"));
  Serial.print(F(" with Chip ID: 0x")); 
  Serial.println(maxlipo.getChipID(), HEX);
}

void loop() {
  float cellVoltage = maxlipo.cellVoltage();
  if (isnan(cellVoltage)) {
    Serial.println("Failed to read cell voltage, check battery is connected!");
    delay(2000);
    return;
  }
  Serial.print(F("Batt Voltage: ")); Serial.print(cellVoltage, 3); Serial.println(" V");
  Serial.print(F("Batt Percent: ")); Serial.print(maxlipo.cellPercent(), 1); Serial.println(" %");
  Serial.println();

  delay(2000);  // dont query too often!
}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{
  while (1)
    sleep_ms(1000);
}

