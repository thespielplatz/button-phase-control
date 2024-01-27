#include "WifiModuleViewController.h"

#define SAVE_AND_RESTART_DELAY 250

WifiModuleViewController::WifiModuleViewController(Config *config): 
    config(config),
    saveAndRestartTimestamp(0) {
}

void WifiModuleViewController::createUI(uint16_t tabWifiSettings) {
    this->textWifiSSIDId = ESPUI.addControl(ControlType::Text, "SSID", this->config->getWifiSSID(), ControlColor::Turquoise, tabWifiSettings, [](Control *sender, int eventype) {
        // This needs to here, otherwise the value of the control is not updated!
    });
    this->textWifiPassword = ESPUI.addControl(ControlType::Password, "Password", this->config->getWifiPassword(), ControlColor::Turquoise, tabWifiSettings, [](Control *sender, int eventype) {
        // This needs to here, otherwise the value of the control is not updated!
    });

    ESPUI.addControl(ControlType::Button, "Save & Restart", "Save & Restart", ControlColor::Alizarin, tabWifiSettings, [](Control * control, int eventtype, void *UserInfo) {
        WifiModuleViewController *self = (WifiModuleViewController *)UserInfo;
        if (eventtype == B_DOWN) {
            self->saveAndRestartTimestamp = millis() + SAVE_AND_RESTART_DELAY;
        }    
    }, this);
}

void WifiModuleViewController::update() {
    if (this->saveAndRestartTimestamp != 0 && millis() >= this->saveAndRestartTimestamp) {
        Serial.println("Save & Restart");
        Serial.println(ESPUI.getControl(this->textWifiSSIDId)->value);

        this->config->setWifiSSID(ESPUI.getControl(this->textWifiSSIDId)->value);
        this->config->setWifiPassword(ESPUI.getControl(this->textWifiPassword)->value);

        this->saveAndRestartTimestamp = 0;

        this->config->end();
        ESP.restart();
    }
}