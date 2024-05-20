uint8_t receiverAddress1[] = {0x4C, 0x75, 0x25, 0x64, 0xE5, 0xDC};
uint8_t receiverAddress2[] = {0xA8, 0x42, 0xE3, 0xAB, 0xA6, 0x88};
uint8_t senderAddress[] = {0xC8, 0xF0, 0x9E, 0x9F, 0x36, 0xA8};
char receiverMacAddress1[] = "4C:75:25:64:E5:DC";
char receiverMacAddress2[] = "A8:42:E3:AB:A6:88";
char senderMacAddress[] = "C8:F0:9E:9F:36:A8";

typedef struct msg {
  uint8_t action;
  int value;
} msg;

esp_err_t send(msg m) { return esp_now_send(0, (uint8_t *)&m, sizeof(msg)); }
