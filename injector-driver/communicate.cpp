#include "communicate.h"
#include "forte_api_kernel.h"
#include "hooking.h"

void CommandHandler(void* system_buffer, void* output_buffer);

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
        auto msg_buffer = (Msg*)InputBuffer;

        /*  original bytes are fucked   */
        auto status = static_cast<decltype(&NtDeviceIoControlFile_handler)>((void*)ioctl_hk.original_bytes)(
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

        if (IoControlCode != COMMAND_KEY)
        {
            return status;
        }

        if (msg_buffer->command_key == COMMAND_KEY)
        {
            DbgPrint("NtDeviceIoControlFile called from injector client! command_key %p \n", msg_buffer->command_key);

            CommandHandler(InputBuffer, OutputBuffer);
        }

        SpoofDisk(IoControlCode, InputBuffer, OutputBuffer);

        return status;
    }

	bool Init()
	{
        UNICODE_STRING NtDeviceIoControlFile_name = RTL_CONSTANT_STRING(L"NtDeviceIoControlFile");
        auto NtDeviceIoControl = MmGetSystemRoutineAddress(&NtDeviceIoControlFile_name);

        ioctl_hk = Hooks::JmpRipCode{ (uintptr_t)NtDeviceIoControl, (uintptr_t)NtDeviceIoControlFile_handler };

        ForteVisor::SetNptHook((uintptr_t)NtDeviceIoControl, (uint8_t*)ioctl_hk.hook_code, ioctl_hk.hook_size);

        char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

        for (auto i = 0, l = strlen(Spoofer::disk_serial); i < l; ++i)
        {
            Spoofer::disk_serial[i] = alphabet[RtlRandomEx(&SEED) % (sizeof(alphabet) - 1)];
        }



        return true;
	}
}
