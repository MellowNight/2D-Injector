#pragma once
#include "util.h"

#define COMMAND_KEY 0xDEADBEEF

namespace Interface
{
	extern PEPROCESS client;

	enum messages : __int64
	{
		NONE,
		START_THREAD,
		ALLOC_MEM,
		WRITE_MEM,
		READ_MEM,
		MODULE_BASE,
		EXIT_CLEANUP,
		SET_NPT_HOOK,
		PROCESS_ID,
		PROTECT_MEMORY
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
		ULONG memory_protection;
		ULONG size;
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
		BYTE* buffer;
		int size;
	};

	bool Init();
}
