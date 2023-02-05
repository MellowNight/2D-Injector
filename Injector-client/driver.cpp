#pragma once
#include "driver.h"

#define COMMAND_KEY 0xDEADBEEF

namespace Driver
{
	HANDLE driver_handle;
	
	void HideMemory(int32_t target_pid, uintptr_t address, uintptr_t hiding_range_size)
	{
		HideMemoryCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = hide_memory;
		msg.target_pid = target_pid;
		msg.hiding_range_size = hiding_range_size;
		msg.address = address;

		uint32_t bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, &msg, sizeof(msg),
			NULL, NULL, (LPDWORD)&bytes, 0);

		return;
	}

	bool SetNptHook(int32_t proc_id, size_t size, uintptr_t hook_address, uint8_t* shellcode)
	{
		uint8_t buffer;

		/*	trigger COW	*/

		Driver::ProtectMemory(proc_id, hook_address, PAGE_EXECUTE_READWRITE, 0x1000);

		NptHookMsg msg = {};

		msg.command_key = COMMAND_KEY;
		msg.message_id = remote_npt_hook;
		msg.proc_id = proc_id;
		msg.size = size;
		msg.hook_address = hook_address;

		memcpy(msg.shellcode, (void*)shellcode, size);
	
		uint32_t bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY,
			&msg, sizeof(msg), 0, 0, (LPDWORD)&bytes, 0);
	}

	bool WriteMem(int process_id, uint64_t address, uint8_t* buffer, int size)
	{
		WriteCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = write_mem;
		msg.proc_id = process_id;
		msg.address = address;
		msg.buffer = buffer;
		msg.size = size;

		uint32_t bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY,
			&msg, sizeof(msg), 0, 0, (LPDWORD)&bytes, 0);
	}

	bool ReadMem(int process_id, uint64_t address, uint8_t* buffer, int size)
	{
		ReadCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = read_mem;
		msg.proc_id = process_id;
		msg.address = address;
		msg.buffer = buffer;
		msg.size = size;

		uint32_t bytes;

		return DeviceIoControl(driver_handle, COMMAND_KEY,
			&msg, sizeof(msg), 0, 0, (LPDWORD) & bytes, 0);
	}

	uint64_t GetModuleBase(std::wstring module, int pid)
	{
		GetModuleMsg msg;

		uintptr_t result;

		msg.command_key = COMMAND_KEY;
		msg.message_id = module_base;
		wcscpy(msg.module, module.c_str());
		msg.proc_id = pid;

		uint32_t bytes;

		DeviceIoControl(driver_handle,
			COMMAND_KEY, &msg, sizeof(msg), &result, 8, (LPDWORD)&bytes, 0);

		return result;
	}

	void ProtectMemory(int pid, uintptr_t address, uint32_t memory_protection, uint32_t size)
	{
		ProtectMemoryMsg message;

		message.command_key = COMMAND_KEY;
		message.message_id = protect_memory;
		message.proc_id = pid;
		message.address = address;
		message.memory_protection = memory_protection;
		message.size = size;

		uint32_t bytes;

		DeviceIoControl(driver_handle, 
			COMMAND_KEY, &message, sizeof(message), NULL, 0, (LPDWORD)&bytes, 0);

		return;
	}

	int GetProcessId(const wchar_t* process_name)
	{
		GetProcessIdMsg msg;

		uintptr_t result;

		msg.command_key = COMMAND_KEY;
		msg.message_id = process_id;
		wcscpy(msg.process_name, process_name);

		uint32_t bytes;

		DeviceIoControl(driver_handle, COMMAND_KEY, 
			&msg, sizeof(msg), &result, 8, (LPDWORD) & bytes, 0);

		return result;
	}

	void Init()
	{
		driver_handle = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}
