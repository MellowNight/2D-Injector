#include "communicate.h"
#include "undocumented_exports.h"

using namespace Interface;

void CommandHandler(PVOID context)
{
	auto request = (Msg*)context;

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
			auto msg = (StartThreadMsg*)request;

			DbgPrint("receieved request %i start thread ProcessID %i\n", msg_id, msg->proc_id);

			PEPROCESS process;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg->proc_id, &process);
			KeStackAttachProcess(process, &apc);
			
			auto dll_info = (InjectInfo*)msg->map_base;

			dll_info->dll_size = msg->image_size;
			dll_info->header = 0x1234;

			UNICODE_STRING user32_name = RTL_CONSTANT_STRING(L"user32.dll");
			auto user32 = Utils::GetUserModule(PsGetCurrentProcess(), &user32_name);

			// NPT hook on PeekMessageW
			auto original =

			*(void**)dll_info->original_bytes = original;

			KeUnstackDetachProcess(&apc);

			break;
		}
		case ALLOC_MEM:
		{
			auto msg = (AllocMemMsg*)request;

			DbgPrint("receieved request %i, process id %i size %i \n", msg_id, msg->proc_id, msg->size);

			PEPROCESS process;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg->proc_id, &process);
			KeStackAttachProcess(process, &apc);

			SIZE_T size = 512;

			auto status = ZwAllocateVirtualMemory(
				NtCurrentProcess(),
				(void**)&new_vmt,
				0,
				&size,
				MEM_COMMIT | MEM_RESERVE,
				PAGE_EXECUTE_READWRITE
			);
	
			KeUnstackDetachProcess(&apc);

			break;
		}
		case MODULE_BASE:
		{
			auto msg = (GetModuleMsg*)request;

			DbgPrint("receieved request %i module name %ws\n", msg_id, msg->module);

			PEPROCESS process;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg->proc_id, &process);
			KeStackAttachProcess(process, &apc);

			UNICODE_STRING mod_name;
			RtlInitUnicodeString(&mod_name, msg->module);
				
			auto mod_base = Utils::GetUserModule(process, &mod_name);

			KeUnstackDetachProcess(&apc);

			DbgPrint("mod_base %p \n", mod_base);

			SIZE_T copied;

			auto status = MmCopyVirtualMemory(PsGetCurrentProcess(), &mod_base, client,
				(void*)msg->out_buf, sizeof(uintptr_t), KernelMode, &copied);

			break;
		}
		case WRITE_MEM:
		{		
			auto msg = (WriteMsg*)request;

			DbgPrint("receieved request %i write size %i write buffer %p proc id %i write address %p\n", 
				msg_id, msg->size, msg->buffer, msg->proc_id, msg->address);

			Utils::WriteMem(msg->proc_id, msg->address, msg->buffer, msg->size);
			
			DbgPrint("wrote memory! status %p \n", status);
			break;
		}
		case INIT_COMM:
		{
			auto msg = (InitMsg*)request;
			PsLookupProcessByProcessId((HANDLE)msg->proc_id, &Interface::client);
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
	Interface::Init();

    return STATUS_SUCCESS;
}

NTSTATUS MapperEntry(uintptr_t driver_base, uintptr_t driver_size)
{
    return DriverEntry(driver_base, driver_size);
}