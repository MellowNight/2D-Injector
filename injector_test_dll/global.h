#pragma once
#include "includes.h"

struct DllParams
{
	uint32_t header;
	uintptr_t dll_base;
	uint32_t dll_size;
	int o_present_bytes_size;
	uint8_t original_present_bytes[20];
	uint8_t* cheat_dll;
	wchar_t host_dll_name[120];
};

namespace Global
{
	extern "C" __declspec(dllexport) extern DllParams * dll_params;
};

#define PAGE_SIZE 0x1000