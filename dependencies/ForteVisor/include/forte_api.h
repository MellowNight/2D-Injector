#pragma once
#include <cstdint>
#include <Windows.h>
#include <math.h>

enum VMMCALL_ID : uintptr_t
{
    disable_hv = 0x11111111,
    set_npt_hook = 0x11111112,
    remove_npt_hook = 0x11111113,
    is_hv_present = 0x11111114,
    sandbox_page = 0x11111116,
    register_sandbox = 0x11111117,
    deny_sandbox_reads = 0x11111118
};

struct GeneralRegisters
{
    uintptr_t  r15;
    uintptr_t  r14;
    uintptr_t  r13;
    uintptr_t  r12;
    uintptr_t  r11;
    uintptr_t  r10;
    uintptr_t  r9;
    uintptr_t  r8;
    uintptr_t  rdi;
    uintptr_t  rsi;
    uintptr_t  rbp;
    uintptr_t  rsp;
    uintptr_t  rbx;
    uintptr_t  rdx;
    uintptr_t  rcx;
    uintptr_t  rax;
};

extern "C" void (*sandbox_execute_handler)(GeneralRegisters * registers, void* return_address, void* o_guest_rip);
extern "C" void (*sandbox_mem_access_handler)(GeneralRegisters * registers, void* o_guest_rip);


extern "C" int __stdcall svm_vmmcall(VMMCALL_ID vmmcall_id, ...);
extern "C" void __stdcall execute_handler_wrap();
extern "C" void __stdcall rw_handler_wrap();

namespace ForteVisor
{
    enum NCR3_DIRECTORIES
    {
        primary,
        noexecute,
        sandbox,
        sandbox_single_step
    };

    enum SandboxHookId
    {
        readwrite_handler = 0,
        execute_handler = 1,
    };

    extern "C"  void SandboxMemAccessHandler(GeneralRegisters * registers, void* o_guest_rip);
    extern "C"  void SandboxExecuteHandler(GeneralRegisters * registers, void* return_address, void* o_guest_rip);

    int SetNptHook(uintptr_t address, uint8_t* patch, size_t patch_len, int32_t noexecute_cr3_id, uintptr_t tag);

    int SandboxPage(uintptr_t address, uintptr_t tag);

    void DenySandboxMemAccess(void* page_addr);

    void RegisterSandboxHandler(SandboxHookId handler_id, void* address);

    int RemoveNptHook(int32_t tag);

    int ForEachCore(void(*callback)(void* params), void* params = NULL);

    int DisableHv();
};