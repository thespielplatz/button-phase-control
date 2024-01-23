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

  // ESPUI.separator("Status");
  labelStatusId = ESPUI.label("Status", ControlColor::Peterriver, "START");
  labelPhaseValueId = ESPUI.label("Phase Control", ControlColor::Peterriver, "-");

  // ESPUI.separator("Control");
  uint16_t panelControlsId = ESPUI.button("Controls", [](Control *sender, int eventname) {
    if (eventname == B_DOWN) {
      phaseControl->setTargetValue(phaseControl->getTargetValue() + 1);
    }
  }, ControlColor::Wetasphalt, "Up");

  ESPUI.addControl(ControlType::Button, "Down (i)", "Down", ControlColor::Wetasphalt, panelControlsId, [](Control *sender, int eventname) {
    if (eventname == B_DOWN) {
      phaseControl->setTargetValue(phaseControl->getTargetValue() - 1);
    }
  });

  uint16_t sliderId = ESPUI.addControl(ControlType::Slider, "Set to value", String(0), ControlColor::Wetasphalt, Control::noParent, [](Control *sender, int eventname) {
    if (eventname == SL_VALUE) {
      phaseControl->setTargetValue(sender->value.toInt());
    }
  });
  ESPUI.addControl(ControlType::Min, "0", String(0), ControlColor::None, sliderId);
  ESPUI.addControl(ControlType::Max, "100", String(100), ControlColor::None, sliderId);

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
