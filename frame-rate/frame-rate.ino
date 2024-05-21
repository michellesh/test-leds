#include <ESP32Time.h>
#include <FastLED.h>

#define LED_PIN 13
#define NUM_LEDS 50

CRGB leds[NUM_LEDS];

ESP32Time rtc;

unsigned long ticks = 0;

void setup() {
  Serial.begin(9600);
  delay(500);

  rtc.setTime(30, 24, 15, 17, 1, 2024); // 17th Jan 2024 15:24:30

  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
}

void loop() {
  FastLED.clear();

  static int prevSecond = -1;
  static unsigned long prevTicks = ticks;
  int currentSecond = rtc.getSecond();
  if (prevSecond != currentSecond) {
    Serial.print("Frames in last second: ");
    Serial.println(ticks - prevTicks);
    prevSecond = currentSecond;
    prevTicks = ticks;
  }

  // FastLED.setMaxRefreshRate(80);

  FastLED.show();
  ticks++;
}
