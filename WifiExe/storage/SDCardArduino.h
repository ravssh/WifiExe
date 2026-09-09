#ifndef SDCARDARDUINO_H
#define SDCARDARDUINO_H

#include "SDCard.h"

class SDCardArduino : public SDCard
{
public:
    SDCardArduino(Stream &debug, const char *mount_point = "/sd");
    ~SDCardArduino();
};

#endif
