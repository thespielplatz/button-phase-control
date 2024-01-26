#ifndef PhaseControlViewController_h
#define PhaseControlViewController_h

#include <stdint.h>

class PhaseControl;
class Control;
class PhaseControlViewController;

class PhaseControlViewController {
  protected:
    static PhaseControlViewController* singleton;

    uint16_t sliderId, btnDownId, btnUpId, btnRecalibrateId, labelPhaseValueId, labelStatusId;
    PhaseControl* phaseControl;

  public:

    PhaseControlViewController(PhaseControl* phaseControl);

    void createUI(uint16_t tabId);
    void setTargetValueAndSlider(int8_t newTargetValue);
    void disableControls();
    void enableControls();
    void update();
};

#endif