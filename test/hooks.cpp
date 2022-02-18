#include "disassembly.h"

namespace Hooks
{
    void CreateJmpRipCode(uintptr_t jmp_target, char* output)
    {
        char jmp_rip[14] = "\xFF\x25\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC";

        memcpy(jmp_rip + 6, &jmp_target, sizeof(void*));
        memcpy((void*)output, jmp_rip, 14);
    }

    char* PlaceJmpRipHook(uintptr_t address, uintptr_t jmp_target, char** original_bytes)
    {
        auto hook_size = Disasm::LengthOfInstructions((void*)address, 14);
        
        *original_bytes = VirtualAlloc(NULL, hook_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        memcpy(*original_bytes, address, hook_size);

        DWORD old_prot;
        VirtualProtect(*original_bytes, hook_size, PAGE_READWRITE, &old_prot);

        auto shellcode = new char[hook_size];

        CreateJmpRipCode(jmp_target, shellcode);

        return shellcode;
    }
};