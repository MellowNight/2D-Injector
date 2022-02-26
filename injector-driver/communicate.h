#pragma once
#include "util.h"
#include "disk_hook.h"

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
		wchar_t	section_name[60];
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

	bool Init();
}
