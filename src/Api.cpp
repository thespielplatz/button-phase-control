#include "Api.h"

Api::Api() {
}

void Api::setup(AsyncWebServer* server, PhaseControl* phaseControl) {
  this->server = server;
  this->phaseControl = phaseControl;

    server->on("/api/current-value", HTTP_GET, [this](AsyncWebServerRequest *request){
        AsyncResponseStream *response = request->beginResponseStream("text/plain");
        response->print(this->phaseControl->getCurrentValue());
        request->send(response);
    });  

    server->on("/api/target-value", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("set")) {
            AsyncWebParameter* p = request->getParam("set");
            Serial.print("set: ");
            Serial.println(p->value().c_str());
            int value = p->value().toInt();
            this->phaseControl->setTargetValue(value);
        }
        AsyncResponseStream *response = request->beginResponseStream("text/plain");
        response->print(this->phaseControl->getTargetValue());
        request->send(response);
    });  

    server->on("/api/target-value", HTTP_POST, [&](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("text/plain");

        if (this->lastPostTargetValueError) {
            response->setCode(400);
            response->print(-1);
        } else {
            response->setCode(400);
            response->print(this->phaseControl->getTargetValue());
        }
        request->send(response);
    }, NULL, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        JsonDocument doc;

        this->lastPostTargetValueError = deserializeJson(doc, (const char*)data);

        if (this->lastPostTargetValueError) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(this->lastPostTargetValueError.c_str());
            return;
        }

        // Access values
        int value = doc["value"];
        this->phaseControl->setTargetValue(value);
    });
}