#include "WifiModule.h"

const byte WifiModule::DNS_PORT = 53;

WifiModule::WifiModule() : dnsServer(), apIP(192, 168, 4, 1)  {
}

void WifiModule::setHostname(const char* hostname) {
  this->hostname = hostname;
  WiFi.setHostname(this->hostname);
}

void WifiModule::connect(const char* wifi_ssid, const char* wifi_password) {
  Serial.print("Try to connect to existing network");

  // Wait for connection, 5s timeout
  this->connectWithTimeout(wifi_ssid, wifi_password);

  // not connected -> create hotspot
  if (WiFi.status() != WL_CONNECTED) {
    this->createHotspot();
  }

  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
}

void WifiModule::update() {
  this->dnsServer.processNextRequest();
}

void WifiModule::connectWithTimeout(const char* wifi_ssid, const char* wifi_password) {
  uint8_t timeout = 10;
  WiFi.begin(wifi_ssid, wifi_password);
  do {
      delay(500);
      Serial.print(".");
      timeout--;
  } while (timeout && WiFi.status() != WL_CONNECTED);  
}

void WifiModule::createHotspot() {
 Serial.print("\n\nCreating hotspot");

  WiFi.mode(WIFI_AP);
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  uint32_t chipid = 0;
  for (int i = 0; i < 17; i = i + 8)
  {
      chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  char ap_ssid[25];
  snprintf(ap_ssid, 26, "ESPUI-%08X", chipid);
  WiFi.softAP(ap_ssid);

  uint8_t timeout = 5;

  do {
    delay(500);
    Serial.print(".");
    timeout--;
  } while (timeout);  
}
