#include "nano.h"

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Constructor (without allocation)
void NanoClient__init(NanoClient *self, WiFiClient &client, const char *ip, const char *apiKey) {
  self->client = &client;
  self->ip = ip;
  self->apiKey = apiKey;
}

// Allocation + initialization (equivalent to "new HueClient(...)")
NanoClient* NanoClient__Create(WiFiClient &client, const char *ip, const char *apiKey) {
   NanoClient* result = (NanoClient*) malloc(sizeof(NanoClient));
   NanoClient__init(result, client, ip, apiKey);
   return result;
}

void NanoClient__request(NanoClient * const self, const char *endpoint, const char *body) {
   HTTPClient http;
   
   char buffer [120];
   sprintf (buffer, "http://%s:16021/api/v1/%s%s", self->ip, self->apiKey, endpoint);
   String uri(buffer);

   //Serial.println(uri);

   if (http.begin(*self->client, uri)) {
      int httpCode = http.PUT(body);
      if (httpCode > 0) {
          // do sucessfull response stuff?
      } else {
        Serial.println(httpCode);
      }
   } else {
      Serial.println("Failed to connect.");
   }
}

void NanoClient__setEffect(NanoClient* const self, const char *effect) {
   char buffer [45];
   sprintf (buffer, "{\"select\" : \"%s\"}", effect);
   NanoClient__request(self, "/effects", buffer);
}

void NanoClient__turnOff(NanoClient* const self) {
   NanoClient__request(self, "/state", "{\"on\": {\"value\": false}}");
}