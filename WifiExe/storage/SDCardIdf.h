#ifndef SDCARDIDF_H
#define SDCARDIDF_H

#include "SDCard.h"

class SDCardIdf : public SDCard
{
public:
    SDCardIdf(Stream &debug, const char *mount_point = "/sd");
    ~SDCardIdf();
};

#endif
