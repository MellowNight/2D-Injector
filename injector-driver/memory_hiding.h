#pragma once
#include "includes.h"
#include "forte_api_kernel.h"
#include "hooking.h"
#include "util.h"

uintptr_t hiding_range_start, hiding_range_size;
uint32_t protect_value;
uint32_t target_pid;
bool ntqvm_nothooked = true;

Hooks::JmpRipCode ntqvm_hook;
Hooks::JmpRipCode ntprotect_hook;

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
				mem_info->RegionSize = 0x5; // make code upload only 5 bytes at a time?
				mem_info->Protect = protect_value;
				mem_info->AllocationProtect = protect_value;
			}
		}
	}

	return status;
}

/*	because byfron forces it to be RW	*/

NTSTATUS NTAPI NtProtectVirtualMemory_Hook
(
	IN HANDLE ProcessHandle,
	IN OUT PVOID* BaseAddress,
	IN OUT SIZE_T* NumberOfBytesToProtect,
	IN ULONG NewAccessProtection,
	OUT PULONG OldAccessProtection
)
{
	if (MmIsAddressValid(BaseAddress) && MmIsAddressValid(NumberOfBytesToProtect))
	{
		if (((*(uintptr_t*)BaseAddress >= hiding_range_start) && ((*(uintptr_t*)BaseAddress + *(uintptr_t*)NumberOfBytesToProtect) <= (hiding_range_size + hiding_range_start))) && (PsGetCurrentProcessId() == (HANDLE)target_pid))
		{
			DbgPrint("caught apex legends trying to change our memory protecton!!! \n");

			return STATUS_SUCCESS;
		}
	}

	return static_cast<decltype(&NtProtectVirtualMemory_Hook)>(ntprotect_hook.original_bytes)(
		ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection
	);
}

void HookMemoryProtectionFn()
{
	if (ntqvm_nothooked)
	{
		ULONG nt_size = 0;
		auto ntoskrnl = (uintptr_t)Utils::GetKernelModule(&nt_size, RTL_CONSTANT_STRING(L"ntoskrnl.exe"));

		auto reference = Utils::FindPattern(ntoskrnl, nt_size, "\xE8\x00\x00\x00\x00\x8B\xF8\x89\x44\x24\x40\x85\xC0\x78\x3B", 15, 0x00);

		auto ntqvm = RELATIVE_ADDR(reference, 1, 5);

		ntqvm_hook = Hooks::JmpRipCode{ ntqvm, (uintptr_t)NtQueryVirtualMemory_Hook };

		ForteVisor::SetNptHook((uintptr_t)ntqvm, (uint8_t*)ntqvm_hook.hook_code, ntqvm_hook.hook_size, NULL);


	/*	auto ntprotect = Utils::FindPattern(ntoskrnl, nt_size, 
			"\x40\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x45\x8B\xE1\x4C\x89\x44\x24\x00\x4C\x8B\xFA", 
			48, 0x00);

		ntprotect_hook = Hooks::JmpRipCode{ ntprotect, (uintptr_t)NtProtectVirtualMemory_Hook };

		ForteVisor::SetNptHook((uintptr_t)ntprotect, (uint8_t*)ntprotect_hook.hook_code, ntprotect_hook.hook_size, NULL);*/

		ntqvm_nothooked = false;
	}
}