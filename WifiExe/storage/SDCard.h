#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>

class SDCard
{
public:
    SDCard(Stream &debug, const char *mount_point = "/sd");
    ~SDCard();

private:
    Stream &m_debug;
    const char *m_mount_point;
};

#endif
