#ifndef HUE_H
#define HUE_H

#include <Arduino.h>
#include <WiFiClient.h>

typedef struct {
 WiFiClient *client;
 const char *ip;
 const char *apiKey;
 const char *lastEventTime;
} HueClient;

HueClient* HueClient__Create(WiFiClient &client, const char *ip, const char *apiKey);
long HueClient__pollButton(HueClient* const self, int id);

#endif /* HUE_H */