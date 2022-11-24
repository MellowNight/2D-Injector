#pragma once
#include <cstdint>
#include <Windows.h>
#include <math.h>

enum VMMCALL_ID : uintptr_t
{
    set_mpk_hook = 0x22FFAA1166,
    disable_hv = 0xFFAA221166,
    set_npt_hook = 0x6611AAFF22,
    remove_npt_hook = 0x1166AAFF22,
    is_hv_present = 0xEEFF,
    remap_page_ncr3_specific = 0x8236FF,
    sandbox_page = 0x8236FE,
    register_sandbox = 0x8F36FE,
};

enum NCR3_DIRECTORIES
{
    primary,
    noexecute,
    sandbox,
};

struct GeneralRegisters
{
    UINT64  r15;
    UINT64  r14;
    UINT64  r13;
    UINT64  r12;
    UINT64  r11;
    UINT64  r10;
    UINT64  r9;
    UINT64  r8;
    UINT64  rdi;
    UINT64  rsi;
    UINT64  rbp;
    UINT64  rsp;
    UINT64  rbx;
    UINT64  rdx;
    UINT64  rcx;
    UINT64  rax;
};

extern "C" void (*sandbox_handler)(GeneralRegisters * registers, void* return_address, void* o_guest_rip);


extern "C" int __stdcall svm_vmmcall(VMMCALL_ID vmmcall_id, ...);
extern "C" int __stdcall sandbox_handler_wrap();

namespace ForteVisor
{
    extern "C"     void SandboxHandler(GeneralRegisters * registers, void* return_address, void* o_guest_rip);

    int RemapPageSingleNcr3(uintptr_t old_page, uintptr_t new_page, int32_t core_id);

    int SetNptHook(uintptr_t address, uint8_t* patch, size_t patch_len, int32_t noexecute_cr3_id, uintptr_t tag);

    int SandboxPage(uintptr_t address, uintptr_t tag);

    void RegisterSandboxHandler(decltype(sandbox_handler) address);

    int RemoveNptHook(int32_t tag);

    int ForEachCore(void(*callback)(void* params), void* params = NULL);

    int DisableHv();
};