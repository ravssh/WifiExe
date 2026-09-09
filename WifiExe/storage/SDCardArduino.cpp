#include "SDCardArduino.h"

SDCardArduino::SDCardArduino(Stream &debug, const char *mount_point) : SDCard(debug, mount_point)
{
}

SDCardArduino::~SDCardArduino()
{
}
