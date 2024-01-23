#ifndef PhaseControl_h
#define PhaseControl_h

#include <stdint.h>
#include <functional>

class PhaseControl {
  private:
    uint8_t pinPhaseDown;
    uint8_t pinPhaseUp;
    uint32_t buttonHoldDuration;
    uint32_t waitForNextPress;

    uint32_t currentValue;
    uint32_t targetValue;

    std::function<void(uint8_t)> phaseChangedCallback;

    void setCurrentValue(uint8_t value);

  public:
    static const uint8_t BUTTON_HOLD_DURATION;
    static const uint8_t WAIT_FOR_NEXT_PRESS;

    PhaseControl(uint8_t _pinPhaseDown, uint8_t _pinPhaseUp);

    void setup();

    void stepDown();
    void stepUp();

    void setTargetValue(uint8_t value);
    uint8_t getTargetValue();
    void update();

    void setPhaseChangedCallback(std::function<void(uint8_t)> phaseChangedCallback);
};

#endif