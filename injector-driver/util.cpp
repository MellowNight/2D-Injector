#include "util.h"
#include "offsets.h"

namespace utils
{
    PVOID	GetVaFromPfn(ULONG64 pfn)
    {
        PHYSICAL_ADDRESS pa;
        pa.QuadPart = pfn << PAGE_SHIFT;

        return MmGetVirtualForPhysical(pa);
    }

    PT_ENTRY_64* GetPte(PVOID VirtualAddress, ULONG64 Pml4BasePa, PageTableOperation Operation)
    {
        ADDRESS_TRANSLATION_HELPER helper;
        PT_ENTRY_64* finalEntry;


        helper.AsUInt64 = (UINT64)VirtualAddress;

        PHYSICAL_ADDRESS    addr;

        addr.QuadPart = Pml4BasePa;

        PML4E_64* pml4;
        PML4E_64* pml4e;

        pml4 = (PML4E_64*)MmGetVirtualForPhysical(addr);

        pml4e = &pml4[helper.AsIndex.Pml4];

        if (Operation)
        {
            Operation((PT_ENTRY_64*)pml4e);
        }

        if (pml4e->Present == FALSE)
        {
            return (PT_ENTRY_64*)pml4e;
        }

        PDPTE_64* pdpt;
        PDPTE_64* pdpte;

        pdpt = (PDPTE_64*)GetVaFromPfn(pml4e->PageFrameNumber);

        pdpte = &pdpt[helper.AsIndex.Pdpt];

        if (Operation)
        {
            Operation((PT_ENTRY_64*)pdpte);
        }

        if ((pdpte->Present == FALSE) || (pdpte->LargePage != FALSE))
        {
            return (PT_ENTRY_64*)pdpte;
        }

        PDE_64* pd;
        PDE_64* pde;

        pd = (PDE_64*)GetVaFromPfn(pdpte->PageFrameNumber);

        pde = &pd[helper.AsIndex.Pd];

        if (Operation)
        {
            Operation((PT_ENTRY_64*)pde);
        }

        if ((pde->Present == FALSE) || (pde->LargePage != FALSE))
        {
            return (PT_ENTRY_64*)pde;
        }


        PTE_64* pt;
        PTE_64* pte;


        pt = (PTE_64*)GetVaFromPfn(pde->PageFrameNumber);

        pte = &pt[helper.AsIndex.Pt];

        if (Operation)
        {
            Operation((PT_ENTRY_64*)pte);
        }

        return  (PT_ENTRY_64*)pte;
    }

    /*  credits blackbone   */
    _MMVAD_SHORT* FindVadNode(UINT_PTR virtual_page, PEPROCESS process)
    {
        UINT_PTR vpnStart = virtual_page >> PAGE_SHIFT;
        PMM_AVL_TABLE table = (PMM_AVL_TABLE)((UINT8*)process + nt::vad_root); //for build 19041

        _RTL_BALANCED_NODE* to_examine = table->BalancedRoot;

        while (1)
        {
            _MMVAD_SHORT* VpnCompare = (_MMVAD_SHORT*)to_examine;
            _RTL_BALANCED_NODE* Child;
            UINT_PTR startVpn = VpnCompare->StartingVpn | (UINT_PTR)(VpnCompare->StartingVpnHigh << 32);
            UINT_PTR endVpn = VpnCompare->EndingVpn | (UINT_PTR)(VpnCompare->EndingVpnHigh << 32);

            if (vpnStart < startVpn) {

                Child = to_examine->Left;

                if (Child != NULL) {
                    to_examine = Child;
                }
                else {
                    DbgPrint("Node not found in tree");
                    return NULL;
                }
            }
            else if (vpnStart <= endVpn) {

                _MMVAD_SHORT* vad_entry = (_MMVAD_SHORT*)to_examine;
                
                DbgPrint("VAD node found, vpnStart %p endVpn %p \n", vpnStart, endVpn);

                return vad_entry;
            }
            else {

                Child = to_examine->Right;

                if (Child != NULL) {
                    to_examine = Child;
                }
                else {

                    DbgPrint("Node not found\n");
                    return NULL;
                }
            }
        }

        DbgPrint("Unreachable code");
        return NULL;
    }

    void* GetExport(uintptr_t base, char* export_name)
    {
        PIMAGE_NT_HEADERS nt_header = (PIMAGE_NT_HEADERS)(
            base + ((PIMAGE_DOS_HEADER)base)->e_lfanew
        );

        IMAGE_DATA_DIRECTORY data_dir =
            nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

        auto export_dir = (IMAGE_EXPORT_DIRECTORY*)(data_dir.VirtualAddress + base);

        for (int i = 0; i < export_dir->NumberOfFunctions; ++i)
        {
            int* function_array = (int*)(export_dir->AddressOfFunctions + base);
            int* name_array = (int*)(export_dir->AddressOfNames + base);
            int* ordinal_array = (int*)(export_dir->AddressOfNameOrdinals + base);

            char* name = (char*)(name_array[i] + base);

            if (strcmp(export_name, name) == 0)
            {
                int ordinal = ordinal_array[i];
                return (void*)function_array[ordinal];
            }
        }
    }

    PMDL LockPages(PVOID VirtualAddress, LOCK_OPERATION  operation)
    {
        PMDL mdl = IoAllocateMdl(VirtualAddress, PAGE_SIZE, FALSE, FALSE, nullptr);

        MmProbeAndLockPages(mdl, KernelMode, operation);

        return mdl;
    }

    NTSTATUS    UnlockPages(PMDL mdl)
    {
        MmUnlockPages(mdl);
        IoFreeMdl(mdl);

        return STATUS_SUCCESS;
    }

    HANDLE    GetProcessId(PCWSTR processName)
    {
        NTSTATUS status = STATUS_SUCCESS;
        PVOID buffer;


        buffer = ExAllocatePoolWithTag(NonPagedPool, 1024 * 1024, 'enoN');

        if (!buffer) {
            DbgPrint("couldn't allocate memory \n");
            return 0;
        }

        DbgPrintEx(0, 0, "Process list allocated at address %#x\n", buffer);

        PSYSTEM_PROCESS_INFORMATION pInfo = (PSYSTEM_PROCESS_INFORMATION)buffer;


        status = ZwQuerySystemInformation(SystemProcessInformation, pInfo, 1024 * 1024, NULL);
        if (!NT_SUCCESS(status)) {
            DbgPrintEx(0, 0, "ZwQuerySystemInformation Failed : STATUS CODE : %p\n", status);
            ExFreePoolWithTag(buffer, 'Enon');
            return 0;
        }

        UNICODE_STRING WantedImageName;

        RtlInitUnicodeString(&WantedImageName, processName);

        if (NT_SUCCESS(status)) {
            for (;;) {
                DbgPrintEx(0, 0, "\nProcess name: %ws | Process ID: %d\n", pInfo->ImageName.Buffer, pInfo->ProcessId); // Display process information.
                if (RtlEqualUnicodeString(&pInfo->ImageName, &WantedImageName, TRUE)) {
                    return pInfo->ProcessId;
                    break;
                }
                else if (pInfo->NextEntryOffset)
                    pInfo = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)pInfo + pInfo->NextEntryOffset);
                else
                    break;
            }
        }

        ExFreePoolWithTag(buffer, 'enoN');
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

    PVOID IATHook(PVOID lpBaseAddress, CHAR* lpcStrImport, PVOID lpFuncAddress)
    {
        if (!lpBaseAddress || !lpcStrImport || !lpFuncAddress)
            return NULL;

        PIMAGE_DOS_HEADER dosHeaders =
            reinterpret_cast<PIMAGE_DOS_HEADER>(lpBaseAddress);

        PIMAGE_NT_HEADERS ntHeaders =
            reinterpret_cast<PIMAGE_NT_HEADERS>(
                reinterpret_cast<DWORD_PTR>(lpBaseAddress) + dosHeaders->e_lfanew);

        IMAGE_DATA_DIRECTORY importsDirectory =
            ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

        PIMAGE_IMPORT_DESCRIPTOR importDescriptor =
            reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(importsDirectory.VirtualAddress + (DWORD_PTR)lpBaseAddress);

        LPCSTR libraryName = NULL;
        PVOID result = NULL;
        PIMAGE_IMPORT_BY_NAME functionName = NULL;

        if (!importDescriptor)
            return NULL;

        while (importDescriptor->Name != NULL)
        {
            libraryName = (LPCSTR)importDescriptor->Name + (DWORD_PTR)lpBaseAddress;

            ANSI_STRING LibName;
            UNICODE_STRING LibNameW;

            RtlInitAnsiString(&LibName, libraryName);
            RtlAnsiStringToUnicodeString(&LibNameW, &LibName, TRUE);

            if (utils::GetKernelModule(0, LibNameW))
            {
                PIMAGE_THUNK_DATA originalFirstThunk = NULL, firstThunk = NULL;
                originalFirstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)lpBaseAddress + importDescriptor->OriginalFirstThunk);
                firstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)lpBaseAddress + importDescriptor->FirstThunk);
                while (originalFirstThunk->u1.AddressOfData != NULL)
                {
                    functionName = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)lpBaseAddress + originalFirstThunk->u1.AddressOfData);
                    if (strcmp(functionName->Name, lpcStrImport) == 0)
                    {
                        // save old function pointer
                        result = reinterpret_cast<PVOID>(firstThunk->u1.Function);
                        auto irql = utils::DisableWP();
                        // swap address
                        firstThunk->u1.Function = reinterpret_cast<ULONG64>(lpFuncAddress);
                        utils::EnableWP(irql);
                        return result;
                    }
                    ++originalFirstThunk;
                    ++firstThunk;
                }
            }

            RtlFreeUnicodeString(&LibNameW);

            importDescriptor++;
        }
        return NULL;
    }

    NTSTATUS BBSearchPattern(IN PCUCHAR pattern, IN UCHAR wildcard, IN ULONG_PTR len, IN const VOID* base, IN ULONG_PTR size, OUT PVOID* ppFound)
    {
        ASSERT(ppFound != NULL && pattern != NULL && base != NULL);
        if (ppFound == NULL || pattern == NULL || base == NULL)
            return STATUS_INVALID_PARAMETER;

        for (ULONG_PTR i = 0; i < size - len; i++)
        {
            BOOLEAN found = TRUE;
            for (ULONG_PTR j = 0; j < len; j++)
            {
                if (pattern[j] != wildcard && pattern[j] != ((PCUCHAR)base)[i + j])
                {
                    found = FALSE;
                    break;
                }
            }

            if (found != FALSE)
            {
                *ppFound = (PUCHAR)base + i;
                return STATUS_SUCCESS;
            }
        }

        return STATUS_NOT_FOUND;
    }

    NTSTATUS BBScan(IN PCCHAR section, IN PCUCHAR pattern, IN UCHAR wildcard, IN ULONG_PTR len, OUT ULONG64* ppFound, PVOID base)
    {
        //ASSERT(ppFound != NULL);
        if (ppFound == NULL)
            return STATUS_ACCESS_DENIED; //STATUS_INVALID_PARAMETER

        PIMAGE_NT_HEADERS64 pHdr = (PIMAGE_NT_HEADERS64)RtlImageNtHeader(base);
        if (!pHdr)
            return STATUS_ACCESS_DENIED; // STATUS_INVALID_IMAGE_FORMAT;

        //PIMAGE_SECTION_HEADER pFirstSection = (PIMAGE_SECTION_HEADER)(pHdr + 1);
        PIMAGE_SECTION_HEADER pFirstSection = (PIMAGE_SECTION_HEADER)((uintptr_t)&pHdr->FileHeader + pHdr->FileHeader.SizeOfOptionalHeader + sizeof(IMAGE_FILE_HEADER));

        PVOID ptr = NULL;

        for (PIMAGE_SECTION_HEADER pSection = pFirstSection; pSection < pFirstSection + pHdr->FileHeader.NumberOfSections; pSection++)
        {
            ANSI_STRING s1, s2;

            RtlInitAnsiString(&s1, section);
            RtlInitAnsiString(&s2, (PCCHAR)pSection->Name);

            if (RtlCompareString(&s1, &s2, TRUE) == 0)
            {
                NTSTATUS status = BBSearchPattern(pattern, wildcard, len, (PUCHAR)base + pSection->VirtualAddress, pSection->Misc.VirtualSize, &ptr);

                if (NT_SUCCESS(status)) {

                    *(PULONG64)ppFound = (ULONG_PTR)(ptr); //- (PUCHAR)base
                    DbgPrint("found\r\n");
                    return status;
                }

            }

        }

        return STATUS_ACCESS_DENIED; //STATUS_NOT_FOUND;
    }

    ULONG64 GetSection(ULONG64  base, IN PCCHAR section, int* size)
    {
        auto pHdr = (PIMAGE_NT_HEADERS64)RtlImageNtHeader((PVOID)base);

        if (!pHdr)
        {
            return 0;
        }

        PIMAGE_SECTION_HEADER pFirstSection = (PIMAGE_SECTION_HEADER)((uintptr_t)&pHdr->FileHeader + pHdr->FileHeader.SizeOfOptionalHeader + sizeof(IMAGE_FILE_HEADER));

        PVOID ptr = NULL;

        for (PIMAGE_SECTION_HEADER pSection = pFirstSection; pSection < pFirstSection + pHdr->FileHeader.NumberOfSections; pSection++)
        {
            ANSI_STRING s1, s2;

            RtlInitAnsiString(&s1, section);
            RtlInitAnsiString(&s2, (PCCHAR)pSection->Name);

            if ((RtlCompareString(&s1, &s2, TRUE) == 0))
            {
                *size = pSection->SizeOfRawData;
                return pSection->VirtualAddress + base;
            }
        }

        return 0;
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
        LARGE_INTEGER time = { 0 };
        time.QuadPart = -250ll * 10 * 1000;     // 250 msec.
        BOOLEAN	 IsWow64 = (PsGetProcessWow64Process(pProcess) != NULL) ? TRUE : FALSE;
        // Wow64 process
        if (IsWow64)
        {
            PPEB32 pPeb32 = (PPEB32)PsGetProcessWow64Process(pProcess);
            if (pPeb32 == NULL)
            {
                return NULL;
            }

            // Wait for loader a bit
            for (INT i = 0; !pPeb32->Ldr && i < 10; i++)
            {
                KeDelayExecutionThread(KernelMode, TRUE, &time);
            }

            // Still no loader
            if (!pPeb32->Ldr)
            {
                return NULL;
            }

            // Search in InLoadOrderModuleList
            for (PLIST_ENTRY32 pListEntry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList.Flink;
                pListEntry != &((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList;
                pListEntry = (PLIST_ENTRY32)pListEntry->Flink)
            {
                UNICODE_STRING ustr;
                PLDR_DATA_TABLE_ENTRY32 pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

                RtlUnicodeStringInit(&ustr, (PWCH)pEntry->BaseDllName.Buffer);

                if (RtlCompareUnicodeString(&ustr, ModuleName, TRUE) == 0)
                    return (PVOID)pEntry->DllBase;
            }
        }
        else
        {
            PPEB pPeb = PsGetProcessPeb(pProcess);
            if (!pPeb)
            {
                return NULL;
            }

            // Wait for loader a bit
            for (INT i = 0; !pPeb->Ldr && i < 10; i++)
            {
                KeDelayExecutionThread(KernelMode, TRUE, &time);
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
                PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
                if (RtlCompareUnicodeString(&pEntry->BaseDllName, ModuleName, TRUE) == 0)
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

    void dump_driver(PVOID baseAddress, const wchar_t* path, bool fixPe)
    {
        PIMAGE_DOS_HEADER dosHeaders =
            reinterpret_cast<PIMAGE_DOS_HEADER>(baseAddress);

        IMAGE_NT_HEADERS64* ntHeaders =
            reinterpret_cast<IMAGE_NT_HEADERS64*>(
                reinterpret_cast<DWORD_PTR>(baseAddress) + dosHeaders->e_lfanew);


        auto sizeOfModule = ntHeaders->OptionalHeader.SizeOfImage;
        auto allocatedPool = ExAllocatePoolWithTag(NonPagedPool, sizeOfModule, 'mMmS');

        MM_COPY_ADDRESS address = { NULL };

        address.VirtualAddress = baseAddress;

        SIZE_T bytesRead = NULL;

        MmCopyMemory(allocatedPool, address, sizeOfModule, MM_COPY_MEMORY_VIRTUAL, &bytesRead);

        if (fixPe) {
            PIMAGE_DOS_HEADER poolDosHeaders =
                reinterpret_cast<PIMAGE_DOS_HEADER>(allocatedPool);
            IMAGE_NT_HEADERS64* poolNtHeaders =
                reinterpret_cast<IMAGE_NT_HEADERS64*>(
                    reinterpret_cast<DWORD_PTR>(allocatedPool) + poolDosHeaders->e_lfanew);

            auto pSectionHeader = (IMAGE_SECTION_HEADER*)(poolNtHeaders + 1);

            for (int i = 0; i < poolNtHeaders->FileHeader.NumberOfSections; i++, pSectionHeader++) {
                pSectionHeader->PointerToRawData = pSectionHeader->VirtualAddress;
                pSectionHeader->SizeOfRawData = pSectionHeader->Misc.VirtualSize;
            }

        }

        CreateFile(allocatedPool, path, bytesRead);

        ExFreePoolWithTag(allocatedPool, 'mMmS');
        return;
    };
}
