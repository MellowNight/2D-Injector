#include "pch.h"
#include "be_bypass.h"
#include "utils.h"
#include "hooks.h"
#include "hv_api.h"
#include <stdio.h>
#include <stdlib.h>

#define BATTLEYE_NAME L"test-anticheat.exe"

LONG BreakpointRemoverVEH(_EXCEPTION_POINTERS* ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
    {
        if (*(BYTE*)ExceptionInfo->ContextRecord->Rip == 0xCC)
        {
            UNICODE_STRING mod_name;

            auto module_base = (IMAGE_DOS_HEADER*)utils::ModuleFromAddress(ExceptionInfo->ContextRecord->Rip, &mod_name);
            auto file_patch_offset = RVA_TO_OFFSET(module_base, (uintptr_t)ExceptionInfo->ContextRecord->Rip - module_base);

            utils::log("breakpoint hit! RIP = %p\n", ExceptionInfo->ContextRecord->Rip);

            auto file_handle = CreateFileW(
                mod_name.Buffer, GENERIC_ALL, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
            );

            auto rip_page = PAGE_ALIGN(ExceptionInfo->ContextRecord->Rip);
            DWORD size = 1;
            ULONG old_protect, old_protect2;

            auto status = ZwProtectVirtualMemory((HANDLE)-1, (PVOID*)&rip_page, &size, PAGE_EXECUTE_READWRITE, &old_protect);

            DWORD bytes_read;

            SetFilePointer(
                file_handle, file_patch_offset,
                NULL, FILE_BEGIN
            );

            ReadFile(
                file_handle,
                ExceptionInfo->ContextRecord->Rip, /* quick thinking  */
                1, &bytes_read, NULL
            );

            ZwProtectVirtualMemory((HANDLE)-1, (PVOID*)&rip_page, &size, old_protect, &old_protect2);
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

JmpRipCode* addveh_hook = NULL;

uint64_t RtlAddVectoredExceptionHandler_hook(int first, __int64 handler_addr)
{            
    UNICODE_STRING mod_name;
    auto module_base = utils::ModuleFromAddress(ExceptionInfo->ContextRecord->Rip, &mod_name);

    if (!module_base || wcscmp(mod_name.Buffer, BATTLEYE_NAME))
    {
        (decltype(RtlAddVectoredExceptionHandler_hook))(addveh_hook->original_bytes)(
            first, 
            BreakpointRemoverVEH
        );

        return (uint64_t)BreakpointRemoverVEH;
    }
    else
    {
        return (decltype(RtlAddVectoredExceptionHandler_hook))(addveh_hook->original_bytes)(
            first, 
            handler_addr
        );
    }
}

void BypassBattleye()
{
    /*  we are going to register our own exception handler, then  hook RtlAddVectoredExceptionHandler to block their VEH    */

    auto ntdll = GetModuleHandle(L"ntdll.dll");
    
    uint64_t*(__fastcall* RtlpAddVectoredHandler)(
        int a1, 
        __int64 a2, 
        unsigned int a3
    ) = (decltype(RtlpAddVectoredHandler))GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler");

    addveh_hook = new JmpRipCode{RtlpAddVectoredHandler, RtlAddVectoredExceptionHandler_hook};

    MatrixVisor::
}