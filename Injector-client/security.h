#pragma once
#include "includes.h"

#define INJECTOR_PASSWORD 0xFF917822

typedef NTSTATUS(NTAPI* pFnNtReadHardError)(
    NTSTATUS ErrorStatus,
    ULONG NumberOfParameters,
    ULONG UnicodeStringParameterMask,
    PULONG_PTR* Parameters,
    ULONG ValidResponseOption,
    PULONG Response
);

void CrashProgram(uint64_t rcx, uint64_t rdx)
{
    rcx += (0xD0) ^ ((0xEFi64 << (3 * rdx))) & 0x431D14225FD1645;
    rdx += (0x7D) ^ ((0x2a << (3 * rcx))) & 0x71211D8798fffa;

    *(__int64*)(rcx + rdx) = rcx - rdx;

    auto ntdll = GetModuleHandle(L"ntdll.dll");

    pFnNtReadHardError pfnNtRaiseHardError = (pFnNtReadHardError)GetProcAddress(ntdll, "NtRaiseHardError");
    ULONG r;
    auto status = pfnNtRaiseHardError(0x11111111, rdx, rcx, 0, 6, &r);
}

void SecurityViolation()
{
    MessageBoxA(NULL, "security violation detected", "SecurityViolation()", 0);
    return;

    srand(time(NULL));

    CrashProgram(rand(), rand() * 5);
}