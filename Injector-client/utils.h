#pragma once
#include "includes.h"

#define StrToWStr(s) (std::wstring(s, &s[strlen(s)]).c_str())

namespace Util
{
	size_t LoadFileIntoMemory(const char* path, uint8_t** buffer);

	bool IsAddressValid(PVOID address);
};
