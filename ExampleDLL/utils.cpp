#include "pch.h"
#include "utils.h"
#include "includes.h"
#include <winternl.h>

#define PeHeader(image) ((IMAGE_NT_HEADERS64*)((uint64_t)image + ((IMAGE_DOS_HEADER*)image)->e_lfanew))

namespace Utils
{
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


    uintptr_t RvaToOffset(void* base, uintptr_t rva)
    {
        auto Pe = PeHeader(base);

        IMAGE_SECTION_HEADER* pSection = (IMAGE_SECTION_HEADER*)(Pe + 1);

        for (int i = 0; i < Pe->FileHeader.NumberOfSections; ++i)
        {
            if (rva >= pSection[i].VirtualAddress && rva < (pSection[i].VirtualAddress + pSection[i].SizeOfRawData))
            {
                rva -= pSection[i].VirtualAddress;
                rva += pSection[i].PointerToRawData;
            }
        }

        return rva;
    }

    enum Offset
    {
        LDR_IMAGESIZE = 0x40,
    };

    PVOID ModuleFromAddress(uintptr_t address, PUNICODE_STRING out_name)
    {
        auto peb = (PPEB)__readgsqword(0x60);

        LIST_ENTRY head = peb->Ldr->InMemoryOrderModuleList;

        LIST_ENTRY curr = head;

        while (curr.Flink != head.Blink)
        {
            _LDR_DATA_TABLE_ENTRY* mod = (_LDR_DATA_TABLE_ENTRY*)CONTAINING_RECORD(curr.Flink, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

            if ((uintptr_t)mod->DllBase <= address &&
                address <= ((uintptr_t)mod->DllBase + *(uintptr_t*)((uintptr_t)mod + LDR_IMAGESIZE))
                )
            {
                *out_name = mod->FullDllName;
                return mod->DllBase;
            }

            curr = *curr.Flink;
        }
        return NULL;
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
}