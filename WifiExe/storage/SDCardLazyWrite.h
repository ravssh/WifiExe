#ifndef SDCARDLAZYWRITE_H
#define SDCARDLAZYWRITE_H

#include "SDCard.h"

class SDCardLazyWrite : public SDCard
{
public:
    SDCardLazyWrite(Stream &debug, const char *mount_point = "/sd");
    ~SDCardLazyWrite();
};

#endif
