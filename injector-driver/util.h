#pragma once
#include "undocumented.h"
#include "ia32_define.h"

#define	RELATIVE_ADDR(insn, operand_offset, size) (ULONG64)(*(int*)((BYTE*)insn + operand_offset) + (BYTE*)insn + (int)size)

namespace Utils
{
	NTSTATUS WriteMem(
		int32_t target_pid, 
		uintptr_t address, 
		void* buffer, 
		size_t size
	);

	NTSTATUS ReadMem(
		int32_t target_pid, 
		uintptr_t address,
		void* buffer, 
		size_t size
	);

	HANDLE GetProcessId(
		PCWSTR processName
	);

	PVOID WriteFile(
		PVOID buffer, 
		const wchar_t* FileName, 
		ULONG64 size
	);

	PVOID GetKernelModule(
		OUT PULONG pSize, 
		UNICODE_STRING DriverName
	);

	PVOID CreateFile(
		PVOID buffer, 
		const wchar_t* FileName, ULONG64 size
	);

	PVOID GetUserModule(
		IN PEPROCESS pProcess, 
		IN PUNICODE_STRING ModuleName
	);

	PVOID ReadFile(
		PVOID buffer, 
		const wchar_t* FileName, 
		ULONG64 size, HANDLE* 
		hFile
	);

	KIRQL DisableWP();

	void EnableWP(
		KIRQL tempirql
	);

	inline PMDL LockPages(PVOID VirtualAddress, LOCK_OPERATION  operation, int size = PAGE_SIZE)
	{
		PMDL mdl = IoAllocateMdl(VirtualAddress, size, FALSE, FALSE, nullptr);

		MmProbeAndLockPages(mdl, KernelMode, operation);

		return mdl;
	}

	NTSTATUS UnlockPages(PMDL mdl);

	void* GetExport(uintptr_t base, char* export_name);
}