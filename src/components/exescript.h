// exescript.h
#ifndef EXESCRIPT_H
#define EXESCRIPT_H

#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

struct TaskParameters
{
    const char *str;
    int del; // Remove the const qualifier
    int e;

    // Constructor to initialize members
    TaskParameters(const char *s, int d, int e) : str(s), del(d), e(e) {}
};

extern USBHIDKeyboard Keyboard;
extern bool check_delay;

// Function declarations
void setup_usb();
void create_write_task();
void open_elevated_cmd();
void clear_trace();
void payload();
void write(const char *str, const int del, int e);
void write_task(const char *str, const int del, int e = 0);

#endif
