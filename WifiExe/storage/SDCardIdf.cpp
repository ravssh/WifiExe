#include "SDCardIdf.h"

SDCardIdf::SDCardIdf(Stream &debug, const char *mount_point) : SDCard(debug, mount_point)
{
}

SDCardIdf::~SDCardIdf()
{
}
