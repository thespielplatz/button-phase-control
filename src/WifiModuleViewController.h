#ifndef WIFI_MODULE_VIEW_CONTROLLER_H
#define WIFI_MODULE_VIEW_CONTROLLER_H

#include "Config.h"
#include "ESPUI.h"

class WifiModuleViewController {
private:
    Config *config;
    String currentWifiSSID;
    String currentWifiPassword;

public:
    WifiModuleViewController(Config *config);
    void createUI(uint16_t tabWifiSettings); // Move tabWifiSettings to the createUI method
};

#endif // WIFI_MODULE_VIEW_CONTROLLER_H