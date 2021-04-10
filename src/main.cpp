#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "settings.h"
#include "hue.h"
#include "nano.h"

WiFiClient client;
HueClient *hue;
NanoClient *nanoleafOne;

#if NANOLEAF_TWO_ENABLED
  NanoClient *nanoleafTwo;
#endif

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    bool res;
    res = wm.autoConnect("hue2nano");

    if(!res) {
        Serial.println("Failed to connect");
    } else { 
        Serial.println("connected...yeey :)");
        hue = HueClient__Create(client, HUEHUB_IP, HUEHUB_APIKEY);
        nanoleafOne = NanoClient__Create(client, NANOLEAF_ONE_IP, NANOLEAF_ONE_APIKEY);

        #if NANOLEAF_TWO_ENABLED
          nanoleafTwo = NanoClient__Create(client, NANOLEAF_TWO_IP, NANOLEAF_TWO_APIKEY);
        #endif
    }
}

void loop() {
    long event = HueClient__pollButton(hue, HUEHUB_REMOTEID);
    if(event == 1002) {
      NanoClient__setEffect(nanoleafOne, NANOLEAF_ONE_DEFAULT_EFFECT);
      #if NANOLEAF_TWO_ENABLED
        NanoClient__setEffect(nanoleafTwo, NANOLEAF_TWO_DEFAULT_EFFECT);
      #endif
    } else if (event == 4002) { // OFF
      NanoClient__turnOff(nanoleafOne);
      #if NANOLEAF_TWO_ENABLED
        NanoClient__turnOff(nanoleafTwo);
      #endif
    } else {
      Serial.println(event);
    }
    Serial.println("loop");
    delay(POLL_SPEED);
}
