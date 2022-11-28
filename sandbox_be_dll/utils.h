#include "includes.h"

#define PeHeader(image) ((IMAGE_NT_HEADERS64*)((uint64_t)image + ((IMAGE_DOS_HEADER*)image)->e_lfanew))

namespace Utils
{
	PVOID ModuleFromAddress(uintptr_t address, PUNICODE_STRING out_name);

	void TriggerCOWAndPageIn(void* address);

	void LogToFile(const char* file_name, const char* format, ...);

	void WriteToReadOnly(void* address, uint8_t* bytes, size_t len);

	void log(const char* format, ...);

	size_t LoadFileIntoMemory(const wchar_t* path, char** buffer);

	bool IsAddressValid(void* address);
		
#pragma optimize( "", off )

	uintptr_t FindPattern(uintptr_t region_base, size_t region_size, const char* pattern, size_t pattern_size, char wildcard);

#pragma optimize( "", on )

}