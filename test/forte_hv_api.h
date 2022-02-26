#pragma once
#include "pch.h"
#include "framework.h"
#include <cstdint>
#include <Windows.h>
#include <math.h>

enum VMMCALL_ID : uintptr_t
{
    set_mpk_hook = 0x22FFAA1166,
    set_tlb_hook = 0xAAFF226611,
    disable_hv = 0xFFAA221166,
};

extern "C" int __stdcall vmmcall(VMMCALL_ID vmmcall_id, ...);

namespace ForteVisor
{
    /*  Not on each core, because it's only relevant in 1 process context */
    int SetTlbHook(uintptr_t address, uint8_t* patch, size_t patch_len);

    int SetMpkHook(uintptr_t address, uint8_t* patch, size_t patch_len);

    int ForEachCore(void(*callback)());

    int DisableHv();
};