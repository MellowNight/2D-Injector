#pragma once
#include <stdint.h>
#include "util.h"

//0x18 bytes (sizeof)
struct _POOL_TRACKER_BIG_PAGES
{
    volatile ULONGLONG Va;                                                  //0x0
    ULONG Key;                                                              //0x8
    ULONG Pattern : 8;                                                        //0xc
    ULONG PoolType : 12;                                                      //0xc
    ULONG SlushSize : 12;                                                     //0xc
    ULONGLONG NumberOfBytes;                                                //0x10
};

bool FindPoolTable(uint64_t* pPoolBigPageTable, uint64_t* pPoolBigPageTableSize)
{
    ULONG nt_size; 
    void* nt_base = utils::GetKernelModule(&nt_size, RTL_CONSTANT_STRING(L"ntoskrnl.exe"));
    auto Pat1 = (UCHAR*)"\xE8\xCC\xCC\xCC\xCC\x83\x67\x0C\x00";

    uintptr_t ExProtectPoolExCallInstructionsAddress;
    (PVOID)utils::BBScan(".text", Pat1, 0xCC, 9, &ExProtectPoolExCallInstructionsAddress, nt_base);

    if (!ExProtectPoolExCallInstructionsAddress)
        return false;

    PVOID ExProtectPoolExAddress = (void*)RELATIVE_ADDR(ExProtectPoolExCallInstructionsAddress, 1, 5);

    if (!ExProtectPoolExAddress)
        return false;

    PVOID PoolBigPageTableInstructionAddress = (PVOID)((ULONG64)ExProtectPoolExAddress + 0x95);
    *pPoolBigPageTable = (uint64_t)RELATIVE_ADDR(PoolBigPageTableInstructionAddress, 3, 7);

    PVOID PoolBigPageTableSizeInstructionAddress = (PVOID)((ULONG64)ExProtectPoolExAddress + 0x8E);
    *pPoolBigPageTableSize = (uint64_t)RELATIVE_ADDR(PoolBigPageTableSizeInstructionAddress, 3, 7);

    return true;
}

bool RemoveFromBigPool(uint64_t Address)
{
    uint64_t pPoolBigPageTable = 0;
    uint64_t pPoolBigPageTableSize = 0;

    if (FindPoolTable(&pPoolBigPageTable, &pPoolBigPageTableSize))
    {
        _POOL_TRACKER_BIG_PAGES* PoolBigPageTable = 0;
        RtlCopyMemory(&PoolBigPageTable, (PVOID)pPoolBigPageTable, 8);
        SIZE_T PoolBigPageTableSize = 0;
        RtlCopyMemory(&PoolBigPageTableSize, (PVOID)pPoolBigPageTableSize, 8);

        for (int i = 0; i < PoolBigPageTableSize; i++)
        {
            if (PoolBigPageTable[i].Va == Address || PoolBigPageTable[i].Va == (Address + 0x1))
            {
                PoolBigPageTable[i].Va = 0x1;
                PoolBigPageTable[i].NumberOfBytes = 0x0;
                return true;
            }
        }

        return false;
    }

    return false;
}