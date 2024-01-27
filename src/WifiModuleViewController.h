#ifndef WIFI_MODULE_VIEW_CONTROLLER_H
#define WIFI_MODULE_VIEW_CONTROLLER_H

#include "Config.h"
#include "ESPUI.h"

class WifiModuleViewController {
private:
    Config *config;
    uint16_t textWifiSSIDId, textWifiPassword;
    unsigned long saveAndRestartTimestamp;

public:
    WifiModuleViewController(Config *config);
    void createUI(uint16_t tabWifiSettings); // Move tabWifiSettings to the createUI method
    void update();
};

#endif // WIFI_MODULE_VIEW_CONTROLLER_H