#include "pch.h"
#include "be_bypass.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define BATTLEYE_NAME L"test-anticheat.exe"

uint64_t* (__fastcall* RtlpAddVectoredHandler)(int a1, __int64 a2, unsigned int a3);

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

            //auto module_base = (IMAGE_DOS_HEADER*)utils::ModuleFromAddress(ExceptionInfo->ContextRecord->Rip, &mod_name);
            //auto file_patch_offset = RVA_TO_OFFSET(module_base, (uintptr_t)ExceptionInfo->ContextRecord->Rip - module_base);

            //utils::log("breakpoint hit! RIP = %p\n", ExceptionInfo->ContextRecord->Rip);

            //auto file_handle = CreateFileW(
            //    mod_name.Buffer, GENERIC_ALL, 0, NULL,
            //    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
            //);

            //SetFilePointer(
            //    file_handle,
            //    file_patch_offset,
            //    NULL,
            //    FILE_BEGIN
            //);

            //auto rip_page = PAGE_ALIGN(ExceptionInfo->ContextRecord->Rip);
            //DWORD size = 1;
            //ULONG old_protect, old_protect2;

            //auto status = ZwProtectVirtualMemory((HANDLE)-1, (PVOID*)&rip_page, &size, PAGE_EXECUTE_READWRITE, &old_protect);

            //DWORD bytes_read;

            //BOOL ReadFile(
            //    file_handle,
            //    ExceptionInfo->ContextRecord->Rip, /* quick thinking  */
            //    1, &bytes_read, NULL
            //);

            //ZwProtectVirtualMemory((HANDLE)-1, (PVOID*)&rip_page, &size, old_protect, &old_protect2);
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
    auto ret_addr = *(uintptr_t*)_AddressOfReturnAddress();

    UNICODE_STRING dll_name;

    bool result = utils::ModuleFromAddress(ret_addr, &dll_name);

    if (!result || wcsstr(dll_name.Buffer, BATTLEYE_NAME))
    {
        utils::log("anticheat tried to set a VEH at %p \n", handler_addr);
        // BE tried to register a handler :flushed: :brutal:
        return (uint64_t)my_vector_handle;
    }
    else
    {
        utils::log("normal VEH at %p \n", handler_addr);
        return (uint64_t)RtlpAddVectoredHandler(first, handler_addr, 0);
    }
}

void BypassBattleye()
{
    /*  we are going to register our own exception handler, then  hook RtlAddVectoredExceptionHandler to block their VEH    */

    auto ntdll = GetModuleHandle(L"ntdll.dll");
    auto RtlAddVectoredExceptionHandler = GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler");
    auto page_base = PAGE_ALIGN(RtlAddVectoredExceptionHandler);

    utils::log("GetProcAddress returned %p, getlasterror 0x%p \n", 
        RtlAddVectoredExceptionHandler, GetLastError());

    my_vector_handle = AddVectoredExceptionHandler(true, BypassBEBreakpoint);
    
    ULONG size = 16;
    ULONG old_protect = NULL;
    ULONG old_protect2 = NULL;

    auto status = ZwProtectVirtualMemory((HANDLE)-1, (PVOID*)&page_base, &size, PAGE_EXECUTE_READWRITE, &old_protect);

    utils::log("zwprotect status %p \n", status);

    //    mov r9, 0xffffffffffff
    //    push r9
    //    ret
    char jmp_shellcode[] = { 0x49, 0xB9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x41, 0x51, 0xC3 };
    *(PVOID*)(jmp_shellcode + 2) = RtlAddVectoredExceptionHandler_hook;
    memcpy(RtlAddVectoredExceptionHandler, jmp_shellcode, 13);

    utils::log("RtlAddVectoredExceptionHandler address %p, first bytes are %p \n",
        RtlAddVectoredExceptionHandler, *(WORD*)RtlAddVectoredExceptionHandler);

    ZwProtectVirtualMemory((HANDLE)-1, (PVOID*)&page_base, &size, old_protect, &old_protect2);

    RtlpAddVectoredHandler = decltype(RtlpAddVectoredHandler)(RELATIVE_ADDR(
        (uint64_t)RtlAddVectoredExceptionHandler + 3,
        1, 5
    ));
}