#ifndef WifiModule_h
#define WifiModule_h

#include <WiFi.h>
#include <DNSServer.h>

class WifiModule {
  private:
    const char *hostname;
    DNSServer dnsServer;
    IPAddress apIP;
    static const byte DNS_PORT;

    void connectWithTimeout(const char* wifi_ssid, const char* wifi_password);
    void createHotspot();

  public:

    WifiModule();
    void setHostname(const char* hostname);
    void connect(const char* wifi_ssid, const char* wifi_password);

    void update();
};

#endif