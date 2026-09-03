#include "neopixel_wrapper.h"

// This implementation creates a temporary Adafruit_NeoPixel strip for the given pin and writes a single pixel.
// It's not the most efficient (re-initializes on every call) but it's portable for Arduino IDE migration.

void neopixelWrite(int pin, uint8_t g, uint8_t r, uint8_t b)
{
    Adafruit_NeoPixel strip(1, pin, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.show();
}
