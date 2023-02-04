#pragma once
#include "util.h"

#define COMMAND_KEY 0xDEADBEEF

namespace Command
{
	extern PEPROCESS client;

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
		__int64 command_key;
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
		uint8_t shellcode[PAGE_SIZE];	// to store hooked page
	};

	struct AllocMemCmd : Msg
	{
		int32_t proc_id;
		size_t size;
		uintptr_t result;
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

	struct ProtectMemory : Msg
	{
		int proc_id;
		uintptr_t address;
		uint32_t memory_protection;
		uint32_t size;
	};

	struct WriteCmd : Msg
	{
		int proc_id;
		uintptr_t address;
		char* buffer;
		int size;
	};

	struct ReadCmd : Msg
	{
		int proc_id;
		uintptr_t address;
		uint32_t* buffer;
		int size;
	};

	struct HideMemoryCmd : Msg
	{
		int32_t target_pid;
		uintptr_t address;
		uintptr_t hiding_range_size;
	};

	bool Init();

	void CommandHandler(Msg* system_buffer, void* output_buffer);
}