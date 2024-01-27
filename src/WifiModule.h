#ifndef WifiModule_h
#define WifiModule_h

#include <WiFi.h>
#include <DNSServer.h>

class WifiModule {
  private:
    String hostname;
    DNSServer dnsServer;
    IPAddress apIP;
    static const byte DNS_PORT;

    void connectWithTimeout(String wifi_ssid, String wifi_password);
    void createHotspot();

  public:

    WifiModule();
    void setHostname(String hostname);
    void connect(String wifi_ssid, String wifi_password);

    void update();
};

#endif