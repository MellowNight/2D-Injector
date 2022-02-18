
enum VMMCALL_ID : uintptr_t
{
    set_tlb_hook = 0xAAFF226611,
    disable_hv = 0xFFAA221166,
};

extern "C" int __stdcall vmmcall(VMMCALL_ID vmmcall_id, ...);

namespace HyperApi
{
    int SetTlbHook(uintptr_t address, uint8_t* patch, size_t patch_len);

    int DisableHv();
};