#include <Arduino.h>
#include "Display.h"
#include "Environment.h"
#include "QAgent.h"
#include "Adafruit_MAX1704X.h" // Battery Library
#include "Display.h"
#include "config.h" // Global consts
#include "debug.h"
#include "SensorWeights.h"
#include <Wire.h>
#include <ConfigValues.h>
#include <ColdStorage.h>

// ===========================================================================================
// GLOBAL OBJECTS ============================================================================
// ===========================================================================================

Adafruit_MAX17048 battery;
Display display;
ColdStorage storage;

QAgent *agent;
Environment *env;
bool sdAvailable = false;

ConfigValues config = {};

// ===========================================================================================
// FUNCTION DECLARATIONS =====================================================================
// ===========================================================================================

void setup();
void setup_display();
void setup_battery();
void setup_SD();
void setup_config();

void loop();                 // Agent completes (EP_COUNT) number of episodes, then halts
void loop_completeEpisode(); // Agent completes one full episode

void loop_display(int action, OutputData op, float batterylvl, float reward);  // Display screen during loop

void halt(String reason = "UNSPECIFIED");

// ===========================================================================================
// FUNCTION DEFINITIONS ======================================================================
// ===========================================================================================

void setup()
{
  // Start Serial, allow time to connect
  Wire.begin();
  Serial.begin(115200);
  sleep_ms(2000);

  setup_display();
  setup_battery();

  // Read in configs, either from configs.h or the SD card
  setup_SD();
  setup_config();

  // Populate and setup objects.
  SensorWeights weights(config.weights);
  env = new Environment(weights, config.conservatism, config.energyPerEp, config.msPerEp);
  agent = new QAgent(config.epsilon, config.gamma, config.lr, config.noOfActions, config.noOfStates);

  // If we're loading from an exisitng project, we have a qTable to work on already.
  if (LOAD_EXISTING_PROJ)
    agent->setQTable(config.qTable);

  env->begin();
}

void setup_battery() {
  if (battery.begin()) D_println("Battery setup complete");
  else halt("Battery Sensor Failure!");
}

void setup_display() {
  if (display.begin()) D_println("Display setup complete");
  else halt("Display Setup Failure!");
}

void setup_SD()
{
  if (storage.begin())
  {
    D_println("SD CARD AVAILABLE");
    sdAvailable = true;
    storage.setTargetRLFile(PROJ_FILE);
  }
  else
  {
    D_println("SD CARD UNAVAILABLE");
  }

  // SD failure is only pertinent if the user wants to use the SD card.
  if ((LOAD_EXISTING_PROJ || SAVE_PROJ_PROGRESS) && !sdAvailable)
    halt("Cold storage operations enabled but SD unavailable");
}

void setup_config()
{
  // Read config from SD
  if (LOAD_EXISTING_PROJ && sdAvailable)
    config = storage.readFromRLFile();

  // Read config from config.h
  else
  {
    config.conservatism = CONSERVATISM;
    config.energyPerEp = ENERGY_PER_EP;
    config.msPerEp = MS_PER_EP;
    config.epsilon = EPSILON;
    config.gamma = GAMMA;
    config.lr = LR;
    config.noOfActions = NO_OF_ACTIONS;
    config.noOfStates = NO_OF_STATES;
    config.weights = {INERTIA_WEIGHT,
                      AIR_QUALITY_WEIGHT,
                      OBSTACLE_WEIGHT};
  }
}

void loop()
{
  for (int i = 1; i <= EP_COUNT; i++)
  {
    loop_completeEpisode();
  }
  halt("Training Complete.");
}

void loop_completeEpisode()
{
  // Create fresh environment
  int state = env->resetEnv().encodedState;
  bool isDone = false;

  // Training loop
  while (!isDone)
  {
    // Take action based on state, get response
    int action = agent->selectAction(state);
    Feedback response = env->step(action);

    // Agent learns based on how environment responds
    agent->update(
        state,
        action,
        response.reward,
        response.newState);

    // Update state and check if the episode is over.
    state = response.newState;
    isDone = response.isDone;

    loop_display(action, response.outputData, battery.cellPercent(), response.reward);
  }

  // Save updated Q table to SD, if enabled.
  if (SAVE_PROJ_PROGRESS && sdAvailable)
  {
    config.qTable = agent->getQTable();
    storage.writeToRLFile(config);
  }
}

void loop_display(int action, OutputData op, float batterylvl, float reward) {

  display.writeLines(
    "Action: " + String(action),
    "Reward: " + String(reward),
    "Battery: " + String(batterylvl) + "%",
    "Energy left: " + String(int(op.energyRemaining)),
    "Mins in Ep: " + String(int(op.msRemaining/60000)),

    "AirQ. Lvl: " + String(op.airQLvl),
    "Light Lvl: " + String(op.lightLvl),
    "Inert. Lvl: " + String(op.inertiaLvl)
  );

}

// Display a halt message and halt the program, unspecified by default
void halt(String reason)
{

  Serial.println("Program Halted! Reason: ");
  Serial.println(reason);

  while (1)
    sleep_ms(1000);
}
