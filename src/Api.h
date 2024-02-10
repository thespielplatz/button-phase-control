#ifndef API_H
#define API_H

#include "ESPAsyncWebServer.h"
#include "PhaseControl.h"
#include <ArduinoJson.h>

class Api {
private:
    AsyncWebServer* server;
    PhaseControl *phaseControl;

    DeserializationError lastPostTargetValueError;

public:

    Api();
    void setup(AsyncWebServer* server, PhaseControl* phaseControl);

};

#endif  // API_H