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

	enum DRIVER_CMD : uint32_t
	{
		none,
		write_mem,
		read_mem,
		module_base,
		remote_npt_hook,
		process_id,
		hide_memory,
		protect_memory
	};

	struct Msg
	{
		int64_t command_key;
		int message_id;
	};

	struct GetProcessIdMsg : Msg
	{
		wchar_t process_name[50];
	};

	struct NptHookMsg : Msg
	{
		int32_t proc_id;
		size_t size;
		uintptr_t hook_address;
		uint8_t shellcode[0x1000];
	};

	struct AllocMemCmd : Msg
	{
		int32_t proc_id;
		size_t size;
		wchar_t	section_name[60];
	};

	struct InvokeRemoteFunctionCmd : Msg
	{
		int proc_id;
		uintptr_t map_base;
		uintptr_t address;
		uintptr_t image_size;
		uintptr_t RtlAddFunctionTable_address;
	};

	struct GetModuleMsg : Msg
	{
		uint32_t proc_id;
		wchar_t module[50];
	};

	struct WriteCmd : Msg
	{
		int proc_id;
		uintptr_t address;
		uint8_t* buffer;
		int size;
	};

	struct ReadCmd : Msg
	{
		int proc_id;
		uintptr_t address;
		uint8_t* buffer;
		int size;
	};

	struct ProtectMemoryMsg : Msg
	{
		int proc_id;
		uintptr_t address;
		uint32_t memory_protection;
		uint32_t size;
	};

	struct HideMemoryCmd : Msg
	{
		int32_t target_pid;
		uintptr_t address;
		uintptr_t hiding_range_size;
	};

	void HideMemory(int32_t target_pid, uintptr_t address, uintptr_t hiding_range_size);

	bool SetNptHook(int32_t proc_id, size_t size, uintptr_t hook_address, uint8_t* shellcode);

	bool WriteMem(int process_id, uint64_t address, uint8_t* buffer, int size);

	uint64_t GetModuleBase(std::wstring module, int pid);

	int GetProcessId(const wchar_t* process_name);

	void Init();

	void ProtectMemory(int pid, uintptr_t address, uint32_t memory_protection, uint32_t size);

	bool ReadMem(int process_id, uint64_t address, uint8_t* buffer, int size);
}
