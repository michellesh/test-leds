#include <Arduino.h>
#include <WiFi.h>

int activePattern = 0;
int speed = 3;
int density = 4;
int width = 5;
int bpm = 60;
int reverse = 0;

#include "web_server.h"

void setup() {
  Serial.begin(9600);
  delay(500);

  setupWebServer();
}

void printValue(int value, int &prevValue, char *label) {
  if (value != prevValue) {
    Serial.print(label);
    Serial.print(": ");
    Serial.println(value);
    prevValue = value;
  }
}

void loop() {
  static int prevActivePattern = activePattern;
  static int prevSpeed = speed;
  static int prevDensity = density;
  static int prevWidth = width;
  static int prevBpm = bpm;
  static int prevReverse = reverse;

  printValue(activePattern, prevActivePattern, "activePattern");
  printValue(speed, prevSpeed, "speed");
  printValue(density, prevDensity, "density");
  printValue(width, prevWidth, "width");
  printValue(bpm, prevBpm, "bpm");
  printValue(reverse, prevReverse, "reverse");
}
