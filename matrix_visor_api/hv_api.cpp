#include "hv_api.h"

namespace HyperApi
{
    int SetTlbHook(uintptr_t address, uint8_t* patch, size_t patch_len)
    {
        return vmmcall(VMMCALL_ID::set_tlb_hook, address, patch, patch_len);
    }

    int DisableHv()
    {
        SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);
        auto core_count = sys_info.dwNumberOfProcessors;

        for (auto idx = 0; idx < core_count; ++idx)
        {
            auto affinity = pow(2, idx);
            
            SetThreadAffinityMask(GetCurrentThread(), affinity);

            vmmcall(VMMCALL_ID::disable_hv);
        }
    }
};