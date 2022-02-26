#include "pch.h"
#include "utils.h"
#include "includes.h"

#define PeHeader(image) ((IMAGE_NT_HEADERS64*)((uint64_t)image + ((IMAGE_DOS_HEADER*)image)->e_lfanew))

namespace Utils
{
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
}