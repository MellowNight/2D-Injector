#include "communicate.h"
#include "forte_api_kernel.h"
#include "hooking.h"

namespace Interface
{
    Hooks::JmpRipCode ioctl_hk;

    NTSTATUS NTAPI NtDeviceIoControlFile_handler(
        _In_ HANDLE FileHandle,
        _In_opt_ HANDLE Event,
        _In_opt_ PIO_APC_ROUTINE ApcRoutine,
        _In_opt_ PVOID ApcContext,
        _Out_ PIO_STATUS_BLOCK IoStatusBlock,
        _In_ ULONG IoControlCode,
        _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
        _In_ ULONG InputBufferLength,
        _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
        _In_ ULONG OutputBufferLength
    )
    {
        return static_cast<decltype(&NtDeviceIoControlFile_handler)>((void*)ioctl_hk.original_bytes)(
            FileHandle,
            Event,
            ApcRoutine,
            ApcContext,
            IoStatusBlock,
            IoControlCode,
            InputBuffer,
            InputBufferLength,
            OutputBuffer,
            OutputBufferLength
        );

        DbgPrint("FileHandle %p \n", FileHandle);
    }

	bool Init()
	{
		ioctl_hk = Hooks::JmpRipCode{ (uintptr_t)NtDeviceIoControlFile, (uintptr_t)NtDeviceIoControlFile_handler };

        ForteVisor::SetNptHook((uintptr_t)NtDeviceIoControlFile, ioctl_hk.hook_code, ioctl_hk.hook_size);

        return true;
	}
}
