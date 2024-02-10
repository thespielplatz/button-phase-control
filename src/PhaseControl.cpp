#include "PhaseControl.h"
#include "tools.h"
#include "Api.h"

#include <Arduino.h>

const uint32_t BUTTON_HOLD_DURATION = 150;
const uint32_t WAIT_FOR_NEXT_PRESS = 150;
const uint32_t RECALIBRATE_WAIT_TIME = 15 * 1000; // 15 sec

PhaseControl::PhaseControl(uint8_t _pinPhaseDown, uint8_t _pinPhaseUp) : 
  pinPhaseDown(_pinPhaseDown),
  pinPhaseUp(_pinPhaseUp),
  currentValueChangedCallback(NULL),
  stateChangedCallback(NULL),
  state(PhaseControlState::Starting) {

  this->currentValue = 0;
  this->targetValue = 0;
  this->timeWhenUpdated = millis();
}

void PhaseControl::setup() {
  pinMode(this->pinPhaseUp, OUTPUT);
  pinMode(this->pinPhaseDown, OUTPUT);
  digitalWrite(this->pinPhaseUp, LOW); 
  digitalWrite(this->pinPhaseDown, LOW); 

  this->setState(PhaseControlState::Running);
}

void PhaseControl::setState(PhaseControlState newState) {
  this->state = newState;
  if (this->stateChangedCallback != NULL) {
    this->stateChangedCallback(state, this->stateChangedCallbackUserInfo);
  }
}


void PhaseControl::setCurrentValue(uint8_t value) {
  this->currentValue = max(min((int)value, 100), 0);
  if (this->currentValueChangedCallback != NULL) {
    this->currentValueChangedCallback(this->currentValue, this->currentValueChangedCallbackUserInfo);
  }
}

uint8_t PhaseControl::getCurrentValue() {
  return this->currentValue;
}


void PhaseControl::stepDown() {
  digitalWrite(this->pinPhaseDown, HIGH); 
  delay(BUTTON_HOLD_DURATION);                      
  digitalWrite(this->pinPhaseDown, LOW);  
  delay(WAIT_FOR_NEXT_PRESS);                      

  this->setCurrentValue(this->currentValue - 1);
}

void PhaseControl::stepUp() {
  digitalWrite(this->pinPhaseUp, HIGH); 
  delay(BUTTON_HOLD_DURATION);                      
  digitalWrite(this->pinPhaseUp, LOW);  
  delay(WAIT_FOR_NEXT_PRESS);     

  this->setCurrentValue(this->currentValue + 1);
}

void PhaseControl::setTargetValue(int8_t value, bool skipCallback) {
  this->targetValue = max(min((int)value, 100), 0);
  Serial.print("Setting target to: ");
  Serial.println(this->targetValue);

  if (!skipCallback && this->targetValueChangedCallback != NULL) {
    this->targetValueChangedCallback(this->targetValue, this->targetValueChangedCallbackUserInfo);
  }
}

uint8_t PhaseControl::getTargetValue() {
  return this->targetValue;
}

void PhaseControl::recalibrate() {
  this->recalibratingTimeLeft = RECALIBRATE_WAIT_TIME;
  this->setState(PhaseControlState::Recalibrating);
  digitalWrite(this->pinPhaseDown, HIGH); 
  Serial.println("Recalibrating");

  this->setCurrentValue(0);
  this->setTargetValue(0);
}  

int32_t PhaseControl::getRecalibratingTimeLeft() {
  return this->recalibratingTimeLeft;
}


void PhaseControl::update() {
  unsigned long deltaTime = millis() - this->timeWhenUpdated;
  this->timeWhenUpdated = millis();

  if (this->state == PhaseControlState::Recalibrating) {
    if (this->recalibratingTimeLeft <= 0) {
      this->recalibratingTimeLeft = 0;
      this->setState(PhaseControlState::Running);
      digitalWrite(this->pinPhaseDown, LOW); 
    }
    this->recalibratingTimeLeft -= deltaTime;
    return;
  }

  int8_t diff = this->targetValue - this->currentValue;
    
  if (diff != 0) {
    Serial.print("currentValue: ");
    Serial.print(this->currentValue);
    Serial.print(" targetValue: ");
    Serial.print(this->targetValue);
    Serial.println("");
  }

  if (diff > 0) {
    this->stepUp();
  }
  if (diff < 0) {
    this->stepDown();
  }
}

void PhaseControl::setCurrentValueChangedCallback(std::function<void(uint8_t, void *UserInfo)> phaseChangedCallback, void *UserInfo) {
  this->currentValueChangedCallback = phaseChangedCallback;
  this->currentValueChangedCallbackUserInfo = UserInfo;
}

void PhaseControl::setStateChangedCallback(std::function<void(PhaseControlState, void *UserInfo)> stateChangedCallback, void *UserInfo) {
  this->stateChangedCallback = stateChangedCallback;
  this->stateChangedCallbackUserInfo = UserInfo;
}

void PhaseControl::setTargetValueChangedCallback(std::function<void(uint8_t, void *UserInfo)> targetValueChangedCallback, void *UserInfo) {
  this->targetValueChangedCallback = targetValueChangedCallback;
  this->targetValueChangedCallbackUserInfo = UserInfo;
}

PhaseControlState PhaseControl::getState() {
  return this->state;
}

const char* PhaseControl::PhaseControlStateToString(PhaseControlState state) {
  switch (state){
      case PhaseControlState::Starting: return "Starting";
      case PhaseControlState::Running: return "Running";
      case PhaseControlState::Recalibrating: return "Recalibrating";
  }

  return "";
}
