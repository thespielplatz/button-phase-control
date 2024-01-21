#include "secrets.h"
#include <ESPUI.h>
#include <WiFi.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

uint8_t PIN_PHASE_DOWN = 33; // Green
uint8_t PIN_PHASE_UP = 14; // Yellow
uint32_t BUTTON_DOWN_DURATION = 150;
uint8_t LED_BUILTIN = 2;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* hostname = "espui";


void btnDownCallback(Control* sender, int type) {
  if (type == B_DOWN) {
    Serial.println("btnDownCallback DOWN");
    digitalWrite(PIN_PHASE_DOWN, HIGH); 
    delay(BUTTON_DOWN_DURATION);                      
    digitalWrite(PIN_PHASE_DOWN, LOW);  
  }
}

void btnUpCallback(Control* sender, int type) {
  if (type == B_DOWN) {
    Serial.println("btnUpCallback DOWN");
    digitalWrite(PIN_PHASE_UP, HIGH); 
    delay(BUTTON_DOWN_DURATION);                      
    digitalWrite(PIN_PHASE_UP, LOW);  
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  pinMode(PIN_PHASE_UP, OUTPUT);
  pinMode(PIN_PHASE_DOWN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(PIN_PHASE_UP, LOW); 
  digitalWrite(PIN_PHASE_DOWN, LOW); 

  //Turn off verbose debugging
	ESPUI.setVerbosity(Verbosity::Quiet);
  WiFi.setHostname(hostname);

  WiFi.begin(ssid, password);
  Serial.print("\Try to connect to existing network");

  // Wait for connection, 5s timeout
  uint8_t timeout = 10;
  do {
      delay(500);
      Serial.print(".");
      timeout--;
  } while (timeout && WiFi.status() != WL_CONNECTED);

  // not connected -> create hotspot
  if (WiFi.status() != WL_CONNECTED) {
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

    timeout = 5;

    do {
      delay(500);
      Serial.print(".");
      timeout--;
    } while (timeout);
  }

  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

  ESPUI.button("Up", &btnUpCallback, ControlColor::Peterriver, "Press");
  ESPUI.button("Down", &btnDownCallback, ControlColor::Wetasphalt, "Press");

  ESPUI.begin("ESPUI Control");
}

void loop() {
  dnsServer.processNextRequest();
  
}
