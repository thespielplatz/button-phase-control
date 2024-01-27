#ifndef CONFIG_H
#define CONFIG_H

#include <Preferences.h>

class Config {
private:
    Preferences *preferences;

public:
    void setup(Preferences *preferences, const char* namespaceName);
    void setWifiSSID(String ssid);
    void setWifiPassword(String password);
    String getWifiSSID();
    String getWifiPassword();
};

#endif // CONFIG_H