#pragma once
#include <cstdint>

#define PAGE_SIZE 0x1000

enum VMMCALL_ID : uintptr_t
{
    disable_hv = 0x11111111,
    set_npt_hook = 0x11111112,
    remove_npt_hook = 0x11111113,
    is_hv_present = 0x11111114,
    sandbox_page = 0x11111116,
    instrumentation_hook = 0x11111117,
    deny_sandbox_reads = 0x11111118,
    start_branch_trace = 0x11111119,
    hook_efer_syscall = 0x1111111A,
};

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

extern "C" {

    extern void (*sandbox_execute_handler)(GuestRegisters* registers, void* return_address, void* o_guest_rip);
    void __stdcall execute_handler_wrap();

    extern void (*sandbox_mem_access_handler)(GuestRegisters* registers, void* o_guest_rip);
    void __stdcall rw_handler_wrap();

    extern void (*branch_log_full_handler)();
    void __stdcall branch_log_full_handler_wrap();

    extern void (*branch_trace_finish_handler)();
    void __stdcall branch_trace_finish_handler_wrap();

    extern void (*syscall_callback)();
    void __stdcall syscall_callback_wrap();

    int __stdcall svm_vmmcall(VMMCALL_ID vmmcall_id, ...);
}

namespace AetherVisor
{
    enum NCR3_DIRECTORIES
    {
        primary,
        shadow,
        sandbox,
        sandbox_single_step
    };

    enum CALLBACK_ID
    {
        sandbox_readwrite = 0,
        sandbox_execute = 1,
        branch_log_full = 2,
        branch_trace_finished = 3,
        syscall = 4,
        max_id
    };

    namespace NptHook
    {
        int Set(
            uintptr_t address,
            uint8_t* patch,
            size_t patch_len,
            NCR3_DIRECTORIES ncr3_id = NCR3_DIRECTORIES::primary,
            bool global_page = false
        );

        int Remove(uintptr_t address);
    }

    namespace BranchTracer
    {
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

            void Init();    // for kernel driver ExAllocatePool
            // BranchLog() for usermode CPP
        };

        extern BranchLog* log_buffer;

        void Trace(uint8_t* start_addr, uintptr_t range_base, uintptr_t range_size, uint8_t* stop_addr = NULL);
    }

    namespace SyscallHook
    {
        int HookEFER();
    }

    namespace Sandbox
    {
        void DenyPageAccess(void* page_addr, bool allow_reads);

        void DenyRegionAccess(void* base, size_t range, bool allow_reads);

        int SandboxPage(uintptr_t address, uintptr_t tag);

        void SandboxRegion(uintptr_t base, uintptr_t size);
    }

    void SetCallback(
        CALLBACK_ID handler_id, 
        void* address
    );

    int StopHv();
};