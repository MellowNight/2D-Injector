#pragma once
#include "hwid_structs.h"
#include "util.h"

class Spoofer
{
private:
    static Spoofer* spoofer;

    void Init()
    {
        char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

        for (auto i = 0; i < sizeof(disk_serial); ++i)
        {
            disk_serial[i] = alphabet[Utils::Random() % (sizeof(alphabet) - 1)];
        }
    }
public:
    /* Static access method. */
    static Spoofer* Get()
    {
        if (spoofer == NULL)
        {
            spoofer = (Spoofer*)ExAllocatePool(NonPagedPool, sizeof(Spoofer));

            spoofer->Init();
        }

        return spoofer; 
    }

    char disk_serial[13];

public:
    void SpoofDisk(uint32_t IoControlCode, void* InputBuffer, void* OutputBuffer, size_t OutputBufferLength);
};