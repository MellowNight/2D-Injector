#include "pch.h"
#include "be_bypass.h"
#include "utils.h"
#include "hooks.h"
#include "hv_api.h"
#include <stdio.h>
#include <stdlib.h>

#define BATTLEYE_NAME L"test-anticheat.exe"

PVOID my_vector_handle = NULL;

LONG BypassBEBreakpoint(
    _EXCEPTION_POINTERS* ExceptionInfo
)
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

            SetFilePointer(
                file_handle,
                file_patch_offset,
                NULL,
                FILE_BEGIN
            );

            auto rip_page = PAGE_ALIGN(ExceptionInfo->ContextRecord);
            DWORD size = 1;
            ULONG old_protect, old_protect2;

            auto status = ZwProtectVirtualMemory((HANDLE)-1, (PVOID*)&rip_page, &size, PAGE_EXECUTE_READWRITE, &old_protect);

            DWORD bytes_read;

            BOOL ReadFile(
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


uint64_t __fastcall RtlAddVectoredExceptionHandler_hook(int first, __int64 handler_addr)
{

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

    char* original_bytes;
    PlaceJmpRipHook(RtlpAddVectoredHandler, RtlAddVectoredExceptionHandler_hook, original_bytes);  
}