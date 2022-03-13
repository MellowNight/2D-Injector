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
		MODULE_BASE,
		EXIT_CLEANUP,
	};

	struct Msg
	{
		__int64 command_key;
		int message_id;
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
		char* buffer;
		int size;
	};

	bool Init();
}
