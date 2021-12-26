#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>

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
		INIT_COMM
	};

	struct Msg
	{
		__int64 command_key;
		int message_id;
	};

	struct AllocMemMsg : Msg
	{
		DWORD proc_id;
		DWORD size;
		uintptr_t* result;
		wchar_t section_name[60];
	};

	struct StartThreadMsg : Msg
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
		uintptr_t* out_buf;
	};

	struct WriteMsg : Msg
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

	uintptr_t MapMem(DWORD proc_id, DWORD size, wchar_t* section_name)
	{
		uintptr_t mapped_base;
		AllocMemMsg msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = ALLOC_MEM;
		msg.proc_id = proc_id;
		msg.size = size;
		msg.result = &mapped_base;
		wcscpy(msg.section_name, section_name);	


		DWORD bytes;

		DeviceIoControl(driver_handle, 0, &msg,
			sizeof(msg), 0, 0, &bytes, 0);

		return mapped_base;
	}

	BOOL StartThread(ULONG64 start_addr, int proc_id, uintptr_t params_addr, uintptr_t real_image_size)
	{
		StartThreadMsg msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = START_THREAD;
		msg.proc_id = proc_id;
		msg.address = start_addr;	// thread addr
		msg.map_base = params_addr;	// to pass base address to mapped DLL
		msg.image_size = real_image_size;

		DWORD bytes;

		return DeviceIoControl(driver_handle, 0, &msg,
			sizeof(msg), 0, 0, &bytes, 0);
	}

	BOOL WriteMem(int process_id, ULONG64 address, BYTE* buffer, int size)
	{
		WriteMsg msg;

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
		//memcpy(Driver::buffer, &msg, sizeof(msg));
		//while (*(int*)Driver::buffer != 0)
		//{
		//	// for some reason the driver misses commands if i don't add this
		//	Sleep(3);
		//}
	}

	uint64_t GetModuleBase(std::wstring module, int pid)
	{
		GetModuleMsg msg;

		uintptr_t result;

		msg.command_key = COMMAND_KEY;
		msg.message_id = MODULE_BASE;
		wcscpy(msg.module, module.c_str());
		msg.proc_id = pid;
		msg.out_buf = &result;

		DWORD bytes;

		DeviceIoControl(driver_handle, 0, &msg, sizeof(msg), 0, 0, &bytes, 0);

		return result;
	}

	void Init()
	{
		driver_handle = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

		InitMsg msg;

		msg.command_key = COMMAND_KEY;
		msg.message_id = INIT_COMM;
		msg.proc_id = GetCurrentProcessId();

		DWORD bytes;

		DeviceIoControl(driver_handle, 0, &msg, sizeof(msg), 0, 0, &bytes, 0);
	}
}
