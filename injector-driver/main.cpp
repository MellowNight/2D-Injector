#include "communicate.h"
#include "disassembly.h"
#include "kernel_structs.h"
#include "hooking.h"
#include "forte_api_kernel.h"
#include "util.h"
#include "offsets.h"

struct DllParams
{
	uint32_t header;
	size_t dll_size;
};

using namespace Interface;

void CommandHandler(void* system_buffer, void* output_buffer)
{
	auto request = (Msg*)system_buffer;

	auto msg_id = request->message_id;

	DbgPrint("msg_id %i \n", msg_id);

	switch (request->message_id)
	{
		case Interface::EXIT_CLEANUP:
		{
			DbgPrint("Exit request \n");
			return;
		}
		case Interface::START_THREAD:
		{
			auto msg = *(InvokeRemoteFunctionCmd*)request;

			DbgPrint("receieved request %i start thread ProcessID %i msg.map_base %p \n", msg_id, msg.proc_id, msg.map_base);

			PEPROCESS process;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg.proc_id, &process);
			KeStackAttachProcess(process, &apc);
			
			auto dll_info = (DllParams*)msg.map_base;

			dll_info->dll_size = msg.image_size;
			dll_info->header = 0x12345678;

			UNICODE_STRING d3d11_name = RTL_CONSTANT_STRING(L"dxgi.dll");

			auto dxgi = (uintptr_t)Utils::GetUserModule(PsGetCurrentProcess(), &d3d11_name);

			auto present = (uintptr_t)dxgi + DXGI_OFFSET::swapchain_present;

			auto present_hk = Hooks::JmpRipCode{ (uintptr_t)present, (uintptr_t)msg.address };

			// NPT hook on dxgi.dll!CDXGISwapChain::Present
			ForteVisor::SetNptHook(present, present_hk.hook_code, present_hk.hook_size);

			auto irql = Utils::DisableWP();

			Utils::EnableWP(irql);

			KeUnstackDetachProcess(&apc);

			break;
		}
		case ALLOC_MEM:
		{
			auto msg = *(AllocMemCmd*)request;

			DbgPrint("receieved request %i, process id %i size 0x%p \n", msg_id, msg.proc_id, msg.size);

			uintptr_t address = NULL;

			PEPROCESS process;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg.proc_id, &process);
			KeStackAttachProcess(process, &apc);

			auto status = ZwAllocateVirtualMemory(
				NtCurrentProcess(),
				(void**)&address,
				0,
				(size_t*)&msg.size,
				MEM_COMMIT | MEM_RESERVE,
				PAGE_EXECUTE_READWRITE
			);

			memset((void*)address, 0x00, msg.size);
	
			KeUnstackDetachProcess(&apc);

			*(uintptr_t*)output_buffer = address;

			break;
		}
		case MODULE_BASE:
		{
			auto msg = *(GetModuleMsg*)request;

			DbgPrint("receieved request %i module name %ws\n", msg_id, msg.module);

			PEPROCESS process;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg.proc_id, &process);
			KeStackAttachProcess(process, &apc);

			UNICODE_STRING mod_name;
			RtlInitUnicodeString(&mod_name, msg.module);
				
			auto module_base = (void*)Utils::GetUserModule(process, &mod_name);

			KeUnstackDetachProcess(&apc);

			DbgPrint("retrieved module base %p \n", module_base);

			*(void**)output_buffer = module_base;

			break;
		}
		case WRITE_MEM:
		{		
			auto msg = (WriteCmd*)request;

			DbgPrint("receieved request %i write size %i write buffer %p proc id %i write address %p\n", 
				msg_id, msg->size, msg->buffer, msg->proc_id, msg->address);

			auto status = Utils::WriteMem(msg->proc_id, msg->address, msg->buffer, msg->size);
			
			DbgPrint("wrote memory! status %p \n", status);
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