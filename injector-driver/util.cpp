#include "util.h"
#include "offsets.h"
#include "pe_header.h"

namespace Utils
{	
    void* GetDriverBaseAddress(size_t* out_driver_size, UNICODE_STRING driver_name)
    {
        auto moduleList = (PLIST_ENTRY)PsLoadedModuleList;

        UNICODE_STRING  DrvName;

        for (PLIST_ENTRY pListEntry = moduleList->Flink; pListEntry != moduleList; pListEntry = pListEntry->Flink)
        {
            // Search for Ntoskrnl entry
            auto entry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

            if (RtlCompareUnicodeString(&entry->FullDllName, &driver_name, TRUE))
            {
                DbgPrint("Found Module! %wZ \n", &entry->FullDllName);

                if (out_driver_size)
                {
                    *out_driver_size = entry->SizeOfImage;
                }

                return entry->DllBase;
            }
        }

        return 0;
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
                DbgPrint("found module! \n");
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
