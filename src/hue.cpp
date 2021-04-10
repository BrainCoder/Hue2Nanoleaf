#include "hue.h"

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Constructor (without allocation)
void HueClient__init(HueClient *self, WiFiClient &client, const char *ip, const char *apiKey) {
  self->client = &client;
  self->ip = ip;
  self->apiKey = apiKey;
  self->lastEventTime = "2021-04-10T12:34:37";
}

// Allocation + initialization (equivalent to "new HueClient(...)")
HueClient* HueClient__Create(WiFiClient &client, const char *ip, const char *apiKey) {
   HueClient* result = (HueClient*) malloc(sizeof(HueClient));
   HueClient__init(result, client, ip, apiKey);
   return result;
}

void HueClient__request(HueClient * const self, JsonDocument &doc, const char *endpoint) {
   HTTPClient http;
   
   char buffer [120];
   sprintf (buffer, "http://%s/api/%s%s", self->ip, self->apiKey, endpoint);
   String uri(buffer);

   //Serial.println(uri);

   if (http.begin(*self->client, uri)) {
      int httpCode = http.GET();
      if (httpCode > 0) {
         deserializeJson(doc, http.getStream());
         http.end();
      } else {
          Serial.println(httpCode);
      }
   } else {
      Serial.println("Failed to connect.");
   }
}

long HueClient__pollButton(HueClient* const self, int id) {
   DynamicJsonDocument doc(2048);
   char buffer [15];
   sprintf (buffer, "/sensors/%d", id);
   HueClient__request(self, doc, buffer);

   long eventType = doc["state"]["buttonevent"].as<long>();
   const char *eventTime = doc["state"]["lastupdated"].as<const char *>();

   Serial.println(eventTime);
   Serial.println(self->lastEventTime);
   if(strcmp(eventTime, self->lastEventTime) != 0) {
      self->lastEventTime = strdup(eventTime);
      return eventType;
   } 
   return 0;
}