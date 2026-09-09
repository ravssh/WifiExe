#include "SDCardLazyWrite.h"

SDCardLazyWrite::SDCardLazyWrite(Stream &debug, const char *mount_point) : SDCard(debug, mount_point)
{
}

SDCardLazyWrite::~SDCardLazyWrite()
{
}
