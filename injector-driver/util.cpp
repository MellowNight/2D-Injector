#include "util.h"
#include "pe_header.h"

namespace Utils
{
    void* VirtualAddrFromPfn(uintptr_t pfn)
    {
        PHYSICAL_ADDRESS pa;
        pa.QuadPart = pfn << PAGE_SHIFT;

        return MmGetVirtualForPhysical(pa);
    }

    PT_ENTRY_64* GetPte(void* virtual_address, uintptr_t pml4_base_pa, int (*page_table_callback)(PT_ENTRY_64*))
    {
        ADDRESS_TRANSLATION_HELPER helper;

        PT_ENTRY_64* final_entry;

        helper.AsUInt64 = (uintptr_t)virtual_address;

        PHYSICAL_ADDRESS pml4_physical;
        pml4_physical.QuadPart = pml4_base_pa;

        PML4E_64* pml4;
        PML4E_64* pml4e;

        pml4 = (PML4E_64*)MmGetVirtualForPhysical(pml4_physical);

        pml4e = &pml4[helper.AsIndex.Pml4];

        if (page_table_callback)
        {
            page_table_callback((PT_ENTRY_64*)pml4e);
        }

        if (pml4e->Present == FALSE)
        {
            return (PT_ENTRY_64*)pml4e;
        }

        PDPTE_64* pdpt;
        PDPTE_64* pdpte;

        pdpt = (PDPTE_64*)Utils::VirtualAddrFromPfn(pml4e->PageFrameNumber);

        pdpte = &pdpt[helper.AsIndex.Pdpt];

        if (page_table_callback)
        {
            page_table_callback((PT_ENTRY_64*)pdpte);
        }

        if ((pdpte->Present == FALSE) || (pdpte->LargePage != FALSE))
        {
            return (PT_ENTRY_64*)pdpte;
        }

        PDE_64* pd;
        PDE_64* pde;

        pd = (PDE_64*)Utils::VirtualAddrFromPfn(pdpte->PageFrameNumber);

        pde = &pd[helper.AsIndex.Pd];

        if (page_table_callback)
        {
            page_table_callback((PT_ENTRY_64*)pde);
        }

        if ((pde->Present == FALSE) || pde->LargePage == TRUE)
        {
            return (PT_ENTRY_64*)pde;
        }


        PTE_64* pt;
        PTE_64* pte;


        pt = (PTE_64*)Utils::VirtualAddrFromPfn(pde->PageFrameNumber);

        pte = &pt[helper.AsIndex.Pt];

        if (page_table_callback)
        {
            page_table_callback((PT_ENTRY_64*)pte);
        }

        return  (PT_ENTRY_64*)pte;
    }

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

    NTSTATUS UnlockPages(PMDL mdl)
    {
        MmUnlockPages(mdl);
        IoFreeMdl(mdl);

        return STATUS_SUCCESS;
    }

    PVOID GetKernelModule(OUT PULONG pSize, UNICODE_STRING DriverName)
    {
        PLIST_ENTRY moduleList = (PLIST_ENTRY)PsLoadedModuleList;

        UNICODE_STRING  DrvName;

        for (PLIST_ENTRY link = moduleList;
            link != moduleList->Blink;
            link = link->Flink)
        {
            LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(link, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

            if (RtlCompareUnicodeString(&DriverName, &entry->BaseDllName, false) == 0)
            {
                DbgPrint("found module! %wZ at %p \n", &entry->BaseDllName, entry->DllBase);
                if (pSize && MmIsAddressValid(pSize))
                {
                    *pSize = entry->SizeOfImage;
                }

                return entry->DllBase;
            }
        }

        return 0;
    }

    KIRQL DisableWP()
    {
        KIRQL	tempirql = KeRaiseIrqlToDpcLevel();

        ULONG64  cr0 = __readcr0();

        cr0 &= 0xfffffffffffeffff;

        __writecr0(cr0);

        _disable();

        return tempirql;

    }

    void EnableWP(KIRQL	tempirql)
    {
        ULONG64	cr0 = __readcr0();

        cr0 |= 0x10000;

        _enable();

        __writecr0(cr0);

        KeLowerIrql(tempirql);
    }

    PVOID ReadFile(PVOID buffer, const wchar_t* FileName, ULONG64 size, HANDLE* hFile)
    {
        UNICODE_STRING     uniName;
        OBJECT_ATTRIBUTES  objAttr;
        IO_STATUS_BLOCK    ioStatusBlock;

        RtlInitUnicodeString(&uniName, FileName);

        InitializeObjectAttributes(
            &objAttr, &uniName,
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL, NULL
        );

        NTSTATUS status = STATUS_SUCCESS;

        LARGE_INTEGER FileSize = { size };

        status = ZwOpenFile(hFile,
            GENERIC_ALL, &objAttr,
            &ioStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            FILE_NON_DIRECTORY_FILE
        );
        DbgPrint("status %p \n", status);
        LARGE_INTEGER FileOffset = { 0 };

        auto status2 = ZwReadFile(*hFile, NULL, NULL, NULL, &ioStatusBlock, buffer, FileSize.QuadPart, &FileOffset, 0);
        DbgPrint("status2 %p \n", status2);

        return buffer;
    }

    PVOID GetUserModule(IN PEPROCESS pProcess, IN PUNICODE_STRING ModuleName)
    {
        PPEB pPeb = PsGetProcessPeb(pProcess);

        if (!pPeb)
        {
            return NULL;
        }

        // Still no loader
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

	int ZwGetRunningSystemProcess(LPWSTR ProcessName)
	{
		ULONG cbBuffer = 0x8000; //32k
		PVOID pSystemInfo;
		NTSTATUS status;
		PSYSTEM_PROCESS_INFORMATION pInfo;
		//Allocate enough space for the search process
		do
		{
			pSystemInfo = ExAllocatePool(NonPagedPool, cbBuffer);
			if (pSystemInfo == NULL) //Failed to apply for space, return
			{
				return 1;
			}
			status = ZwQuerySystemInformation(SystemProcessInformation, pSystemInfo, cbBuffer, NULL);
			if (status == STATUS_INFO_LENGTH_MISMATCH) //Insufficient space
			{
				ExFreePool(pSystemInfo);
				cbBuffer *= 2;
			}
			else if (!NT_SUCCESS(status))
			{
				ExFreePool(pSystemInfo);
				return 1;
			}
		} while (status == STATUS_INFO_LENGTH_MISMATCH); //If there is not enough space, keep looping
		pInfo = (PSYSTEM_PROCESS_INFORMATION)pSystemInfo; //Put the obtained information into pInfo
		for (;;)
		{
			LPWSTR pszProcessName = pInfo->ImageName.Buffer;
			if (pszProcessName == NULL)
			{
				pszProcessName = L"NULL";
			}

			if (wcscmp(pszProcessName, ProcessName) == 0)
			{
				//DbgMessage("PID:%d, process name:%S\n", pInfo->UniqueProcessId, pszProcessName);
				return (int)pInfo->UniqueProcessId;
			}

			if (pInfo->NextEntryOffset == 0) //== 0, indicating that the end of the process chain has been reached
			{
				break;
			}
			pInfo = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)pInfo) + pInfo->NextEntryOffset); //Traversal
		}
		return 0;
	}


    PVOID WriteFile(PVOID buffer, const wchar_t* FileName, ULONG64 size)
    {
        UNICODE_STRING     uniName;
        OBJECT_ATTRIBUTES  objAttr;
        IO_STATUS_BLOCK    ioStatusBlock;

        RtlInitUnicodeString(&uniName, FileName);

        InitializeObjectAttributes(
            &objAttr, &uniName,
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL, NULL
        );

        NTSTATUS status = STATUS_SUCCESS;
        HANDLE hFile;

        LARGE_INTEGER FileSize = { size };

        status = ZwOpenFile(&hFile,
            GENERIC_ALL, &objAttr,
            &ioStatusBlock, 
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            FILE_NON_DIRECTORY_FILE
        );
        DbgPrint("status %p \n", status);

        LARGE_INTEGER FileOffset = { 0 };

        auto status2 = ZwWriteFile(hFile, NULL, NULL, NULL, &ioStatusBlock, buffer, FileSize.QuadPart, &FileOffset, 0);
        DbgPrint("status2 %p \n", status2);

        return buffer;
    }

    PVOID CreateFile(PVOID buffer, const wchar_t* FileName, ULONG64 size)
    {
        UNICODE_STRING     uniName;
        OBJECT_ATTRIBUTES  objAttr;
        IO_STATUS_BLOCK    ioStatusBlock;

        RtlInitUnicodeString(&uniName, FileName);

        InitializeObjectAttributes(
            &objAttr, &uniName,
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL, NULL
        );

        NTSTATUS status = STATUS_SUCCESS;
        HANDLE hFile;

        LARGE_INTEGER FileSize = { size };

        status = ZwCreateFile(&hFile,
            GENERIC_ALL, &objAttr,
            &ioStatusBlock, &FileSize,
            FILE_ATTRIBUTE_NORMAL, FILE_SHARE_WRITE | FILE_SHARE_READ,
            FILE_SUPERSEDE, FILE_NON_DIRECTORY_FILE,
            NULL, 0
        );

        LARGE_INTEGER FileOffset = { 0 };

        auto status2 = ZwWriteFile(hFile, NULL, NULL, NULL, &ioStatusBlock, buffer, FileSize.QuadPart, &FileOffset, 0);

        return buffer;
    }

    KAPC_STATE AttachToProcess(int32_t pid)
    {
        PEPROCESS process;
        KAPC_STATE apc;

        PsLookupProcessByProcessId((HANDLE)pid, &process);
        KeStackAttachProcess(process, &apc);

        return apc;
    }
}
