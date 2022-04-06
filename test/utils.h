#pragma once
#include <stdint.h>
#include <intrin.h>
#include <wchar.h>
#include <winternl.h>
#include <Windows.h>

#define PAGE_SIZE 0x1000
#define PAGE_ALIGN(Va) ((PVOID)((ULONG_PTR)(Va) & ~(PAGE_SIZE - 1)))
#define RELATIVE_ADDR(insn, operand_offset, size) (ULONG64)(*(int*)((BYTE*)insn + operand_offset) + (BYTE*)insn + (int)size)

extern "C" __int64 __stdcall ZwProtectVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, PULONG RegionSize, ULONG NewProtect, PULONG OldProtect);

namespace Utils
{
    uintptr_t RvaToOffset(void* base, uintptr_t rva);
    PVOID ModuleFromAddress(uintptr_t address, PUNICODE_STRING out_name);
    void log(const char* format, ...);
    bool IsAddressValid(void* address);
};

