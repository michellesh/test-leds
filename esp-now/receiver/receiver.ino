// Source https://RandomNerdTutorials.com/esp-now-one-to-many-esp32-esp8266/

#include <WiFi.h>
#include <esp_now.h>

#include "shared.h"

msg data;

// callback function that will be executed when data is received
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("action: ");
  Serial.println(data.action);
  Serial.print("value: ");
  Serial.println(data.value);
  Serial.println();
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {}
