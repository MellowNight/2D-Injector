#pragma once
#include "includes.h"

#define StrToWStr(s) (std::wstring(s, &s[strlen(s)]).c_str())

#define PAGE_ALIGN(Va) ((uintptr_t)((ULONG_PTR)(Va) & ~(PAGE_SIZE - 1)))

namespace Util
{
	size_t LoadFileIntoMemory(const char* path, uint8_t** buffer);

	bool IsAddressValid(PVOID address);
};
