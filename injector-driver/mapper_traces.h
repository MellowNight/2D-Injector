#pragma once
#include "util.h"

typedef struct _POOL_TRACKER_BIG_PAGES
{
	volatile ULONGLONG Va;                              //0x0
	ULONG Key;                                          //0x8
	ULONG Pattern : 8;                                    //0xc
	ULONG PoolType : 12;							      //0xc
	ULONG SlushSize : 12;							     //0xc
	ULONGLONG NumberOfBytes;                            //0x10
}POOL_TRACKER_BIG_PAGES, * PPOOL_TRACKER_BIG_PAGES;


bool FindPoolTable(ULONG64* pPoolBigPageTable, ULONG64* pPoolBigPageTableSize);
bool RemoveFromBigPool(PVOID Address);