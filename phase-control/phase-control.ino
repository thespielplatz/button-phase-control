#include "secrets.h"
#include <Arduino.h>
#include <ESPUI.h>
#include "PhaseControl.h"
#include "PhaseControlViewController.h"
#include "WifiModule.h"

const uint8_t PIN_PHASE_DOWN = 33; // Green
const uint8_t PIN_PHASE_UP = 14; // Yellow

const char* hostname = "PhaseControl";

uint16_t tabPhaseControl, tabWifiSettings;

PhaseControl *phaseControl = new PhaseControl(PIN_PHASE_DOWN, PIN_PHASE_UP);
PhaseControlViewController *phaseControlViewController = new PhaseControlViewController(phaseControl);
WifiModule *wifi = new WifiModule();

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  phaseControl->setup();

  wifi->setHostname((char *)hostname);
  wifi->connect(WIFI_SSID, WIFI_PASSWORD);

  //Turn off verbose debugging
	ESPUI.setVerbosity(Verbosity::Quiet);

  uint16_t tabPhaseControl = ESPUI.addControl(ControlType::Tab, "Phase Control", "Phase Control");
  uint16_t tabWifiSettings = ESPUI.addControl(ControlType::Tab, "Wifi Settings", "Wifi Settings");

  phaseControlViewController->createUI(tabPhaseControl);

  ESPUI.begin("Phase Control");
}

void loop() {
  wifi->update();
  phaseControl->update();
  phaseControlViewController->update();
}
