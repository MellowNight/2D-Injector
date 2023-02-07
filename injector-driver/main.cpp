#include "communicate.h"
#include "disassembly.h"
#include "kernel_structs.h"
#include "hooking.h"
#include "util.h"
#include "memory_hiding.h"

enum INJECTOR_CONSTANTS
{
	mapped_dll_header = 0x12345678,
};

using namespace Command;

void Command::Handler(Msg* system_buffer, void* output_buffer)
{
	auto msg_id = system_buffer->message_id;

	DbgPrint("msg_id %i \n", msg_id);

	switch (system_buffer->message_id)
	{
		case Command::hide_memory:
		{	
			auto msg = *(HideMemoryCmd*)system_buffer;

			DbgPrint("msg_id %i target_pid %i \n", msg_id, msg.target_pid);

			hiding_range_start = msg.address;
			hiding_range_size = msg.hiding_range_size;

			HANDLE hthread;

			PsCreateSystemThread(&hthread, THREAD_ALL_ACCESS,
				NULL, NULL, NULL, (PKSTART_ROUTINE)HookNTQVM, NULL);

			break;
		}
		case Command::protect_memory:
		{
			auto msg = *(ProtectMemory*)system_buffer;

			auto apc = Utils::AttachToProcess(msg.proc_id);

			DWORD old_protection = 0;

			SIZE_T size = msg.size;

			auto status = ZwProtectVirtualMemory(ZwCurrentProcess(), 
				(void**)&msg.address, &size, msg.memory_protection, &old_protection);

			KeUnstackDetachProcess(&apc);

			DbgPrint("msg_id %i msg.address 0x%p msg.size %p, msg.memory_protection %d status 0x%p \n",
				msg_id, msg.address, size, msg.memory_protection, status
			);

			break;
		}
		case Command::module_base:
		{
			auto msg = *(GetModuleMsg*)system_buffer;

			DbgPrint("receieved request %i module name %ws\n", msg_id, msg.module);

			auto apcstate = Utils::AttachToProcess(msg.proc_id);

			UNICODE_STRING module_name;

			RtlInitUnicodeString(&module_name, msg.module);
				
			auto module_base = 
				(void*)Utils::GetUserModule(IoGetCurrentProcess(), &module_name);

			KeUnstackDetachProcess(&apcstate);

			DbgPrint("retrieved module base %p \n", module_base);

			*(void**)output_buffer = module_base;

			break;
		}
		case Command::remote_npt_hook:
		{
			auto hook_cmd = *(NptHookMsg*)system_buffer;

			DbgPrint(
				"receieved request %i hook_cmd.shellcode 0x%p hook_cmd.size %i \n", hook_cmd.message_id, hook_cmd.shellcode, hook_cmd.size);

			auto apcstate = Utils::AttachToProcess(hook_cmd.proc_id);

			AetherVisor::NptHook::Set(
				hook_cmd.hook_address, hook_cmd.shellcode, hook_cmd.size);

			KeUnstackDetachProcess(&apcstate);
			
			break;
		}
		case Command::write_mem:
		{
			auto msg = (WriteCmd*)system_buffer;

			auto status = Utils::WriteMem(
				msg->proc_id, msg->address, msg->buffer, msg->size);

			break;
		}
		case Command::read_mem:
		{
			auto msg = (WriteCmd*)system_buffer;

			auto status = 
				Utils::ReadMem(msg->proc_id, msg->address, msg->buffer, msg->size);

			DbgPrint("receieved request %i target_pid %i msg.address %p msg->buffer %p \n", 
				msg_id, msg->proc_id, msg->address, msg->buffer);

			break;
		}
		case Command::process_id:
		{
			auto msg = *(GetProcessIdMsg*)system_buffer;

			UNICODE_STRING uni_name;

			ANSI_STRING	ansi_name;

			RtlInitUnicodeString(&uni_name, msg.process_name);

			RtlUnicodeStringToAnsiString(&ansi_name, &uni_name, TRUE);

			auto processid = (int)Utils::GetProcessId(ansi_name.Buffer);

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

	PsCreateSystemThread(&thread_handle, GENERIC_ALL,
		NULL, NULL, NULL, (PKSTART_ROUTINE)Command::Init, NULL
	);

    return STATUS_SUCCESS;
}

NTSTATUS MapperEntry(uintptr_t driver_base, uintptr_t driver_size)
{
    return DriverEntry(driver_base, driver_size);
}