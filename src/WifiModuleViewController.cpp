#include "WifiModuleViewController.h"


WifiModuleViewController::WifiModuleViewController(Config *config): config(config) {
}

void WifiModuleViewController::createUI(uint16_t tabWifiSettings) {
    ESPUI.addControl(ControlType::Text, "SSID", this->config->getWifiSSID(), ControlColor::Alizarin, tabWifiSettings, [](Control * sender, int eventtype, void *UserInfo){
        WifiModuleViewController* self = (WifiModuleViewController *)UserInfo;
        self->currentWifiSSID = sender->value;

        Serial.println("Text Incoming");
        Serial.println(eventtype);
        Serial.println(sender->value);
    }, this);

    ESPUI.addControl(ControlType::Password, "Password", this->config->getWifiPassword(), ControlColor::Alizarin, tabWifiSettings, [](Control * sender, int eventtype, void *UserInfo){
        WifiModuleViewController* self = (WifiModuleViewController *)UserInfo;
        self->currentWifiPassword = sender->value;
    }, this);


    ESPUI.addControl(ControlType::Button, "Save & Restart", "Save & Restart", ControlColor::Alizarin, tabWifiSettings, [](Control * control, int eventtype, void *UserInfo) {
        if (eventtype == B_DOWN) {
            Serial.println("Save & Restart");
            delay(250);
            WifiModuleViewController* self = (WifiModuleViewController *)UserInfo;
            Serial.println(self->currentWifiSSID);

            self->config->setWifiSSID(self->currentWifiSSID);
            self->config->setWifiPassword(self->currentWifiPassword);

            Serial.println(self->config->getWifiSSID());
        }
    }, this);
}