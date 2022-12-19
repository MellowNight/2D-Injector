#pragma once
#include <cstdint>
#include <Windows.h>
#include <math.h>
#include <intrin.h>

enum VMMCALL_ID : uintptr_t
{
    disable_hv = 0x11111111,
    set_npt_hook = 0x11111112,
    remove_npt_hook = 0x11111113,
    is_hv_present = 0x11111114,
    sandbox_page = 0x11111116,
    register_instrumentation_hook = 0x11111117,
    deny_sandbox_reads = 0x11111118,
    start_branch_trace = 0x11111119,
};

#define PAGE_SIZE 0x1000

struct GuestRegisters
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

union BranchLog
{
    struct LogEntry
    {
        uintptr_t branch_address;
        uintptr_t branch_target;
    };

    struct
    {
        int capacity;
        int buffer_idx;
        LogEntry* buffer;
    } info;

    LogEntry log_entries[PAGE_SIZE / sizeof(LogEntry)];

    BranchLog()
    {
        info.capacity = ((PAGE_SIZE - sizeof(info)) / sizeof(LogEntry));
        info.buffer_idx = 0;
        info.buffer = &log_entries[1];
    }
};

static_assert(sizeof(BranchLog) == PAGE_SIZE, "Vmcbcontrol_area Size Mismatch");

extern "C" void (*sandbox_execute_handler)(GuestRegisters * registers, void* return_address, void* o_guest_rip);
extern "C" void __stdcall execute_handler_wrap();

extern "C" void (*sandbox_mem_access_handler)(GuestRegisters * registers, void* o_guest_rip);
extern "C" void __stdcall rw_handler_wrap();

extern "C" void (*branch_log_full_handler)();
extern "C" void __stdcall branch_log_full_handler_wrap();

extern "C" void (*branch_trace_finish_handler)();
extern "C" void __stdcall branch_trace_finish_handler_wrap();

extern "C" int __stdcall svm_vmmcall(VMMCALL_ID vmmcall_id, ...);


namespace BVM
{
    extern BranchLog* log_buffer;

    enum NCR3_DIRECTORIES
    {
        primary,
        noexecute,
        sandbox,
        sandbox_single_step
    };

    enum HookId
    {
        sandbox_readwrite = 0,
        sandbox_execute = 1,
        branch_log_full = 2,
        branch_trace_finished = 3,
        max_id
    };

    void TraceFunction(uint8_t* start_addr, uintptr_t range_base, uintptr_t range_size);

    int SetNptHook(uintptr_t address, uint8_t* patch, size_t patch_len, int32_t ncr3_id);

    int SandboxPage(uintptr_t address, uintptr_t tag);

    void SandboxRegion(uintptr_t base, uintptr_t size);

    void DenySandboxMemAccess(void* page_addr);

    void InstrumentationHook(HookId handler_id, void* address);

    int RemoveNptHook(uintptr_t address);

    int DisableHv();
};