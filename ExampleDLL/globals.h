#pragma once
#include "includes.h"

struct DllParams
{
	uint32_t header;
	uintptr_t dll_base;
	uint32_t dll_size;
	uintptr_t payload_dll_base;
	uint32_t payload_dll_size;

	int o_present_bytes_size;
	uint8_t original_present_bytes[20];
};

namespace Global
{
	extern "C" __declspec(dllexport) extern DllParams * dll_params;
}

//template<typename t>
inline auto valid_pointer(void* address) -> bool
{
	if (((uintptr_t)address < 0x7FFFFFFFFFFF) && ((uintptr_t)address > 0x10000))
	{
		return true;
	}
	else
	{
		return false;
	}
}

#define ASSERT_VALID_PTR(pointer)	if (!valid_pointer((void*)pointer)) { return; }
