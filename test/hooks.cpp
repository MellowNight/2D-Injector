#include "disassembly.h"

namespace Hooks
{
    void CreateJmpRipCode(uintptr_t hook_address, uintptr_t jmp_target, uint8_t** output, uint8_t** original_bytes)
    {
        auto hook_size = Disasm::LengthOfInstructions((void*)address, 14);
        auto orig_bytes_size = hook_size + 14;

        char jmp_rip[14] = "\xFF\x25\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC";

        *original_bytes = VirtualAlloc(NULL, orig_bytes_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        DWORD old_prot;
        VirtualProtect(*original_bytes, orig_bytes_size, PAGE_READWRITE, &old_prot);

        memcpy(*original_bytes, address, hook_size);
        memcpy(*original_bytes + hook_size, jmp_rip, 14);
        memcpy(*original_bytes + hook_size + 6, &hook_address, sizeof(uintptr_t));

        *output = VirtualAlloc(NULL, hook_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        VirtualProtect(*output, hook_size, PAGE_READWRITE, &old_prot);

        memcpy(jmp_rip + 6, &jmp_target, sizeof(uintptr_t));
        memcpy(*output, jmp_rip, 14);
    }
};