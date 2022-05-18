#include "pch.h"
#include "be_bypass.h"
#include "utils.h"
#include "hooks.h"
#include "forte_api.h"
#include "inline_syscall.h"

#define BATTLEYE_NAME L"BEClient.dll"

Hooks::JmpRipCode* addveh_hook, *ntqvm_hook;

LONG BreakpointRemoverVEH(_EXCEPTION_POINTERS* ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
    {
        if (*(uint8_t*)ExceptionInfo->ContextRecord->Rip == 0xCC)
        {
            UNICODE_STRING mod_name;

            auto module_base = (uintptr_t)Utils::ModuleFromAddress(ExceptionInfo->ContextRecord->Rip, &mod_name);

            auto file_patch_offset = Utils::RvaToOffset((void*)module_base, (uintptr_t)ExceptionInfo->ContextRecord->Rip - module_base);

            //Utils::log("breakpoint hit! RIP = %p\n", ExceptionInfo->ContextRecord->Rip);

            auto file_handle = CreateFileW(
                mod_name.Buffer, GENERIC_ALL, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
            );
            
            auto rip_page = PAGE_ALIGN(ExceptionInfo->ContextRecord->Rip);
            
            DWORD size = 1;
            
            ULONG old_protect, old_protect2;
            
            auto status = do_syscall<NTSTATUS>(NtProtectVirtual, (HANDLE)-1, (PVOID*)&ExceptionInfo->ContextRecord->Rip, &size, PAGE_EXECUTE_READWRITE, &old_protect);
            
            /* Restore the byte from disk */
            DWORD bytes_read;
            
            SetFilePointer(
                file_handle, file_patch_offset,
                NULL, FILE_BEGIN
            );
            
            char byte;
            
            ReadFile(
                file_handle, (void*)&byte,
                size, &bytes_read, NULL
            );
            
            memcpy((void*)ExceptionInfo->ContextRecord->Rip, &byte, 1);
            
            do_syscall<NTSTATUS>(NtProtectVirtual, (HANDLE)-1, (PVOID*)&ExceptionInfo->ContextRecord->Rip, &size, old_protect, &old_protect2);
        }
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    else if (ExceptionInfo->ExceptionRecord->ExceptionCode == DBG_PRINTEXCEPTION_C)
    {
        // for OutputDebugString
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

uint64_t (__fastcall* RtlpAddVectoredHandler)(
    int a1,
    __int64 a2,
    unsigned int a3
) = NULL;

uint64_t RtlAddVectoredExceptionHandler_hook(int first, __int64 handler_addr, unsigned int a3)
{            
    auto retaddr = *(uintptr_t*)_AddressOfReturnAddress();

    UNICODE_STRING mod_name;
    auto module_base = Utils::ModuleFromAddress(retaddr, &mod_name);

    if (!module_base || !wcscmp(mod_name.Buffer, BATTLEYE_NAME))
    {
        if (!module_base)
        {
            Utils::log("RtlAddVectoredExceptionHandler was called from 0x%p \n", retaddr);
        }
        else
        {
            Utils::log("RtlAddVectoredExceptionHandler was called from %wZ+0x%p \n", mod_name, retaddr-(uintptr_t)module_base);
        }

        static_cast<decltype(RtlpAddVectoredHandler)>(addveh_hook->original_bytes)(
            first, 
            (uint64_t)BreakpointRemoverVEH,
            a3
        );

        return (uint64_t)BreakpointRemoverVEH;
    }
    else
    {
        return static_cast<decltype(RtlpAddVectoredHandler)>(addveh_hook->original_bytes)(
            first,
            (uint64_t)handler_addr,
            a3
        );
    }
}

void BypassBattleye()
{   
    Disasm::Init();

    auto ntdll = GetModuleHandle(L"ntdll.dll");

    RtlpAddVectoredHandler = (decltype(RtlpAddVectoredHandler))GetProcAddress(ntdll, "RtlpAddVectoredHandler");

    addveh_hook = new Hooks::JmpRipCode{ (uintptr_t)RtlpAddVectoredHandler, (uintptr_t)RtlAddVectoredExceptionHandler_hook };

    ForteVisor::SetNptHook((uintptr_t)RtlpAddVectoredHandler, addveh_hook->hook_code, addveh_hook->hook_size, 'VEH');
}