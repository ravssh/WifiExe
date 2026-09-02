// neopixel_wrapper.h
#ifndef NEOPIXEL_WRAPPER_H
#define NEOPIXEL_WRAPPER_H

#include <Adafruit_NeoPixel.h>

// Simple helper that provides neopixelWrite(pin, g, r, b);
void neopixelWrite(int pin, uint8_t g, uint8_t r, uint8_t b);

#endif
