#include "secrets.h"
#include <ESPUI.h>
#include "PhaseControl.h"
#include "WifiModule.h"
#include "tools.h"

const uint8_t PIN_PHASE_DOWN = 33; // Green
const uint8_t PIN_PHASE_UP = 14; // Yellow

const char* hostname = "PhaseControl";

uint16_t labelStatusId, labelPhaseValueId, sliderTargetValueId, sliderId, btnDownId, btnUpId, btnRecalibrateId;

PhaseControl *phaseControl = new PhaseControl(PIN_PHASE_DOWN, PIN_PHASE_UP);
WifiModule *wifi = new WifiModule();

void disableControls() {
  ESPUI.setEnabled(sliderId, false);
  ESPUI.setEnabled(btnDownId, false);
  ESPUI.setEnabled(btnUpId, false);
  ESPUI.setEnabled(btnRecalibrateId, false);
}

void enableControls() {
  ESPUI.setEnabled(sliderId, true);
  ESPUI.setEnabled(btnDownId, true);
  ESPUI.setEnabled(btnUpId, true);
  ESPUI.setEnabled(btnRecalibrateId, true);
}

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
  labelStatusId = ESPUI.label("Status", ControlColor::Peterriver, "Startup");
  labelPhaseValueId = ESPUI.label("Phase Control", ControlColor::Peterriver, "-");

  // ESPUI.separator("Control");
  uint16_t panelControlsId = ESPUI.button("Controls", [](Control *sender, int eventname) {
    if (eventname == B_DOWN) {
      phaseControl->setTargetValue(phaseControl->getTargetValue() + 1);
    }
  }, ControlColor::Wetasphalt, "Up");

  btnUpId = panelControlsId;

  btnDownId = ESPUI.addControl(ControlType::Button, "Down (i)", "Down", ControlColor::Wetasphalt, panelControlsId, [](Control *sender, int eventname) {
    if (eventname == B_DOWN) {
      phaseControl->setTargetValue(phaseControl->getTargetValue() - 1);
    }
  });

  sliderId = ESPUI.addControl(ControlType::Slider, "Set to value", "0", ControlColor::Wetasphalt, Control::noParent, [](Control *sender, int eventname) {
    if (eventname == SL_VALUE) {
      phaseControl->setTargetValue(sender->value.toInt());
    }
  });
  ESPUI.addControl(ControlType::Min, "0", String(0), ControlColor::None, sliderId);
  ESPUI.addControl(ControlType::Max, "100", String(100), ControlColor::None, sliderId);

  uint16_t calibrationId = ESPUI.addControl(ControlType::Label, "Recalibration", "Recalibration takes 15sec!", ControlColor::Wetasphalt, Control::noParent);
  btnRecalibrateId =  ESPUI.addControl(ControlType::Button, "Info", "Recalibrate", ControlColor::Wetasphalt, calibrationId, [](Control *sender, int eventname) {
    if (eventname == B_DOWN) {
      phaseControl->recalibrate();
    }
  });

  ESPUI.begin("ESPUI Control");

  ESPUI.updateLabel(labelStatusId,  "Startup");
  ESPUI.updateLabel(labelPhaseValueId,  "0");

  phaseControl->setPhaseChangedCallback([](uint8_t phaseValue) { 
    ESPUI.updateLabel(labelPhaseValueId, String(phaseValue));
  });
  phaseControl->setStateChangedCallback([](PhaseControlState state) { 
    Serial.print("State Changed to: ");
    Serial.println(PhaseControl::PhaseControlStateToString(state));

    ESPUI.updateLabel(labelStatusId, String(PhaseControl::PhaseControlStateToString(state)));
    if (state == PhaseControlState::Recalibrating) {
      ESPUI.updateSlider(sliderId, 0);
      disableControls();
    }
    if (state == PhaseControlState::Running) {
      enableControls();
    }
  });
}

void loop() {
  wifi->update();
  phaseControl->update();

  if (phaseControl->getState() == PhaseControlState::Recalibrating) {
    ESPUI.updateLabel(labelStatusId, "Recalibrating - " + millisToReadable(phaseControl->getRecalibratingTimeLeft()));
  }
}
