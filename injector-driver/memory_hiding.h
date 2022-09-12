#pragma once
#include "includes.h"
#include "forte_api_kernel.h"
#include "hooking.h"
#include "util.h"

uintptr_t hiding_range_start, hiding_range_size;
uint32_t protect_value;
uint32_t target_pid;

Hooks::JmpRipCode ntqvm_hook;

NTSTATUS NTAPI NtQueryVirtualMemory_Hook(_In_ HANDLE ProcessHandle, _In_opt_ PVOID BaseAddress, _In_ MEMORY_INFORMATION_CLASS MemoryInformationClass, _Out_writes_bytes_(MemoryInformationLength) PVOID MemoryInformation, _In_ SIZE_T MemoryInformationLength, _Out_opt_ PSIZE_T ReturnLength)
{
	auto status = static_cast<decltype(&NtQueryVirtualMemory)>(ntqvm_hook.original_bytes)(
		ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength
	);

	if (MemoryInformationClass == MemoryBasicInformation && (PsGetCurrentProcessId() == (HANDLE)target_pid))
	{
		auto mem_info = (MEMORY_BASIC_INFORMATION*)MemoryInformation;

		if (MmIsAddressValid(mem_info))
		{
			if (((UINT64)mem_info->BaseAddress >= hiding_range_start && (UINT64)mem_info->BaseAddress < (hiding_range_start + hiding_range_size)))
			{
				mem_info->Protect = protect_value;
				mem_info->AllocationProtect = protect_value;
			}
		}
	}

	return status;
}


void HookNTQVM()
{
	ULONG nt_size = 0;
	auto ntoskrnl = (uintptr_t)Utils::GetKernelModule(&nt_size, RTL_CONSTANT_STRING(L"ntoskrnl.exe"));

	auto reference = Utils::FindPattern(ntoskrnl, nt_size, "\xE8\x00\x00\x00\x00\x8B\xF8\x89\x44\x24\x40\x85\xC0\x78\x3B", 15, 0x00);

	auto ntqvm = RELATIVE_ADDR(reference, 1, 5);

	ntqvm_hook = Hooks::JmpRipCode{ ntqvm, (uintptr_t)NtQueryVirtualMemory_Hook };

	ForteVisor::SetNptHook((uintptr_t)ntqvm, (uint8_t*)ntqvm_hook.hook_code, ntqvm_hook.hook_size, NULL);
}