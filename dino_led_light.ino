#include <FastLED.h>
#include "lighting_mode.hpp"

// LED Strip information
#define DATA_PIN 3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 26
#define BRIGHTNESS 200
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 10
Animation *animation;

// Mode change button
const int button_itr_pin = 2;
volatile bool button_was_pressed = false;

void setup() {
    // Initialize mode change button
    // trigger when button pressed, but not when released.
    pinMode(button_itr_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button_itr_pin), button_itr_fn, FALLING);

    // initial delay is recommended
    delay(200);

    // initialize LED strip
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(
        leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    // FIXME: Is it required ? We pass brightness to ColorFromPalette…
    FastLED.setBrightness(0);

    // Initialize animation utility
    animation = new Animation(leds, NUM_LEDS);
    animation->setBrightness(BRIGHTNESS);
    // Use the second palette
    animation->changePalette();

}

void button_itr_fn() {
    button_was_pressed = true;
}

void loop() {
    // Handle button press
    if (button_was_pressed) {
        button_was_pressed = false;
        animation->changeAnimation();
    }

    // This will fill the leds array with some shiny colors
    animation->animate();

    // delay() actually shows the LEDs
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}
