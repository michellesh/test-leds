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

  byte boardNumber = getBoardNumber();

  if (boardNumber == 1) {
    // set up pin configuration for board 1
  } else if (boardNumber == 2) {
    // set up pin configuration for board 2
  }
}

void loop() {}

byte getBoardNumber() {
  byte boardNumber = WiFi.macAddress() == receiverMacAddress1   ? 1
                     : WiFi.macAddress() == receiverMacAddress2 ? 2
                                                                : 0;
  if (boardNumber == 0) {
    Serial.print("MAC address not found: ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.print("Board number: ");
    Serial.println(boardNumber);
  }

  return boardNumber;
}
