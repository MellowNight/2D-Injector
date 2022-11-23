#include "pch.h"
#include "utils.h"

namespace Utils
{
	void TriggerCOWAndPageIn(void* address)
	{
		uint8_t buffer;

		/*	1. page in	*/

		buffer = *(uint8_t*)address;

		/*	2. trigger COW	*/

		WriteToReadOnly(address, (uint8_t*)"\xC3", 1);
		WriteToReadOnly(address, &buffer, 1);
	}

	void LogToFile(const char* file_name, const char* format, ...)
	{
		auto file_handle = CreateFileA(
			file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
		);

		SetFilePointer(file_handle, 0, 0, FILE_END);

		if (GetLastError() != 183 && GetLastError() != 0)
		{
			return;
		}

		char buffer[256] = { 0 };

		va_list args;
		va_start(args, format);
		vsnprintf(buffer, 255, format, args);

		size_t written;

		WriteFile(file_handle, buffer, strlen(buffer), (LPDWORD)&written, NULL);

		if (GetLastError() != 183 && GetLastError() != 0)
		{
			MessageBoxA(NULL, "WriteFile GetLastError", std::to_string(GetLastError()).c_str(), MB_OK);
		}

		va_end(args);

		CloseHandle(file_handle);
	};

	void WriteToReadOnly(void* address, uint8_t* bytes, size_t len)
	{
		DWORD old_prot;
		VirtualProtect((LPVOID)address, len, PAGE_EXECUTE_READWRITE, &old_prot);
		memcpy((void*)address, (void*)bytes, len);
		VirtualProtect((LPVOID)address, len, old_prot, 0);
	}

	void log(const char* format, ...)
	{
		static char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);
		buffer[200] = '\0';
		OutputDebugStringA(buffer);
	}

	size_t LoadFileIntoMemory(const wchar_t* path, char** buffer)
	{
		auto file_handle = CreateFileW(
			path, GENERIC_ALL, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
		);

		auto size = GetFileSize(file_handle, NULL);

		*buffer = (char*)VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		SetFilePointer(file_handle, 0, 0, 0);

		DWORD bytes;

		ReadFile(file_handle, *buffer, size, &bytes, NULL);

		CloseHandle(file_handle);

		return size;
	}

	bool IsAddressValid(void* address)
	{
		if (((uintptr_t)address < 0x7FFFFFFFFFFF) && ((uintptr_t)address > 0x1000))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

#pragma optimize( "", off )

	uintptr_t FindPattern(uintptr_t region_base, size_t region_size, const char* pattern, size_t pattern_size, char wildcard)
	{
		for (auto byte = (char*)region_base; byte < (char*)region_base + region_size;
			++byte)
		{
			bool found = true;

			for (char* pattern_byte = (char*)pattern, *begin = byte; pattern_byte < pattern + pattern_size; ++pattern_byte, ++begin)
			{
				if (*pattern_byte != *begin && *pattern_byte != wildcard)
				{
					found = false;
				}
			}

			if (found)
			{
				return (uintptr_t)byte;
			}
		}

		return 0;
	}

#pragma optimize( "", on )

}