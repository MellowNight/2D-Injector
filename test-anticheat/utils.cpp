#include "utils.h"
#include <stdio.h>
#include <winternl.h>

namespace Utils
{
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
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        buffer[200] = '\0';
        OutputDebugStringA(buffer);
    }
}