#ifndef WifiModule_h
#define WifiModule_h

#include <WiFi.h>
#include <DNSServer.h>

class WifiModule {
  private:
    char *hostname;
    DNSServer dnsServer;
    IPAddress apIP;
    static const byte DNS_PORT;

    void connectWithTimeout(char* wifi_ssid, char* wifi_password);
    void createHotspot();

  public:

    WifiModule();
    void setHostname(char* hostname);
    void connect(char* wifi_ssid, char* wifi_password);

    void update();
};

#endif