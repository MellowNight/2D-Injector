#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

#define COMMAND_KEY 0xDEADBEEF

namespace Driver
{
	extern HANDLE driver_handle;

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

	uintptr_t AllocateMemory(DWORD proc_id, DWORD size);

	BOOL InvokeRemoteFunc(ULONG64 start_addr, int proc_id, uintptr_t params_addr, uintptr_t real_image_size);

	bool WriteMem(int process_id, ULONG64 address, BYTE* buffer, int size);

	void ExitDriver();

	uint64_t GetModuleBase(std::wstring module, int pid);

	void Init();
}
