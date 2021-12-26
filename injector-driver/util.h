#pragma once
#include "undocumented.h"
#include "ia32_define.h"

namespace utils
{
	PVOID	GetVaFromPfn(ULONG64 pfn);

	typedef int (*PageTableOperation)(PT_ENTRY_64*);
	PT_ENTRY_64* GetPte(PVOID VirtualAddress, ULONG64 Pml4BasePa, PageTableOperation Operation);

	_MMVAD_SHORT* FindVadNode(UINT_PTR virtual_page, PEPROCESS process);

	ULONG64 GetSection(ULONG64  base, IN PCCHAR section, int* size);

	HANDLE  GetProcessId(PCWSTR     processName);

	PVOID WriteFile(PVOID buffer, const wchar_t* FileName, ULONG64 size);

	PVOID GetKernelModule(
		OUT PULONG pSize, 
		UNICODE_STRING DriverName
	);

	PVOID CreateFile(
		PVOID buffer, 
		const wchar_t* FileName, ULONG64 size
	);
	PVOID GetUserModule(IN PEPROCESS pProcess, IN PUNICODE_STRING ModuleName);
	PVOID ReadFile(PVOID buffer, const wchar_t* FileName, ULONG64 size, HANDLE* hFile);

	KIRQL DisableWP();

	void EnableWP(KIRQL tempirql);

	NTSTATUS BBSearchPattern(
		IN PCUCHAR pattern, 
		IN UCHAR wildcard, 
		IN ULONG_PTR len,
		IN const VOID* base, 
		IN ULONG_PTR size,
		OUT PVOID* ppFound
	);

	NTSTATUS BBScan(
		IN PCCHAR section,
		IN PCUCHAR pattern, 
		IN UCHAR wildcard,
		IN ULONG_PTR len, 
		OUT ULONG64* ppFound, 
		PVOID base
	);

	void dump_driver(
		PVOID baseAddress, 
		const wchar_t* path, 
		bool fixPe
	);
	PVOID IATHook(
		PVOID lpBaseAddress, 
		CHAR* lpcStrImport, 
		PVOID lpFuncAddress
	);

	PMDL LockPages(PVOID VirtualAddress, LOCK_OPERATION  operation, int size = PAGE_SIZE);

	NTSTATUS UnlockPages(PMDL mdl);

	void* GetExport(uintptr_t base, char* export_name);
}