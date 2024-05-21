char receiverMacAddress1[] = "00:00:00:00:00:00";
char receiverMacAddress2[] = "00:00:00:00:00:00";

typedef struct msg {
  uint8_t action;
  int value;
} msg;

esp_err_t send(msg m) { return esp_now_send(0, (uint8_t *)&m, sizeof(msg)); }
