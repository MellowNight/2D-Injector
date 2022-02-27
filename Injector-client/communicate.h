#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

#define COMMAND_KEY 0xDEADBEEF

namespace Driver
{
	HANDLE driver_handle;

	enum messages : __int64
	{
		NONE,
		START_THREAD,
		ALLOC_MEM,
		WRITE_MEM,
		MODULE_BASE,
		EXIT_CLEANUP,
	};

	struct Msg
	{
		int64_t command_key;
		int message_id;
	};

	struct AllocMemCmd : Msg
	{
		DWORD proc_id;
		DWORD size;
	};

	struct InvokeRemoteFunctionCmd : Msg
	{
		int proc_id;
		uintptr_t map_base;
		uintptr_t address;
		uintptr_t image_size;
	};

	struct GetModuleMsg : Msg
	{
		DWORD proc_id;
		wchar_t module[50];
	};

	struct WriteCmd : Msg
	{
		int proc_id;
		uintptr_t address;
		BYTE* buffer;
		int size;
	};

	struct InitMsg : Msg
	{
		int proc_id;
	};

	uintptr_t AllocateMemory(DWORD proc_id, DWORD size)
	{
		uintptr_t alloc_base = NULL;
		AllocMemCmd msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = ALLOC_MEM;
		msg.proc_id = proc_id;
		msg.size = size;

		DWORD bytes;

		DeviceIoControl(driver_handle, 0, &msg, sizeof(msg), 
			&alloc_base, sizeof(uintptr_t), &bytes, 0);

		return alloc_base;
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

		DWORD bytes;

		return DeviceIoControl(driver_handle, 0, &msg, sizeof(msg), 0, 0, &bytes, 0);
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

		return DeviceIoControl(driver_handle, 0, &msg,
			sizeof(msg), 0, 0, &bytes, 0);
	}

	void ExitDriver()
	{
		auto msg = EXIT_CLEANUP;
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

		DeviceIoControl(driver_handle, 0, &msg, sizeof(msg), &result, 8, &bytes, 0);

		return result;
	}
}
