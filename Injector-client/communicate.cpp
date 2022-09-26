#pragma once
#include "communicate.h"

#define COMMAND_KEY 0xDEADBEEF

namespace Driver
{
	HANDLE driver_handle;

	bool HideMem(int32_t target_pid, uint8_t* address, uintptr_t hiding_range_size, uint32_t spoofed_protection)
	{
		HideMemoryCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = HIDE_MEMORY;
		msg.target_pid = target_pid;
		msg.hiding_range_size = hiding_range_size;
		msg.spoofed_protection = spoofed_protection;
		msg.address = (uintptr_t)address;

		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), 0, 0, &bytes, 0);
	}

	uintptr_t AllocateMemory(DWORD proc_id, DWORD size)
	{
		uintptr_t alloc_base = NULL;
		AllocMemCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = ALLOC_MEM;
		msg.proc_id = proc_id;
		msg.size = size;

		DWORD bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), &alloc_base, sizeof(uintptr_t), &bytes, 0);

		return alloc_base;
	}

	bool ProtectMemory(uint32_t proc_id, uintptr_t address, uintptr_t size, ULONG memory_prot)
	{
		ProtectMemoryCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = PROTECT_MEMORY;
		msg.proc_id = proc_id;
		msg.size = size;
		msg.address = address;
		msg.memory_protection = memory_prot;

		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), NULL, NULL, &bytes, 0);
	}

	BOOL InvokeRemoteFunc(ULONG64 start_addr, int proc_id, uintptr_t params_addr, uintptr_t real_image_size)
	{
		InvokeRemoteFunctionCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = START_THREAD;
		msg.proc_id = proc_id;
		msg.address = start_addr;	// thread addr
		msg.map_base = params_addr;	// to pass custom parameters to mapped DLL
		msg.image_size = real_image_size;
		msg.RtlAddFunctionTable_address = (uintptr_t)RtlAddFunctionTable;

		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), 0, 0, &bytes, 0);
	}

	bool SetNptHook(int32_t proc_id, size_t size, uintptr_t hook_address, uint8_t* shellcode)
	{
		NptHookMsg msg = {};

		msg.command_key = COMMAND_KEY;
		msg.message_id = SET_NPT_HOOK;
		msg.proc_id = proc_id;
		msg.size = size;
		msg.hook_address = hook_address;

		memcpy(msg.shellcode, (void*)shellcode, size);
	
		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), 0, 0, &bytes, 0);
	}

	bool ReadMem(int process_id, ULONG64 address, uint8_t* buffer, int size)
	{
		ReadCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = READ_MEM;
		msg.proc_id = process_id;
		msg.address = address;
		msg.buffer = buffer;
		msg.size = size;

		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg,
			sizeof(msg), 0, 0, &bytes, 0);
	}


	bool WriteMem(int process_id, ULONG64 address, BYTE* buffer, int size)
	{
		WriteCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = WRITE_MEM;
		msg.proc_id = process_id;
		msg.address = address;
		msg.buffer = buffer;
		msg.size = size;

		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), 0, 0, &bytes, 0);
	}

	uint64_t GetModuleBase(std::wstring module, int pid)
	{
		GetModuleMsg msg;

		uintptr_t result;

		msg.command_key = COMMAND_KEY;
		msg.message_id = MODULE_BASE;
		wcscpy(msg.module, module.c_str());
		msg.proc_id = pid;

		DWORD bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), &result, 8, &bytes, 0);

		return result;
	}

	int GetProcessId(const wchar_t* process_name)
	{
		GetProcessIdMsg msg;

		uintptr_t result;

		msg.command_key = COMMAND_KEY;
		msg.message_id = PROCESS_ID;
		wcscpy(msg.process_name, process_name);

		DWORD bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg), &result, 8, &bytes, 0);

		return result;
	}

	void Init()
	{
		driver_handle = CreateFileW( L"\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
		);
	}
}
