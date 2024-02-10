#include <Arduino.h>
#include <Preferences.h>
#include <ESPUI.h>

#include "Api.h"
#include "PhaseControl.h"
#include "PhaseControlViewController.h"
#include "WifiModule.h"
#include "WifiModuleViewController.h"
#include "Config.h"

#define VERSION "1.0"

const uint8_t PIN_PHASE_DOWN = 33; // Green
const uint8_t PIN_PHASE_UP = 14; // Yellow

const char* hostname = "PhaseControl";
const char* configNamespace = "PhaseControl";

uint16_t tabPhaseControl, tabWifiSettings;

Preferences *preferences;

bool firstRun = true;

Config *config = new Config();

Api *api = new Api();

PhaseControl *phaseControl = new PhaseControl(PIN_PHASE_DOWN, PIN_PHASE_UP);
PhaseControlViewController *phaseControlViewController = new PhaseControlViewController(phaseControl);
WifiModule *wifi = new WifiModule();
WifiModuleViewController *wifiModuleViewController = new WifiModuleViewController(config);

void setup() {
  Serial.begin(115200);
  Serial.print("Version: ");
  Serial.println(VERSION);

  Serial.println("Setup");

  preferences = new Preferences();
  config->setup(preferences, configNamespace);

  phaseControl->setup();

  wifi->setHostname((char *)hostname);
  wifi->connect(config->getWifiSSID(), config->getWifiPassword());

  //Turn off verbose debugging
  ESPUI.setVerbosity(Verbosity::Quiet);

  uint16_t tabPhaseControl = ESPUI.addControl(ControlType::Tab, "Phase Control", "Phase Control");
  uint16_t tabWifiSettings = ESPUI.addControl(ControlType::Tab, "Wifi Settings", "Wifi Settings");

  phaseControlViewController->createUI(tabPhaseControl);
  wifiModuleViewController->createUI(tabWifiSettings);

  ESPUI.addControl(ControlType::Label, "Version", VERSION, ControlColor::Dark, tabWifiSettings);

  ESPUI.begin("Phase Control");

  api->setup(ESPUI.server, phaseControl);
}

void loop() {
  wifi->update();
  phaseControl->update();
  phaseControlViewController->update();
  wifiModuleViewController->update();

  if (firstRun) {
    Serial.println("First Run - starting recalibration");

    firstRun = false;
    phaseControl->recalibrate();
  }
}
