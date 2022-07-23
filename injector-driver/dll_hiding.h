#pragma once
#include "includes.h"
#include "hooking.h"

uintptr_t global_dll_start, global_dll_size;

Hooks::JmpRipCode ntqvm_hook;

NTSTATUS NTAPI NtQueryVirtualMemory_Hook(_In_ HANDLE ProcessHandle, _In_opt_ PVOID BaseAddress, _In_ MEMORY_INFORMATION_CLASS MemoryInformationClass, _Out_writes_bytes_(MemoryInformationLength) PVOID MemoryInformation, _In_ SIZE_T MemoryInformationLength, _Out_opt_ PSIZE_T ReturnLength)
{
	auto status = static_cast<decltype(&NtQueryVirtualMemory)>(ntqvm_hook.original_bytes)(
		ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength
	);

	if (MemoryInformationClass == MemoryBasicInformation)
	{
		auto mem_info = (MEMORY_BASIC_INFORMATION*)MemoryInformation;

		if ((UINT64)mem_info->BaseAddress == global_dll_start || ((UINT64)mem_info->BaseAddress > global_dll_start && (UINT64)mem_info->BaseAddress < (global_dll_start + global_dll_size)))
		{
			mem_info->AllocationBase = 0;
			mem_info->AllocationProtect = 0;
			mem_info->State = MEM_FREE;
			mem_info->Protect = PAGE_NOACCESS;
			mem_info->Type = 0;
		}
	}

	return status;
}


uintptr_t NtQueryVirtualMem;
void HookNTQVM()
{
	ForteVisor::SetNptHook((uintptr_t)NtQueryVirtualMem, (uint8_t*)ntqvm_hook.hook_code, ntqvm_hook.hook_size, NULL);
}