#include "secrets.h"
#include <ESPUI.h>
#include "PhaseControl.h"
#include "WifiModule.h"

const uint8_t PIN_PHASE_DOWN = 33; // Green
const uint8_t PIN_PHASE_UP = 14; // Yellow

const char* hostname = "PhaseControl";

uint16_t labelStatusId, labelPhaseValueId, sliderTargetValueId;

PhaseControl *phaseControl = new PhaseControl(PIN_PHASE_DOWN, PIN_PHASE_UP);
WifiModule *wifi = new WifiModule();

void test(Control *sender, int eventname) {

}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  phaseControl->setup();

  wifi->setHostname((char *)hostname);
  wifi->connect(WIFI_SSID, WIFI_PASSWORD);

  //Turn off verbose debugging
	ESPUI.setVerbosity(Verbosity::Quiet);

  ESPUI.separator("Status");
  labelStatusId = ESPUI.label("Status", ControlColor::Peterriver, "START");
  labelPhaseValueId = ESPUI.label("Phase", ControlColor::Peterriver, "-");

  ESPUI.separator("Control");

  ESPUI.button("Up", [](Control *sender, int eventname) {
    if (eventname = B_DOWN) {
      phaseControl->setTargetValue(phaseControl->getTargetValue() + 1);
    }
  }, ControlColor::Wetasphalt, "+1");

  ESPUI.button("Down", [](Control *sender, int eventname) {
    if (eventname = B_DOWN) {
      phaseControl->setTargetValue(phaseControl->getTargetValue() - 1);
    }
  }, ControlColor::Wetasphalt, "-1");

  sliderTargetValueId = ESPUI.slider("Set to", NULL, ControlColor::Alizarin, 0, 0, 100);
  ESPUI.button("Set", [](Control *sender, int eventname) {
    if (eventname = B_DOWN) {
      Control *slider = ESPUI.getControl(sliderTargetValueId);
      phaseControl->setTargetValue(slider->value.toInt());
    }
  }, ControlColor::Wetasphalt, "Set");

  ESPUI.begin("ESPUI Control");

  ESPUI.updateLabel(labelStatusId,  "Startup");

  phaseControl->setPhaseChangedCallback([](uint8_t phaseValue) { 
    ESPUI.updateLabel(labelPhaseValueId, String(phaseValue));
  });
}

void loop() {
  wifi->update();
  phaseControl->update();
}
