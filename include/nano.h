#ifndef NANO_H
#define NANO_H

#include <Arduino.h>
#include <WiFiClient.h>

typedef struct {
 WiFiClient *client;
 const char *ip;
 const char *apiKey;
} NanoClient;

NanoClient* NanoClient__Create(WiFiClient &client, const char *ip, const char *apiKey);
void NanoClient__setEffect(NanoClient* const self, const char *effect);
void NanoClient__turnOff(NanoClient* const self);

#endif /* NANO_H */