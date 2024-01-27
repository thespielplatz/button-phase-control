#include "Config.h"

#define CONFIG_WIFI_SSID "ssid"
#define CONFIG_WIFI_PASSWORD "password"

void Config::setup(Preferences *preferences, const char* namespaceName) {
    this->preferences = preferences;
    this->preferences->begin(namespaceName, false);
}

void Config::setWifiSSID(String ssid) {
    this->preferences->putString(CONFIG_WIFI_SSID, ssid);
}

void Config::setWifiPassword(String password) {
    this->preferences->putString(CONFIG_WIFI_PASSWORD, password);
}

String Config::getWifiSSID() {
    return this->preferences->getString(CONFIG_WIFI_SSID, "");
}

String Config::getWifiPassword() {
    return this->preferences->getString(CONFIG_WIFI_PASSWORD, "");
}

void Config::end() {
    this->preferences->end();
}
