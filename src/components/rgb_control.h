// rgb_control.h
#ifndef RGB_CONTROL_H
#define RGB_CONTROL_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// Function declarations
void setup_rgb();
void blink(bool check); // default value = true
#endif
