#include "mapper_traces.h"
#include "communicate.h"

bool FindPoolTable(PVOID* pPoolBigPageTable, PVOID* pPoolBigPageTableSize)
{
	ULONG nt_size = 0;
	auto ntos_base = (uintptr_t)Utils::GetKernelModule(&nt_size, RTL_CONSTANT_STRING(L"ntoskrnl.exe"));

	auto ex_protect_pool_call = Utils::FindPattern(ntos_base, nt_size, "\xE8\xCC\xCC\xCC\xCC\x83\x67\x0C\x00", 9, '\xCC');

	if (!ex_protect_pool_call)
		return false;

	auto ExProtectPoolExAddress = RELATIVE_ADDR(ex_protect_pool_call, 1, 5);

	if (!ExProtectPoolExAddress)
		return false;

	PVOID PoolBigPageTableInstructionAddress = (PVOID)((ULONG64)ExProtectPoolExAddress + 0x95);
	*pPoolBigPageTable = (PVOID)RELATIVE_ADDR(PoolBigPageTableInstructionAddress, 3, 7);

	PVOID PoolBigPageTableSizeInstructionAddress = (PVOID)((ULONG64)ExProtectPoolExAddress + 0x8E);
	*pPoolBigPageTableSize = (PVOID)RELATIVE_ADDR(PoolBigPageTableSizeInstructionAddress, 3, 7);

	return true;
}

bool RemoveFromBigPool(PVOID Address)
{
	PVOID pPoolBigPageTable = 0;
	PVOID pPoolBigPageTableSize = 0;

	if (FindPoolTable(&pPoolBigPageTable, &pPoolBigPageTableSize))
	{
		PPOOL_TRACKER_BIG_PAGES PoolBigPageTable = *(PPOOL_TRACKER_BIG_PAGES*)pPoolBigPageTable;

		SIZE_T PoolBigPageTableSize = *(__int64*)pPoolBigPageTableSize;

		for (int i = 0; i < PoolBigPageTableSize; i++)
		{
			if (PoolBigPageTable[i].Va == (DWORD64)Address || PoolBigPageTable[i].Va == ((DWORD64)Address + 0x1))
			{
				PoolBigPageTable[i].Va = 0x1;
				PoolBigPageTable[i].NumberOfBytes = 0x0;


				DbgPrint("freed pool entry %p \n", Address);

				return true;
			}
		}

		return false;
	}

	return false;
}