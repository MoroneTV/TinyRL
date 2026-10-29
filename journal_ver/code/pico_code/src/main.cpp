#include <Arduino.h>
#include <Wire.h>
#include "AdaptiveSystem.h"
#include <Adafruit_INA260.h>
#include <consts.h>
#include <Display.h>

SystemConfig* config = nullptr;
Display display;
// Network network, maybe?
AdaptiveSystem* adaptiveSystem = nullptr;

void loop();

void setup();
bool setup_SerialAndWire();
bool setup_createConfig();
bool setup_buildAdaptiveSystem();
// bool setup_connectToNetwork(); assumes we're doing networking, come back to later in project
bool setup_enableDisplay();

void halt(String reason = "UNSPECIFIED");
//////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINITIONS ///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

// Setup objects in order of necessity.
void setup()
{
  if (setup_SerialAndWire())
    Serial.println("SETUP: Serial and wire connected!");
  else
    halt("SETUP: Serial and wire setup failed!");

  if (setup_createConfig())
    Serial.println("SETUP: Config Object Created!");
  else
    halt("SETUP: Config creation failed!");

  if (setup_enableDisplay())
    Serial.println("SETUP: Display setup complete!");
  else
    halt("SETUP: Display setup failed!");

  if (setup_buildAdaptiveSystem())
    Serial.println("SETUP: Adaptive system build complete!");
  else
    halt("SETUP: Adaptive system build failed!");

  
}

// Enables Wire and Serial Comms
bool setup_SerialAndWire()
{
  Serial.begin(115200);
  delay(3000); // Small delay to give time to switch to monitor if needed.

  Serial.println("SETUP, WIRE: Configuring I2C");

  // free pins on the side of the device- GP0 and GP1, set up for I2C
  if (!Wire.setSDA(0) || !Wire.setSCL(1))
  {
    Serial.println("SETUP, WIRE: SDA | SCL Pin allocation failed!");
    return false;
  }

  Wire.begin();
  return true;
}

// Create config struct containing all system consts, can be edited in consts.h
bool setup_createConfig()
{
  SystemConfig masterConfig;

  // Package Sensor Configs
  LightSensorConfig light;
  light.readingBufferLength = LIGHT_BUFFER_LEN;
  light.weight = LIGHT_WEIGHT;

  InertialSensorConfig inertia;
  inertia.readingBufferLength = INERTIA_BUFFER_LEN;
  inertia.weight = INERTIA_WEIGHT;

  ClimateSensorConfig climate;
  climate.readingBufferLength = CLIMATE_BUFFER_LEN;
  climate.weight = CLIMATE_WEIGHT;
  climate.upperTempBound = CLIMATE_MAX_BOUND;
  climate.lowerTempBound = CLIMATE_MIN_BOUND;

  SensorConfig sensorConfig;
  sensorConfig.climate = climate;
  sensorConfig.inertia = inertia;
  sensorConfig.light = light;

  masterConfig.sensorConfig = sensorConfig;

  // Controller Config
  masterConfig.controllerType = CONTROLLER_TYPE;
  masterConfig.operationMode = OPERATION_MODE;

  // Hyperparams
  masterConfig.conservation = CONSERVATION_SCALER;

  // Assign to global config obj
  config = new SystemConfig(masterConfig);
  return true;
}

// Enables the display, introduces 2 seconds delay for the splash screen to clear.
bool setup_enableDisplay()
{
  if (display.begin())
  {
    delay(2000);
    return true;
  }
  Serial.println("SETUP, DISPLAY: Display begin failed!");
  return false;
}

bool setup_buildAdaptiveSystem() {
  adaptiveSystem = new AdaptiveSystem(*config);
  return adaptiveSystem != nullptr;
}


void loop()
{
  delay(1000);
}

// Display a halt message to screen and serial then halt the program, unspecified by default
void halt(String reason)
{
  display.writeLines(
      "Program Halted!",
      "",
      "Reason:",
      reason,
      "");

  Serial.println("");
  Serial.println("PROGRAM HALTED, REASON PROVIDED:");
  Serial.println(reason);
  Serial.println("");

  // Hang indefinitely
  while (1)
    sleep_ms(1000);
}
