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

bool ResolveExports(BYTE* base)
{
	auto pe_header = PeHeader(base);

	auto export_dir = (IMAGE_EXPORT_DIRECTORY*)(base + pe_header->OptionalHeader.DataDirectory->VirtualAddress);

	auto export_functions = (uint32_t*)(base + export_dir->AddressOfFunctions);
	auto export_names = (uint32_t*)(base + export_dir->AddressOfNames);
	auto export_ordinals = (uint16_t*)(base + export_dir->AddressOfNameOrdinals);

	for (auto i = 0u; i < export_dir->NumberOfNames; i++)
	{
		const auto export_name = reinterpret_cast<const char*>(base + export_names[i]);

	/*	if (strcmp(export_name, "desired_export"))
			continue;

		return reinterpret_cast<void*>(module_address + export_functions[export_ordinals[i]]);
		*/
	}
}

bool ResolveImports(int target_pid, BYTE* base)
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
			std::cout << "target process does not have " << moduleName << " loaded\n";
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

IMAGE_DOS_HEADER* MapImage(BYTE* dll_bytes, int file_size, ULONG64 map_base, int target_pid)
{
	auto pe_hdr = PeHeader(dll_bytes);
	auto real_size = pe_hdr->OptionalHeader.SizeOfImage;

	auto mapped_image = new BYTE[real_size];	// mapped to RVA

	CopyHeaders((IMAGE_DOS_HEADER*)dll_bytes, mapped_image);
	CopySections((IMAGE_DOS_HEADER*)dll_bytes, mapped_image);

	ResolveRelocations(mapped_image, (BYTE*)map_base);
	ResolveImports(target_pid, mapped_image);


	return (IMAGE_DOS_HEADER*)mapped_image;
}