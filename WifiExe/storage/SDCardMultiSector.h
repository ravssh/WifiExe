#ifndef SDCARDMULTISECTOR_H
#define SDCARDMULTISECTOR_H

#include "SDCard.h"

class SDCardMultiSector : public SDCard
{
public:
    SDCardMultiSector(Stream &debug, const char *mount_point = "/sd");
    ~SDCardMultiSector();
};

#endif
