#pragma once
#include <Windows.h>
#include "communicate.h"

#define PeHeader(image) (IMAGE_NT_HEADERS64*)((uint64_t)image + ((IMAGE_DOS_HEADER*)image)->e_lfanew)
#define StrToWStr(s) (std::wstring(s, &s[strlen(s)]).c_str())

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

void CopyHeaders(IMAGE_DOS_HEADER* src, BYTE* dest)
{
	auto pe_hdr = PeHeader(src);

	auto section = (IMAGE_SECTION_HEADER*)(pe_hdr + 1);

	memcpy(dest, src, pe_hdr->OptionalHeader.SizeOfHeaders);
}

void CopySections(IMAGE_DOS_HEADER* src, BYTE* dest)
{
	auto pe_hdr = PeHeader(src);

	auto section = (IMAGE_SECTION_HEADER*)(pe_hdr + 1);

	for (int i = 0; i < pe_hdr->FileHeader.NumberOfSections; ++i)
	{
		memcpy(dest + section[i].VirtualAddress,
			(BYTE*)src + section[i].PointerToRawData,
			section[i].SizeOfRawData
		);
	}
}

DWORD64 RvaToOffset(IMAGE_DOS_HEADER* base, ULONG64 rva)
{
	int i;
	WORD wSections;
	PIMAGE_SECTION_HEADER pSectionHdr;
	auto pe_hdr = (IMAGE_NT_HEADERS64*)((ULONG64)base + base->e_lfanew);
	
	pSectionHdr = IMAGE_FIRST_SECTION(pe_hdr);
	wSections = pe_hdr->FileHeader.NumberOfSections;

	for (i = 0; i < wSections; i++)
	{
		if (pSectionHdr->VirtualAddress <= rva)
			if ((pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize) > rva)
			{
				rva -= pSectionHdr->VirtualAddress;
				rva += pSectionHdr->PointerToRawData;

				return rva;
			}

		pSectionHdr++;
	}

	return (DWORD64)-1;
}



VOID ResolveRelocations(BYTE* base, PBYTE mapped) 
{
	auto nt = PeHeader(base);
	auto& baseRelocDir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	if (!baseRelocDir.VirtualAddress) {
		return;
	}

	auto reloc = (PIMAGE_BASE_RELOCATION)(base + baseRelocDir.VirtualAddress);

	if (!reloc) {
		return;
	}

	for (auto currentSize = 0UL; currentSize < baseRelocDir.Size; ) {
		auto relocCount = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		auto relocData = reinterpret_cast<PWORD>(reinterpret_cast<PBYTE>(reloc) + sizeof(IMAGE_BASE_RELOCATION));
		auto relocBase = reinterpret_cast<PBYTE>
			(base + reloc->VirtualAddress);

		for (auto i = 0UL; i < relocCount; ++i, ++relocData) {
			auto data = *relocData;
			auto type = data >> 12;
			auto offset = data & 0xFFF;

			if (type == IMAGE_REL_BASED_DIR64) {
				*reinterpret_cast<PBYTE*>(relocBase + offset) += (mapped - reinterpret_cast<PBYTE>(nt->OptionalHeader.ImageBase));
			}
		}

		currentSize += reloc->SizeOfBlock;
		reloc = reinterpret_cast<PIMAGE_BASE_RELOCATION>(relocData);
	}
}

BOOLEAN ResolveImports(int target_pid, PBYTE base)
{
	auto nt = PeHeader(base);

	auto rva = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	if (!rva) {
		return TRUE;
	}

	auto importDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(
		base + rva
	);

	if (!importDescriptor) {
		return TRUE;
	}

	for (; importDescriptor->FirstThunk; ++importDescriptor) {
		auto moduleName = reinterpret_cast<PCHAR>(base + importDescriptor->Name);
		if (!moduleName) {
			break;
		}
		std::cout << "module name " << moduleName << std::endl;

		auto module = LoadLibraryA(moduleName);
		if (!module) {
			std::cout << "failed to load module: \n" << moduleName;
			continue;
		}

		if (Driver::GetModuleBase(StrToWStr(moduleName), target_pid) == 0) 
		{
			std::cout << "target process does not have " << moduleName
				<< " loaded because this DLL isn't supposed to be globally mapped \n";

			continue;
		}

		for (auto thunk = (PIMAGE_THUNK_DATA)(base + importDescriptor->FirstThunk);
			thunk->u1.AddressOfData; ++thunk)
		{
			auto importByName = (PIMAGE_IMPORT_BY_NAME)(base + thunk->u1.AddressOfData);
			thunk->u1.Function = (ULONG64)GetProcAddress(module, importByName->Name);
			std::cout << "Import " << importByName->Name << ": " << std::hex << thunk->u1.Function << std::endl;
		}
	}

	return TRUE;
}

void* GetExport(uintptr_t base, char* export_name)
{
	PIMAGE_NT_HEADERS nt_header = PeHeader(base);

	IMAGE_DATA_DIRECTORY data_dir =
		nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	auto export_dir = (IMAGE_EXPORT_DIRECTORY*)(data_dir.VirtualAddress + base);

	auto function_array = (int*)(export_dir->AddressOfFunctions + base);
	auto name_array = (int*)(export_dir->AddressOfNames + base);
	auto ordinal_array = (int16_t*)(export_dir->AddressOfNameOrdinals + base);

	for (int i = 0; i < export_dir->NumberOfFunctions; ++i)
	{
		char* name = (char*)(name_array[i] + base);

		std::cout << "name " << name << " export_name " << export_name << std::endl;

		if (!strcmp(export_name, name))
		{
			int ordinal = ordinal_array[i];
			return (void*)function_array[ordinal];
		}
	}

	return NULL;
}

IMAGE_DOS_HEADER* PrepareImage(HANDLE h_dll, int size, ULONG64 map_base, int target_pid)
{
	SetFilePointer(h_dll, 0, 0, 0);

	auto imagebuf1 = new BYTE[size];	// mapped to file offset
	
	DWORD bytes;
	ReadFile(h_dll, imagebuf1, size, &bytes, NULL);
	auto PeHdr = PeHeader(imagebuf1);
	auto size2 = PeHdr->OptionalHeader.SizeOfImage;
	
	auto imagebuf2 = new BYTE[size2];	// mapped to RVA

	CopyHeaders((IMAGE_DOS_HEADER*)imagebuf1, imagebuf2);
	CopySections((IMAGE_DOS_HEADER*)imagebuf1, imagebuf2);

	delete imagebuf1;

	ResolveRelocations(imagebuf2, (BYTE*)map_base);
	ResolveImports(target_pid, imagebuf2);

	return (IMAGE_DOS_HEADER*)imagebuf2;
}