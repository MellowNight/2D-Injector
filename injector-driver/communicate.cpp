#include "communicate.h"
#include "forte_api_kernel.h"
#include "hooking.h"

namespace Interface
{
    Hooks::JmpRipCode ioctl_hk;

    NTSTATUS NTAPI NtReadFile_handler(
            _In_ HANDLE FileHandle,
            _In_opt_ HANDLE Event,
            _In_opt_ PIO_APC_ROUTINE ApcRoutine,
            _In_opt_ PVOID ApcContext,
            _Out_ PIO_STATUS_BLOCK IoStatusBlock,
            _Out_writes_bytes_(Length) PVOID Buffer,
            _In_ ULONG Length,
            _In_opt_ PLARGE_INTEGER ByteOffset,
            _In_opt_ PULONG Key
        )
    {
        /*  original bytes are fucked   */
        return static_cast<decltype(&NtReadFile_handler)>((void*)ioctl_hk.original_bytes)(
            FileHandle,
            Event,
            ApcRoutine,
            ApcContext,
            IoStatusBlock,
            Buffer,
            Length,
            ByteOffset,
            Key
        );

        DbgPrint("FileHandle %p \n", FileHandle);
    }

	bool Init()
	{
        UNICODE_STRING NtReadFile_name = RTL_CONSTANT_STRING(L"NtReadFile");
        auto ReadFile = MmGetSystemRoutineAddress(&NtReadFile_name);
        
		ioctl_hk = Hooks::JmpRipCode{ (uintptr_t)ReadFile, (uintptr_t)NtReadFile_handler };


        DbgPrint("NtReadFile %p \n", ReadFile);
        DbgPrint("ioctl_hk.jmp back %p \n", ioctl_hk.original_bytes);
        __debugbreak();

        ForteVisor::SetNptHook((uintptr_t)ReadFile, (uint8_t*)ioctl_hk.hook_code, ioctl_hk.hook_size);
        __debugbreak();

        NtReadFile(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        return true;
	}
}
