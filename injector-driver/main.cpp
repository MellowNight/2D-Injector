#include "communicate.h"
#include "disassembly.h"
#include "kernel_structs.h"
#include "hooking.h"
#include "forte_api_kernel.h"
#include "util.h"
#include "memory_hiding.h"

struct DllParams
{
	uint32_t header;
	size_t dll_size;
	uintptr_t swapchain_present_address;
	uintptr_t original_function_address;
	uintptr_t module_base;
	uint8_t original_present_bytes[20];
};

enum INJECTOR_CONSTANTS
{
	mapped_dll_header = 0x12345678,
	entrypoint_npt_hook = 0xAAAA
};

using namespace Interface;

void CommandHandler(void* system_buffer, void* output_buffer)
{
	auto request = (Msg*)system_buffer;

	auto msg_id = request->message_id;

	DbgPrint("msg_id %i \n", msg_id);

	switch (request->message_id)
	{
		case Interface::HIDE_MEMORY:
		{	
			auto msg = *(HideMemoryCmd*)system_buffer;

			DbgPrint("msg_id %i target_pid %i \n", msg_id, msg.target_pid);

			hiding_range_start = msg.address;
			hiding_range_size = msg.hiding_range_size;
			target_pid = msg.target_pid;
			protect_value = msg.spoofed_protection;

			HANDLE hthread;
			PsCreateSystemThread(&hthread, THREAD_ALL_ACCESS, NULL, NULL, NULL, (PKSTART_ROUTINE)HookMemoryProtectionFn, NULL);
			
			break;
		}
		case Interface::PROTECT_MEMORY:
		{
			auto msg = *(ProtectMemoryCmd*)system_buffer;

			auto apc = Utils::AttachToProcess(msg.proc_id);

			DWORD old_protection = 0;
			SIZE_T size = msg.size;

			auto status = ZwProtectVirtualMemory(
				ZwCurrentProcess(),
				(void**)&msg.address,
				&size,
				msg.memory_protection,
				&old_protection
			);

			KeUnstackDetachProcess(&apc);

			DbgPrint("msg_id %i msg.address 0x%p msg.size %p, msg.memory_protection %d status 0x%p \n", msg_id, msg.address, size, msg.memory_protection, status);

			break;
		}
		case Interface::START_THREAD:
		{
			auto msg = *(InvokeRemoteFunctionCmd*)request;

			DbgPrint("receieved request %i start thread ProcessID %i msg.map_base %p \n", msg_id, msg.proc_id, msg.map_base);

			auto apc = Utils::AttachToProcess(msg.proc_id);

			UNICODE_STRING dxgi_name = RTL_CONSTANT_STRING(L"dxgi.dll");

			auto dxgi = (uintptr_t)Utils::GetUserModule(PsGetCurrentProcess(), &dxgi_name);

			auto present_address = Utils::FindPattern(
				dxgi, PeHeader(dxgi)->OptionalHeader.SizeOfImage,
				"\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x60", 35, 0x00
			);

			auto present_hk = Hooks::JmpRipCode{ present_address, msg.address };

			auto dll_params = (DllParams*)msg.map_base;


			memcpy((void*)dll_params->original_present_bytes, present_hk.original_bytes, present_hk.orig_bytes_size);
			dll_params->dll_size = msg.image_size;
			dll_params->header = mapped_dll_header;
			dll_params->swapchain_present_address = present_address;

			//// NPT hook on dxgi.dll!CDXGISwapChain::Present
			ForteVisor::SetNptHook(present_address, present_hk.hook_code, present_hk.hook_size, entrypoint_npt_hook);

			KeUnstackDetachProcess(&apc);

			break;
		}
		case Interface::ALLOC_MEM:
		{
			auto msg = *(AllocMemCmd*)request;

			DbgPrint("receieved request %i, process id %i size 0x%p \n", msg_id, msg.proc_id, msg.size);

			uintptr_t address = NULL;

			auto apc = Utils::AttachToProcess(msg.proc_id);

			auto status = ZwAllocateVirtualMemory(
				NtCurrentProcess(),
				(void**)&address,
				0,
				(size_t*)&msg.size,
				MEM_COMMIT | MEM_RESERVE,
				PAGE_READWRITE
			);

			memset((void*)address, 0x00, msg.size);
	
			KeUnstackDetachProcess(&apc);

			*(uintptr_t*)output_buffer = address;

			break;
		}
		case Interface::MODULE_BASE:
		{
			auto msg = *(GetModuleMsg*)request;

			DbgPrint("receieved request %i module name %ws\n", msg_id, msg.module);

			auto apcstate = Utils::AttachToProcess(msg.proc_id);

			UNICODE_STRING mod_name;
			RtlInitUnicodeString(&mod_name, msg.module);
				
			auto module_base = (void*)Utils::GetUserModule(IoGetCurrentProcess(), &mod_name);

			KeUnstackDetachProcess(&apcstate);

			DbgPrint("retrieved module base %p \n", module_base);

			*(void**)output_buffer = module_base;

			break;
		}
		case Interface::SET_NPT_HOOK:
		{
			auto hook_cmd = *(NptHookMsg*)request;

			DbgPrint("receieved request %i hook_cmd.shellcode 0x%p hook_cmd.size %i \n", hook_cmd.message_id, hook_cmd.shellcode, hook_cmd.size);

			auto apcstate = Utils::AttachToProcess(hook_cmd.proc_id);

			ForteVisor::SetNptHook(hook_cmd.hook_address, hook_cmd.shellcode, hook_cmd.size, NULL);

			KeUnstackDetachProcess(&apcstate);

			break;
		}
		case Interface::WRITE_MEM:
		{
			auto msg = (WriteCmd*)request;

			auto status = Utils::WriteMem(msg->proc_id, msg->address, msg->buffer, msg->size);

			break;
		}
		case Interface::READ_MEM:
		{
			auto msg = (ReadCmd*)request;

			auto status = Utils::ReadMem(msg->proc_id, msg->address, msg->buffer, msg->size);
			DbgPrint("receieved request %i target_pid %i msg.address %p msg->buffer %p \n", msg_id, msg->proc_id, msg->address, msg->buffer);

			break;
		}
		case Interface::PROCESS_ID:
		{
			auto msg = *(GetProcessIdMsg*)request;

			int processid = Utils::ZwGetRunningSystemProcess(msg.process_name);

			*(int*)output_buffer = processid;

			break;
		}
		default:
		{
			break;
		}
	}
}

NTSTATUS DriverEntry(uintptr_t driver_base, uintptr_t driver_size)
{
	DbgPrint("hello, driver_base %p, driver_size %p \n", driver_base, driver_size);

	Disasm::Init();
	HANDLE thread_handle;

	PsCreateSystemThread(
		&thread_handle,
		GENERIC_ALL, NULL, NULL, NULL,
		(PKSTART_ROUTINE)Interface::Init,
		NULL
	);

    return STATUS_SUCCESS;
}

NTSTATUS MapperEntry(uintptr_t driver_base, uintptr_t driver_size)
{
    return DriverEntry(driver_base, driver_size);
}