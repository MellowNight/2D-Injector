#include "util.h"
#include "pe_header.h"

namespace Utils
{
    uint32_t Random()
    {
        LARGE_INTEGER time;
        KeQuerySystemTime(&time);

        auto seed = time.LowPart;

        return RtlRandomEx(&seed);
    }

    void SwapEndianess(PCHAR dest, PCHAR src)
	{
		for (size_t i = 0, l = strlen(src); i < l; i += 2) {
			dest[i] = src[i + 1];
			dest[i + 1] = src[i];
		}
	}
    
    uintptr_t FindPattern(uintptr_t region_base, size_t region_size, const char* pattern, size_t pattern_size, char wildcard)
    {
        for (auto byte = (char*)region_base;
            byte < (char*)region_base + region_size;
            ++byte)
        {
            bool found = true;

            for (char* pattern_byte = (char*)pattern, *begin = byte;
                pattern_byte < pattern + pattern_size;
                ++pattern_byte, ++begin)
            {
                if (*pattern_byte != *begin && *pattern_byte != wildcard)
                {
                    found = false;
                }
            }

            if (found)
            {
                return (uintptr_t)byte;
            }
        }

        return 0;
    }

    NTSTATUS WriteMem(int32_t target_pid, uintptr_t address, void* buffer, size_t size)
	{
		PEPROCESS target;
		PsLookupProcessByProcessId((HANDLE)target_pid, &target);

		size_t copied;

		auto status = MmCopyVirtualMemory(
			PsGetCurrentProcess(), buffer, target, (void*)address, size, KernelMode, &copied
		);

		return status;
	}

	NTSTATUS ReadMem(int32_t target_pid, uintptr_t address, void* buffer, size_t size)
	{
		PEPROCESS target;
		PsLookupProcessByProcessId((HANDLE)target_pid, &target);

		size_t copied;

		auto status = MmCopyVirtualMemory(
			target, (void*)address, PsGetCurrentProcess(), buffer, size, KernelMode, &copied
		);

		return status;
	}
    
    HANDLE GetProcessId(const char* process_name)
    {
        auto list_entry = (LIST_ENTRY*)(((uintptr_t)PsInitialSystemProcess) + OFFSET::ProcessLinksOffset);

        auto current_entry = list_entry->Flink;

        while (current_entry != list_entry && current_entry != NULL)
        {
            auto process = (PEPROCESS)((uintptr_t)current_entry - OFFSET::ProcessLinksOffset);

            if (!strcmp(PsGetProcessImageFileName(process), process_name))
            {
                return process;
            }

            current_entry = current_entry->Flink;
        }
    }

    void* GetKernelModule(size_t* out_size, UNICODE_STRING driver_name)
    {
        auto module_list = (PLIST_ENTRY)PsLoadedModuleList;

        for (auto link = module_list; link != module_list->Blink; link = link->Flink)
        {
            LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(link, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

            if (RtlCompareUnicodeString(
                &driver_name, &entry->BaseDllName, false) == 0)
            {
                // DbgPrint("found module! %wZ at %p \n", &entry->BaseDllName, entry->DllBase);

                if (out_size && MmIsAddressValid(out_size))
                {
                    *out_size = entry->SizeOfImage;
                }
                return entry->DllBase;
            }
        }
    }

    void* GetUserModule(PEPROCESS pProcess, PUNICODE_STRING ModuleName)
    {
        PPEB pPeb = PsGetProcessPeb(pProcess);

        if (!pPeb)
        {
            return NULL;
        }

        if (!pPeb->Ldr)
        {
            return NULL;
        }

        // Search in InLoadOrderModuleList

        for (PLIST_ENTRY pListEntry = pPeb->Ldr->InLoadOrderModuleList.Flink;
            pListEntry != &pPeb->Ldr->InLoadOrderModuleList;
            pListEntry = pListEntry->Flink)
        {
            LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

            if (RtlCompareUnicodeString(&pEntry->BaseDllName, ModuleName, TRUE) == 0)
            {
                DbgPrint("found process module %wZ \n", &pEntry->BaseDllName);

                return pEntry->DllBase;
            }
        }
        
        return NULL;
    }

    int32_t GetProcessIdFromName(const wchar_t* ProcessName)
	{
		uint32_t buffer_size = 0x8000; //32k

		void* system_info;

		NTSTATUS status;

		//Allocate enough space for the search process

		do
		{
			system_info = ExAllocatePool(NonPagedPool, buffer_size);

			if (system_info == NULL) //Failed to apply for space, return
			{
				return 1;
			}

			status = ZwQuerySystemInformation(SystemProcessInformation, system_info, buffer_size, NULL);

			if (status == STATUS_INFO_LENGTH_MISMATCH) //Insufficient space
			{
				ExFreePool(system_info);
				buffer_size *= 2;
			}

			else if (!NT_SUCCESS(status))
			{
				ExFreePool(system_info);
				return 1;
			}
		} 
        while (status == STATUS_INFO_LENGTH_MISMATCH); //If there is not enough space, keep looping

		auto proc_info = (PSYSTEM_PROCESS_INFORMATION)system_info; //Put the obtained information into pInfo

		for (;;)
		{
			wchar_t* process_name = proc_info->ImageName.Buffer;

			if (process_name == NULL)
			{
				process_name = L"NULL";
			}

			if (wcscmp(process_name, ProcessName) == 0)
			{
				return (int32_t)proc_info->UniqueProcessId;
			}

			if (proc_info->NextEntryOffset == 0) //== 0, indicating that the end of the process chain has been reached
			{
				break;
			}

			proc_info = (PSYSTEM_PROCESS_INFORMATION)((uint8_t*)proc_info + proc_info->NextEntryOffset); //Traversal
		}
		return 0;
	}

    void* WriteFile(void* buffer, const wchar_t* file_name, uint64_t size)
    {
        UNICODE_STRING unicode_name;

        RtlInitUnicodeString(&unicode_name, file_name);

        OBJECT_ATTRIBUTES  object_attrib;

        InitializeObjectAttributes(&object_attrib,
            &unicode_name, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

        HANDLE hfile;

        LARGE_INTEGER file_size = { size };

        IO_STATUS_BLOCK    iosb;

        auto status = ZwOpenFile(&hfile, GENERIC_ALL, 
            &object_attrib, &iosb, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_NON_DIRECTORY_FILE);

        DbgPrint("status %p \n", status);

        LARGE_INTEGER file_offset = { 0 };

        auto status2 = ZwWriteFile(hfile, 
            NULL, NULL, NULL, &iosb, buffer, file_size.QuadPart, &file_offset, 0);

        DbgPrint("status2 %p \n", status2);

        return buffer;
    }

    KAPC_STATE AttachToProcess(int32_t pid)
    {
        PEPROCESS process;

        PsLookupProcessByProcessId((HANDLE)pid, &process);

        KAPC_STATE apc;

        KeStackAttachProcess(process, &apc);

        return apc;
    }
}
