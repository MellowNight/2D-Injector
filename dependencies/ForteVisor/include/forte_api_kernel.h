#pragma once
#include <ntddk.h>
#include <cstdint>

enum VMMCALL_ID : uintptr_t
{
    set_mpk_hook = 0x22FFAA1166,
    set_tlb_hook = 0xAAFF226611,
    disable_hv = 0xFFAA221166,
    set_npt_hook = 0x6611AAFF22,
    remove_npt_hook = 0x1166AAFF22
};

extern "C" int __stdcall svm_vmmcall(VMMCALL_ID vmmcall_id, ...);

namespace ForteVisor
{
    /*  TLB & MPK hook Not on each core, because it's only relevant in 1 process context */
    int SetTlbHook(uintptr_t address, uint8_t* patch, size_t patch_len);

    int SetNptHook(uintptr_t address, uint8_t* patch, size_t patch_len, int32_t tag);

    int ForEachCore(void(*callback)(void* params), void* params = NULL);

    int DisableHv();
};