#include "PhaseControlViewController.h"
#include "PhaseControl.h"
#include <ESPUI.h>
#include "tools.h"

PhaseControlViewController::PhaseControlViewController(PhaseControl* phaseControl) : 
    sliderId(0),
    btnDownId(0),
    btnUpId(0),
    btnRecalibrateId(0),
    labelStatusId(0),
    labelPhaseValueId(0) {
  this->phaseControl = phaseControl;
}

void PhaseControlViewController::createUI(uint16_t tabId) {
  uint16_t labelStatusId, labelPhaseValueId, sliderTargetValueId;

  this->labelStatusId = ESPUI.addControl(ControlType::Label, "Status", "-", ControlColor::Peterriver, tabId);
  this->labelPhaseValueId = ESPUI.addControl(ControlType::Label, "Current Value", "-", ControlColor::Peterriver, tabId);

  uint16_t panelControlsId = ESPUI.addControl(ControlType::Button, "Controls", "Up", ControlColor::Wetasphalt, tabId, [](Control *sender, int eventname, void *UserInfo) {
    PhaseControlViewController *self = (PhaseControlViewController *)UserInfo;
    if (eventname == B_DOWN) {
      self->setTargetValueAndSlider(ESPUI.getControl(self->sliderId)->value.toInt() + 1);
    }
  }, this);

  this->btnUpId = panelControlsId;

  this->btnDownId = ESPUI.addControl(ControlType::Button, "Down (i)", "Down", ControlColor::Wetasphalt, panelControlsId, [](Control *sender, int eventname, void *UserInfo) {
    PhaseControlViewController *self = (PhaseControlViewController *)UserInfo;
    if (eventname == B_DOWN) {
      self->setTargetValueAndSlider(ESPUI.getControl(self->sliderId)->value.toInt() - 1);
    }
  }, this);


  this->sliderId = ESPUI.addControl(ControlType::Slider, "Target Value", "0", ControlColor::Wetasphalt, tabId, [](Control *sender, int eventname, void *UserInfo) {
    PhaseControlViewController *self = (PhaseControlViewController *)UserInfo;
    if (eventname == SL_VALUE) {
      self->phaseControl->setTargetValue(sender->value.toInt(), true);
    }
  }, this);
  ESPUI.addControl(ControlType::Min, "0", String(0), ControlColor::None, sliderId);
  ESPUI.addControl(ControlType::Max, "100", String(100), ControlColor::None, sliderId);

  uint16_t calibrationId = ESPUI.addControl(ControlType::Label, "Recalibration", "Recalibration takes 15sec!", ControlColor::Wetasphalt, tabId);
  this->btnRecalibrateId =  ESPUI.addControl(ControlType::Button, "Info", "Recalibrate", ControlColor::Wetasphalt, calibrationId, [](Control *sender, int eventname, void *UserInfo) {
    PhaseControlViewController *self = (PhaseControlViewController *)UserInfo;
    if (eventname == B_DOWN) {
      self->phaseControl->recalibrate();
    }
  }, this);

  this->phaseControl->setCurrentValueChangedCallback([](uint8_t phaseValue, void *UserInfo) { 
    PhaseControlViewController *self = (PhaseControlViewController *)UserInfo;
    ESPUI.updateLabel(self->labelPhaseValueId, String(phaseValue));
  }, this);

  this->phaseControl->setStateChangedCallback([](PhaseControlState state, void *UserInfo) { 
    PhaseControlViewController *self = (PhaseControlViewController *)UserInfo;
    Serial.print("State Changed to: ");
    Serial.println(PhaseControl::PhaseControlStateToString(state));

    ESPUI.updateLabel(self->labelStatusId, String(PhaseControl::PhaseControlStateToString(state)));
    if (state == PhaseControlState::Recalibrating) {
      ESPUI.updateSlider(self->sliderId, 0);
      self->disableControls();
    }
    if (state == PhaseControlState::Running) {
      self->enableControls();
    }
  }, this);

  this->phaseControl->setTargetValueChangedCallback([](uint8_t phaseValue, void *UserInfo) { 
    PhaseControlViewController *self = (PhaseControlViewController *)UserInfo;
    ESPUI.updateSlider(self->sliderId, phaseValue);
  }, this);

  ESPUI.updateLabel(this->labelStatusId,  "Startup");
  ESPUI.updateLabel(this->labelPhaseValueId,  "0");
}

void PhaseControlViewController::disableControls() {
  ESPUI.setEnabled(this->sliderId, false);
  ESPUI.setEnabled(this->btnDownId, false);
  ESPUI.setEnabled(this->btnUpId, false);
  ESPUI.setEnabled(this->btnRecalibrateId, false);
}

void PhaseControlViewController::enableControls() {
  ESPUI.setEnabled(this->sliderId, true);
  ESPUI.setEnabled(this->btnDownId, true);
  ESPUI.setEnabled(this->btnUpId, true);
  ESPUI.setEnabled(this->btnRecalibrateId, true);
}

void PhaseControlViewController::setTargetValueAndSlider(int8_t newTargetValue) {
  newTargetValue = min(100, max(0, (int)newTargetValue));
  ESPUI.updateSlider(this->sliderId, newTargetValue);
  phaseControl->setTargetValue(newTargetValue, true);
}

void PhaseControlViewController::update() {
  if (phaseControl->getState() == PhaseControlState::Recalibrating) {
    ESPUI.updateLabel(this->labelStatusId, "Recalibrating - " + millisToReadable(phaseControl->getRecalibratingTimeLeft()));
  }
}