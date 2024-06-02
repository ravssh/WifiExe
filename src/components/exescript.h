// exescript.h
#ifndef EXESCRIPT_H
#define EXESCRIPT_H

#include "USB.h"
#include "USBHIDKeyboard.h"

#include "rgb_control.h"

struct TaskParameters
{
    const char *str;
    int del; // Remove the const qualifier
    bool enter;

    // Constructor to initialize members
    TaskParameters(const char *s, int d, bool e) : str(s), del(d), enter(e) {}
};

extern USBHIDKeyboard Keyboard;
extern bool check_delay;

// Function declarations
void setup_usb();
void open_elevated_cmd();
void clear_trace();
void payload();
void write(const char *str, const int del, bool e);
void write_task(const char *str, const int del, bool);

#endif
