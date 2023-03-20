#pragma once 
#include "includes.h"

#define PAGE_SIZE 0x1000
#define PAGE_ALIGN(Va) ((PVOID)((ULONG_PTR)(Va) & ~(PAGE_SIZE - 1)))
#define RELATIVE_ADDR(insn, operand_offset, size) (ULONG64)(*(int*)((BYTE*)insn + operand_offset) + (BYTE*)insn + (int)size)

extern "C" int64_t __stdcall ZwProtectVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, PULONG RegionSize, ULONG NewProtect, PULONG OldProtect);

namespace Utils
{
    void LogToFile(const char* file_name, const char* format, ...);
    uintptr_t RvaToOffset(void* base, uintptr_t rva);
    PVOID ModuleFromAddress(uintptr_t address, PUNICODE_STRING out_name);
    void log(const char* format, ...);
    bool IsAddressValid(void* address);
    uintptr_t FindPattern(uintptr_t region_base, size_t region_size, const char* pattern, size_t pattern_size, char wildcard);
};
