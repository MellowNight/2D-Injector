#include "pch.h"
#include "be_bypass.h"
#include "utils.h"
#include "hooks.h"

#define BATTLEYE_NAME L"test-anticheat.exe"

LONG BreakpointRemoverVEH(_EXCEPTION_POINTERS* ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
    {
        if (*(BYTE*)ExceptionInfo->ContextRecord->Rip == 0xCC)
        {
            UNICODE_STRING mod_name;

            auto module_base = (uintptr_t)Utils::ModuleFromAddress(ExceptionInfo->ContextRecord->Rip, &mod_name);
            auto file_patch_offset = Utils::RvaToOffset((void*)module_base, (uintptr_t)ExceptionInfo->ContextRecord->Rip - module_base);

            Utils::log("breakpoint hit! RIP = %p\n", ExceptionInfo->ContextRecord->Rip);

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
                (void*)ExceptionInfo->ContextRecord->Rip, /* quick thinking  */
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

Hooks::JmpRipCode* addveh_hook = NULL;
uint64_t (__fastcall* RtlpAddVectoredHandler)(
    int a1,
    __int64 a2,
    unsigned int a3
) = NULL;

uint64_t RtlAddVectoredExceptionHandler_hook(int first, __int64 handler_addr, unsigned int a3)
{            
    UNICODE_STRING mod_name;
    auto module_base = Utils::ModuleFromAddress(*(uintptr_t*)_AddressOfReturnAddress(), &mod_name);

    if (!module_base || !wcscmp(mod_name.Buffer, BATTLEYE_NAME))
    {
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

    /*  we are going to register our own exception handler to replace their VEH    */

    auto ntdll = GetModuleHandle(L"ntdll.dll");
    
    RtlpAddVectoredHandler = (decltype(RtlpAddVectoredHandler))GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler");
    RtlpAddVectoredHandler = (decltype(RtlpAddVectoredHandler))RELATIVE_ADDR((uint8_t*)RtlpAddVectoredHandler + 3, 1, 5);

    addveh_hook = new Hooks::JmpRipCode{ (uintptr_t)RtlpAddVectoredHandler, (uintptr_t)RtlAddVectoredExceptionHandler_hook };

    ForteVisor::SetMpkHook(RtlpAddVectoredHandler, addveh_hook->hook_code, addveh_hook->hook_size);
}