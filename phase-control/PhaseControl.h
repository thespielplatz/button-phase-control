#ifndef PhaseControl_h
#define PhaseControl_h

#include <stdint.h>
#include <functional>

enum PhaseControlState {
    Starting,
    Running,
    Recalibrating
};

class PhaseControl {
  private:
    PhaseControlState state; 

    uint8_t pinPhaseDown;
    uint8_t pinPhaseUp;
    uint32_t buttonHoldDuration;
    uint32_t waitForNextPress;

    uint32_t currentValue;
    uint32_t targetValue;

    unsigned long timeWhenUpdated;
    int32_t recalibratingTimeLeft;

    std::function<void(uint8_t)> phaseChangedCallback;
    std::function<void(PhaseControlState)> stateChangedCallback;

    void setCurrentValue(uint8_t value);
    void setState(PhaseControlState newState);

  public:
    static char* PhaseControlStateToString(PhaseControlState state);

    PhaseControl(uint8_t _pinPhaseDown, uint8_t _pinPhaseUp);

    void setup();

    void stepDown();
    void stepUp();

    void setTargetValue(int8_t value);
    uint8_t getTargetValue();
    void update();

    void setPhaseChangedCallback(std::function<void(uint8_t)> phaseChangedCallback);
    void setStateChangedCallback(std::function<void(PhaseControlState)> stateChangedCallback);

    void recalibrate();
    int32_t getRecalibratingTimeLeft();
    PhaseControlState getState();
};

#endif