#include "communicate.h"
#include "shellcode_invoke.h"
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

			NTSTATUS status;

			if (strcmp(PsGetProcessImageFileName(process), "rainbowsix.exe") == NULL)
			{
				UNICODE_STRING module_name = RTL_CONSTANT_STRING(L"RainbowSix.exe");

				auto module = utils::GetUserModule(process, &module_name);

				PVOID o_present;
				SetR6PresentHook((uintptr_t)module, (PVOID)msg->address, &o_present);
			}
			else
			{
				HANDLE thread_handle;
				status = RtlCreateUserThread(
					(HANDLE)-1, 0, 0, 0, 0, 0, (PVOID)msg->address, NULL, &thread_handle, 0
				);
			}

			KeUnstackDetachProcess(&apc);

			DbgPrint("RtlCreateUserThread status %p address %p \n", status, msg->address);

			break;
		}
		case ALLOC_MEM:
		{
			auto msg = (AllocMemMsg*)request;

			NTSTATUS status;

			DbgPrint("receieved request %i, process id %i size %i \n", msg_id, msg->proc_id, msg->size);

			PEPROCESS process;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg->proc_id, &process);
			KeStackAttachProcess(process, &apc);

			HANDLE section_handle;
			OBJECT_ATTRIBUTES attrs;
			UNICODE_STRING     uniName;

			RtlInitUnicodeString(&uniName, L"\\BaseNamedObjects\\r6iinternal");
			InitializeObjectAttributes(&attrs, &uniName, OBJ_CASE_INSENSITIVE, NULL, NULL);
			
			status = ZwOpenSection(&section_handle , FILE_SHARE_READ, &attrs);

			SIZE_T map_size = msg->size;
			PVOID mapped_base = NULL;
			LARGE_INTEGER section_offset ;

			status = ZwMapViewOfSection(section_handle, NtCurrentProcess(), &mapped_base, 0,
				map_size, NULL, &map_size, ViewShare, 0, PAGE_EXECUTE_READWRITE | PAGE_NOCACHE);

			DbgPrint("ZwMapViewOfSection status %p mapped_base valid: %i \n", status, MmIsAddressValid(mapped_base));

			CR3 cr3;
			cr3.Flags = __readcr3();

			for (uintptr_t page = (uintptr_t)mapped_base;
				page < (uintptr_t)mapped_base + map_size;
				page += 0x1000)
			{
				/*	We need to do this because the PTE changes
					are only applied when the pages are mapped in
				*/

				utils::LockPages((void*)page, IoModifyAccess);

				/*	set every PTE, PML4E, PDPTE, and PDE for our memory to enable execute	*/

				utils::GetPte((PVOID)page, cr3.AddressOfPageDirectory << PAGE_SHIFT,
					[](PT_ENTRY_64* pte) -> int {
						pte->ExecuteDisable = 0;
						return 0;
					}
				);
			}

			/*	clear PE header	*/
			memset(mapped_base, 0xCC, 0x1000);
			
			KeUnstackDetachProcess(&apc);

			SIZE_T bytes;

			status = MmCopyVirtualMemory(PsGetCurrentProcess(), &mapped_base, client,
				(void*)msg->result, sizeof(uintptr_t), KernelMode, &bytes);

			DbgPrint("number of bytes copied %i  status %p \n", bytes, status);

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
				
			auto mod_base = utils::GetUserModule(process, &mod_name);

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

			PEPROCESS target;
			KAPC_STATE apc;

			PsLookupProcessByProcessId((HANDLE)msg->proc_id, &target);

			SIZE_T	copied;
			auto status = MmCopyVirtualMemory(client, msg->buffer, target,
				(void*)msg->address, msg->size, KernelMode, &copied);

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



NTSTATUS DriverEntry()
{
	// RemoveFromBigPool()
	Interface::Init();

    return STATUS_SUCCESS;
}

NTSTATUS MapperEntry()
{
    return DriverEntry();
}