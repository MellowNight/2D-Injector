#pragma once
#include "utils.h"

#define PeHeader(image) ((IMAGE_NT_HEADERS64*)((uint64_t)image + ((IMAGE_DOS_HEADER*)image)->e_lfanew))

namespace PE
{
	bool InitializeTLS(uint8_t* dll_base)
	{
		auto pe_hdr = PeHeader(dll_base);

		if (pe_hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
		{
			auto tls_dir = (IMAGE_TLS_DIRECTORY*)(dll_base + pe_hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);

			auto tls_callbacks = (PIMAGE_TLS_CALLBACK*)tls_dir->AddressOfCallBacks;

			for (; tls_callbacks && *tls_callbacks; ++tls_callbacks)
			{
				__debugbreak();

				(*tls_callbacks)(dll_base, DLL_PROCESS_ATTACH, nullptr);
			}
			return true;
		}
		else
		{
			__debugbreak();

			return false;
		}
	}

	bool ResolveImports(uint8_t* base)
	{
		auto pe_hdr = PeHeader(base);

		auto rva = pe_hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

		if (!rva)
		{
			return false;
		}

		auto import_desc = (IMAGE_IMPORT_DESCRIPTOR*)(base + rva);

		for (; import_desc->FirstThunk; ++import_desc)
		{
			auto mod_name = (char*)(base + import_desc->Name);

			if (!mod_name)
			{
				break;
			}

			auto module = LoadLibraryA(mod_name);

			bool manualmap_dependency = false;

			for (auto of_thunk = (IMAGE_THUNK_DATA*)(base + import_desc->OriginalFirstThunk),
				thunk = (IMAGE_THUNK_DATA*)(base + import_desc->FirstThunk);
				of_thunk->u1.AddressOfData;
				++of_thunk, ++thunk)
			{
				auto import_by_name = (IMAGE_IMPORT_BY_NAME*)(base + of_thunk->u1.AddressOfData);

				if (Utils::IsAddressValid(import_by_name))
				{
					thunk->u1.Function = (uintptr_t)GetProcAddress(module, import_by_name->Name);
				}
				else
				{
					thunk->u1.Function = NULL;
				}
			}
		}

		return true;
	}
}