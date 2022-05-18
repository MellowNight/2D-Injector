#pragma once
#include "hwid_structs.h"
#include "util.h"

class Spoofer
{
private:
    static Spoofer* spoofer;

    /*  i dont feel like overloading the new operator, so init() will be constructor */

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
    static Spoofer* Get();

    char disk_serial[13];

public:
    void SpoofDisk(uint32_t IoControlCode, void* InputBuffer, void* OutputBuffer, size_t OutputBufferLength);
};