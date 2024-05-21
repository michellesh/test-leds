// Source https://RandomNerdTutorials.com/esp-now-one-to-many-esp32-esp8266/

#include <WiFi.h>
#include <esp_now.h>

#include "shared.h"

#define NUM_RECEIVERS 2
uint8_t receiverAddresses[NUM_RECEIVERS][6]; // 6 bytes in a mac address

msg data;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0],
           mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success"
                                                : "Delivery Fail");
}

void macAddressStrToBytes(const char *macStr, uint8_t *macBytes) {
  for (int i = 0; i < 6; ++i) {
    macBytes[i] = strtoul(macStr + 3 * i, nullptr, 16);
  }
}

void registerPeer(uint8_t *receiverAddress) {
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.print("Failed to add peer");
    return;
  }
}

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  // register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  macAddressStrToBytes(receiverMacAddress1, receiverAddresses[0]);
  macAddressStrToBytes(receiverMacAddress2, receiverAddresses[1]);

  registerPeer(receiverAddresses[0]);
  registerPeer(receiverAddresses[1]);
}

void loop() {
  data.action = random(0, 20);
  data.value = random(0, 20);

  esp_err_t result = send(data);

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
