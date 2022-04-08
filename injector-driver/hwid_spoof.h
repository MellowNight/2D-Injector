#include "includes.h"
#include "hwid_spoof.h"

namespace Spoofer
{
    extern char disk_serial[13];

    void SpoofDisk(uint32_t IoControlCode, void* InputBuffer, void* OutputBuffer);
};