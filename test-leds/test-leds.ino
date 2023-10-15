#include <FastLED.h>

#define DATA_PIN 4
#define LED_TYPE NEOPIXEL
#define COLOR_ORDER GRB
#define NUM_LEDS 50
#define BRIGHTNESS 255

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  delay(500);

  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
  //  .setCorrection(TypicalLEDStrip)
  //  .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  FastLED.clear();

  for (int i = 0; i < NUM_LEDS; i++) {
    int hue = map(i, 0, NUM_LEDS, 0, 255);
    leds[i] = CHSV(hue, 255, BRIGHTNESS);
  }

  FastLED.show();
}
