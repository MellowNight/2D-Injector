#pragma once
#include "driver.h"

#define COMMAND_KEY 0xDEADBEEF

namespace Driver
{
	HANDLE driver_handle;

	uintptr_t AllocateMemory(DWORD proc_id, DWORD size)
	{
		uintptr_t alloc_base = NULL;
		AllocMemCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = ALLOC_MEM;
		msg.proc_id = proc_id;
		msg.size = size;

		DWORD bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg),
			&alloc_base, sizeof(uintptr_t), &bytes, 0);

		return alloc_base;
	}

	void HideMemory(int32_t target_pid, uintptr_t address, uintptr_t hiding_range_size)
	{
		HideMemoryCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = HIDE_MEMORY;
		msg.target_pid = target_pid;
		msg.hiding_range_size = hiding_range_size;
		msg.address = address;

		DWORD bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg),
			NULL, NULL, &bytes, 0);

		return;
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
		uint8_t buffer;

		/*	1. trigger COW	*/

		Driver::ProtectMemory(proc_id, hook_address, PAGE_EXECUTE_READWRITE, 0x1000);

		/*	2. page in	*/

		auto status = Driver::ReadMem(proc_id, hook_address, &buffer, 1);
		Driver::WriteMem(proc_id, hook_address, (uint8_t*)"\xC3", 1);
		Driver::WriteMem(proc_id, hook_address, &buffer, 1);


		NptHookMsg msg = {};

		msg.command_key = COMMAND_KEY;
		msg.message_id = SET_NPT_HOOK;
		msg.proc_id = proc_id;
		msg.size = size;
		msg.hook_address = hook_address;

		memcpy(msg.shellcode, (void*)shellcode, size);
	
		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg,
			sizeof(msg), 0, 0, &bytes, 0);
	}

	bool WriteMem(int process_id, ULONG64 address, uint8_t* buffer, int size)
	{
		WriteCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = WRITE_MEM;
		msg.proc_id = process_id;
		msg.address = address;
		msg.buffer = buffer;
		msg.size = size;

		DWORD bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY, &msg,
			sizeof(msg), 0, 0, &bytes, 0);
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

	void ProtectMemory(int pid, uintptr_t address, DWORD memory_protection, ULONG size)
	{
		ProtectMemoryMsg message;

		message.command_key = COMMAND_KEY;
		message.message_id = PROTECT_MEMORY;
		message.proc_id = pid;
		message.address = address;
		message.memory_protection = memory_protection;
		message.size = size;

		DWORD bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, &message, sizeof(message), NULL, 0, &bytes, 0);

		return;
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
		driver_handle = CreateFileW(
			L"\\\\.\\PhysicalDrive0",
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL
		);
	}
}
