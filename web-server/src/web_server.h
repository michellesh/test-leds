/*
 * The webserver will attempt to connect to the primary ssid 5 times, with a delay
 * of 1 second between each attempt. If it fails to connect, it will retry
 * using the secondary ssid. The reasoning behind this: If I am at home, I want
 * it to connect to my home network. If I'm elsewhere, I want it to connect
 * to the hotspot on my phone. If it fails to connect to either, it will default
 * to its previously saved settings.
 *
 * Libraries:
 * ESPAsyncWebServer    https://github.com/me-no-dev/ESPAsyncWebServer
 * AsyncTCP             https://github.com/me-no-dev/AsyncTCP
 *
 * Include in platformio.ini:
 * lib_deps =
 *     ...
 *     ESP Async WebServer
 *     ESP Async TCP
 *     FS
 */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "secrets.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Web server html
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Infinitube</title>
  <style>
    html { font-family: Arial; display: inline-block; }
    body { max-width: 500px; margin:0px auto; padding: 0px 7px; }
    button { width: 32%%; height: 50px; font-size: 0.9rem; }
    table { width: 100%%; }
    h2 { font-size: 2.3rem; text-align: center }
    p { font-size: 1.9rem; }
    .buttons { display: flex; flex-wrap: wrap; align-items: center; gap: 0.3em; }
    .break { flex-basis: 100%%; height: 10px; }
    .labelCol { width: 80px; }
    .valCol { width: 40px; padding-left: 14px; }
    .slider { -webkit-appearance: none; margin: 14px 0; width: 100%%;
      height: 15px; background: #FFD65C; outline: none; -webkit-transition: .2s;
      transition: opacity .2s; }
    .slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none;
      width: 25px; height: 25px; background: #003249; cursor: pointer; }
    .slider::-moz-range-thumb { width: 25px; height: 25px; background: #003249;
      cursor: pointer; }
  </style>
</head>
<body>
  <h2>Title</h2>

  <h3>Sliders</h3>
  <table border="0">
  <tr>
    <td class="labelCol"><label for="speedSlider">Speed</label></td>
    <td><input type="range" id="speedSlider" onchange="sendData('s',this.value)" min="1" max="10" value="%SPEEDVALUE%" step="1" class="slider"></td>
    <td class="valCol"><span id="speedValue">%SPEEDVALUE%</span></td>
  </tr>
  <tr>
    <td class="labelCol"><label for="densitySlider">Density</label></td>
    <td><input type="range" id="densitySlider" onchange="sendData('d',this.value)" min="1" max="10" value="%DENSITYVALUE%" step="1" class="slider"></td>
    <td class="valCol"><span id="densityValue">%DENSITYVALUE%</span></td>
  </tr>
  <tr>
    <td class="labelCol"><label for="widthSlider">Width</label></td>
    <td><input type="range" id="widthSlider" onchange="sendData('w',this.value)" min="1" max="10" value="%WIDTHVALUE%" step="1" class="slider"></td>
    <td class="valCol"><span id="widthValue">%WIDTHVALUE%</span></td>
  </tr>
  </table>

  <h3>Patterns</h3>
  <div class="buttons">
    <button type="button" onclick="sendData('p',0)">Pattern 0</button>
    <button type="button" onclick="sendData('p',1)">Pattern 1</button>
    <button type="button" onclick="sendData('p',2)">Pattern 2</button>
    <div class="break"></div>
    <button type="button" onclick="sendData('p',3)">Pattern 3</button>
    <button type="button" onclick="sendData('p',4)">Pattern 4</button>
    <button type="button" onclick="sendData('p',5)">Pattern 5</button>
    <button type="button" onclick="sendData('p',6)">Pattern 6</button>
    <button type="button" onclick="sendData('p',7)">Pattern 7</button>
    <div class="break"></div>
  </div>

  <h3>Checkbox</h3>
  <div>
    <input type="checkbox" id="reverse" name="reverse" onclick="sendData('r',this.value)" />
    <label for="reverse">Reverse</label>
  </div>

  <h3>Text input</h3>
  <label for="bpm">BPM </label>
  <input id="bpm" type="number" min="1" max="65535" onchange="sendData('b',this.value)" value="%BPMVALUE%">

<script>
 var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }

  function onOpen(event) {
    console.log('Connection opened');
    document.getElementById('autoBtn').style.backgroundColor = '#baffb3';
  }

  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }

  function onMessage(event) {
    var dataType = event.data.charAt(0);
    var dataValue = event.data.substring(1);
    switch (dataType){
    case 's':
      document.getElementById('speedValue').innerHTML = dataValue;
      document.getElementById('speedSlider').value = dataValue;
      break;
    case 'd':
      document.getElementById('densityValue').innerHTML = dataValue;
      document.getElementById('densitySlider').value = dataValue;
      break;
    case 'w':
      document.getElementById('widthValue').innerHTML = dataValue;
      document.getElementById('widthSlider').value = dataValue;
      break;
    }
  }

  function onLoad(event) {
    initWebSocket();

    // select all text when focus on text inputs
    const rgbInputIds = ['bpm'];
    rgbInputIds.forEach(rgbInputId => {
      document.getElementById(rgbInputId).addEventListener("focus", e => {
        e.target.select();
      });
    });
  }

  function sendData(type, val) {
    console.log(type+val);
    websocket.send(type+val);
  }
</script>
</body>
</html>
)rawliteral";

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.println((char*)data);
    String message = String((char*)data);
    char dataType = message.charAt(0);
    String dataValue = message.substring(1);

    switch (dataType) {
      case 's':
        speed = dataValue.toInt();
        ws.textAll(message);
        break;
      case 'd':
        speed = dataValue.toInt();
        ws.textAll(message);
        break;
      case 'w':
        speed = dataValue.toInt();
        ws.textAll(message);
        break;
      case 'b':
        bpm = dataValue.toInt();
        ws.textAll(message);
        break;
      case 'p':
        activePattern = dataValue.toInt();
        ws.textAll(message);
        break;
      case 'r':
        reverse = reverse == 0 ? 1 : 0;
        ws.textAll(message);
        break;
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  if(var == "BPMVALUE"){
    return String(bpm);
  }
  if(var == "SPEEDVALUE"){
    return String(speed);
  }
  if(var == "DENSITYVALUE"){
    return String(density);
  }
  if(var == "WIDTHVALUE"){
    return String(width);
  }
  return "";
}

void setupWebServer(){

  uint8_t connectionAttempts = 0;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay (1000);
    Serial.println("Connecting to primary WiFi ...");
    connectionAttempts++;
    if (connectionAttempts > 5) break;
  }

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid2, password2);
    delay (1000);
    Serial.println("Connecting to secondary WiFi ...");
    connectionAttempts++;
    if (connectionAttempts > 10) break;
  }

  // Print ESP Local IP Address
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.begin();
}