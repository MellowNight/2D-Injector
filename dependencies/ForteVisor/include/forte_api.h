#pragma once
#include "pch.h"
#include "framework.h"
#include <cstdint>
#include <Windows.h>
#include <math.h>

enum VMMCALL_ID : uintptr_t
{
    set_mpk_hook = 0x22FFAA1166,
    disable_hv = 0xFFAA221166,
    set_npt_hook = 0x6611AAFF22,
    remove_npt_hook = 0x1166AAFF22,
    is_hv_present = 0xEEFF,
    remap_page_ncr3_specific = 0x8236FF,
};

enum NCR3_DIRECTORIES
{
    primary,
    noexecute,
    sandbox,
};

extern "C" int __stdcall svm_vmmcall(VMMCALL_ID vmmcall_id, ...);

namespace ForteVisor
{
    int RemapPageSingleNcr3(uintptr_t old_page, uintptr_t new_page, int32_t core_id);

    int SetNptHook(uintptr_t address, uint8_t* patch, size_t patch_len, int32_t noexecute_cr3_id, int32_t tag);

    int RemoveNptHook(int32_t tag);

    int ForEachCore(void(*callback)(void* params), void* params = NULL);

    int DisableHv();
};