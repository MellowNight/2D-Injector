// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "portable_executable.h"
#include "forte_api.h"
#include "be_bypass.h"

enum INJECTOR_CONSTANTS
{
    mapped_dll_header = 0x12345678,
    entrypoint_npt_hook = 0xAAAA
};

struct DllParams
{
    uint32_t header;
    size_t dll_size;
};

bool already_called = false;

extern "C" __declspec(dllexport) void HookEntryPoint()
{
    if (!already_called)
    {
        already_called = true;

        ForteVisor::RemoveNptHook(INJECTOR_CONSTANTS::entrypoint_npt_hook);

        DllParams* dll_params = NULL;

        /*  find the base of our allocation, where parameters are stored    */

        for (auto address = (uintptr_t)&PE::ResolveImports; address; address -= sizeof(uint32_t))
        {
            if (*(uint32_t*)address == INJECTOR_CONSTANTS::mapped_dll_header)
            {
                dll_params = (DllParams*)address;
                break;
            }
        }

        /* some DLLs such as api - ms dlls aren't globally mapped so  we have to do it here */

        PE::ResolveImports((uint8_t*)dll_params + 0x1000);

        BypassBattleye();
    }
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
