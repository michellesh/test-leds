// Complete Instructions to Get and Change ESP MAC Address:
// https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/
#include <FastLED.h>

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

void setup() { Serial.begin(9600); }

void loop() {
  EVERY_N_SECONDS(1) {
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  }
}
