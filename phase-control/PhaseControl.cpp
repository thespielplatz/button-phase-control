#include "PhaseControl.h"

#include <Arduino.h>

const uint8_t PhaseControl::BUTTON_HOLD_DURATION = 150;
const uint8_t PhaseControl::WAIT_FOR_NEXT_PRESS = 150;

PhaseControl::PhaseControl(uint8_t _pinPhaseDown, uint8_t _pinPhaseUp) : 
  pinPhaseDown(_pinPhaseDown),
  pinPhaseUp(_pinPhaseUp),
  phaseChangedCallback(NULL) {

  this->currentValue = 0;
  this->targetValue = 0;
}

void PhaseControl::setup() {
  pinMode(this->pinPhaseUp, OUTPUT);
  pinMode(this->pinPhaseDown, OUTPUT);
  digitalWrite(this->pinPhaseUp, LOW); 
  digitalWrite(this->pinPhaseDown, LOW); 
}

void PhaseControl::setCurrentValue(uint8_t value) {
  this->currentValue = max(min((int)value, 100), 0);
  if (this->phaseChangedCallback != NULL) {
    this->phaseChangedCallback(this->currentValue);
  }
}

void PhaseControl::stepDown() {
  digitalWrite(this->pinPhaseDown, HIGH); 
  delay(PhaseControl::BUTTON_HOLD_DURATION);                      
  digitalWrite(this->pinPhaseDown, LOW);  
  delay(PhaseControl::WAIT_FOR_NEXT_PRESS);                      

  this->setCurrentValue(this->currentValue - 1);
}

void PhaseControl::stepUp() {
  digitalWrite(this->pinPhaseUp, HIGH); 
  delay(PhaseControl::BUTTON_HOLD_DURATION);                      
  digitalWrite(this->pinPhaseUp, LOW);  
  delay(PhaseControl::WAIT_FOR_NEXT_PRESS);     

  this->setCurrentValue(this->currentValue + 1);
}

void PhaseControl::setTargetValue(uint8_t value) {
  this->targetValue = max(min((int)value, 100), 0);
}

uint8_t PhaseControl::getTargetValue() {
  return this->targetValue;
}
    

void PhaseControl::update() {
  uint8_t diff = this->targetValue - this->currentValue;
    
  if (diff > 0) {
    this->stepUp();
  }
  if (diff < 0) {
    this->stepDown();
  }
}

void PhaseControl::setPhaseChangedCallback(std::function<void(uint8_t)> phaseChangedCallback) {
  this->phaseChangedCallback = phaseChangedCallback;
}
