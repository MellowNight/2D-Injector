#pragma once
#include    <ntifs.h>
#include    <Ntstrsafe.h>
#include    <intrin.h>
#include    <ntdef.h>
#include    <windef.h>
#include    <wdm.h>

extern "C"
{
    NTSTATUS NTAPI NtMapViewOfSection(
        HANDLE SectionHandle, 
        HANDLE ProcessHandle,
        PVOID* BaseAddress, 
        ULONG_PTR ZeroBits, 
        SIZE_T CommitSize, 
        PLARGE_INTEGER SectionOffset, 
        PSIZE_T ViewSize,
        DWORD InheritDisposition, 
        ULONG AllocationType, 
        ULONG Win32Protect
    );

    NTKERNELAPI  PPEB NTAPI PsGetProcessPeb(IN PEPROCESS Process);

    NTKERNELAPI PVOID NTAPI PsGetProcessWow64Process(
        _In_ PEPROCESS Process
    );

    NTSTATUS NTAPI NtSetInformationProcess(
        _In_ HANDLE ProcessHandle,
        _In_ PROCESSINFOCLASS ProcessInformationClass,
        _In_ PVOID ProcessInformation,
        _In_ ULONG ProcessInformationLength
    );

    NTSTATUS WINAPI ZwQuerySystemInformation(
        _In_      int SystemInformationClass,
        _Inout_   PVOID                    SystemInformation,
        _In_      ULONG                    SystemInformationLength,
        _Out_opt_ PULONG                   ReturnLength
    );

    NTSTATUS
        MmCopyVirtualMemory(
            IN  PEPROCESS FromProcess,
            IN  CONST VOID* FromAddress,
            IN  PEPROCESS ToProcess,
            OUT PVOID ToAddress,
            IN  SIZE_T BufferSize,
            IN  KPROCESSOR_MODE PreviousMode,
            OUT PSIZE_T NumberOfBytesCopied
        );


    PLIST_ENTRY NTKERNELAPI PsLoadedModuleList;

    NTSYSAPI PIMAGE_NT_HEADERS NTAPI RtlImageNtHeader(IN PVOID   ModuleAddress);

    NTSYSAPI
        NTSTATUS
        NTAPI
        ZwQueryInformationThread(
            IN HANDLE ThreadHandle,
            IN THREADINFOCLASS ThreadInformationClass,
            OUT PVOID ThreadInformation,
            IN ULONG ThreadInformationLength,
            OUT PULONG ReturnLength OPTIONAL
        );


    NTSTATUS NTAPI RtlCreateUserThread(
        IN HANDLE ProcessHandle,
        IN PSECURITY_DESCRIPTOR SecurityDescriptor OPTIONAL,
        IN BOOLEAN CreateSuspended,
        IN ULONG StackZeroBits OPTIONAL,
        IN SIZE_T StackReserve OPTIONAL,
        IN SIZE_T StackCommit OPTIONAL,
        IN PVOID StartAddress,
        IN PVOID Parameter OPTIONAL,
        PHANDLE ThreadHandle OPTIONAL,
        PCLIENT_ID ClientId OPTIONAL
    );

    const char* PsGetProcessImageFileName(PEPROCESS Process);

    NTKERNELAPI PVOID PsGetCurrentThreadStackBase();
}