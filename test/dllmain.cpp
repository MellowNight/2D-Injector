// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "dllmain.h"
#include "d3d_hook.h"
#include "utils.h"

Hooks::JmpRipCode* present_hk = NULL;

void StartCheat()
{
    /*  testing procedure:
        1. launch hypervisor
        2. launch test-anticheat
        3. attach debugger/open dbgview
        4. use CE to inject test bypass .dll
    */

    Disasm::Init();

    auto dxgi = GetModuleHandle(L"dxgi.dll");

    auto swapchain_present = (uintptr_t)dxgi + 0x2A40;

    present_hk = new Hooks::JmpRipCode{ (uintptr_t)swapchain_present, (uintptr_t)hk_present };

    ForteVisor::SetNptHook((uintptr_t)swapchain_present, present_hk->hook_code, present_hk->hook_size, 'DXGI');

    // ForteVisor::DisableHv();
}