#include "be_logging.h"

#define DEFAULT_HOOK_MODE  2 // 1 == INTEL | 2 == AMD | 3 == normal

#define LOG_FILE "C:\\Users\\user123\\Desktop\\testing_drivers\\test_logs.txt"

enum INJECTOR_CONSTANTS
{
	mapped_dll_header = 0x12345678,
};

size_t dll_file_size = 0;

__declspec(dllexport) DllParams* Global::dll_params = 0;

bool entry_not_called = true;


extern "C" __declspec(dllexport) HRESULT __fastcall HookEntryPoint(IDXGISwapChain * pChain, UINT SyncInterval, UINT Flags)
{
	if (entry_not_called)
	{
		entry_not_called = false;

		for (uintptr_t address = (uintptr_t)&PE::ResolveImports; address; address -= sizeof(uint16_t))
		{
			if (*(uint32_t*)address == INJECTOR_CONSTANTS::mapped_dll_header)
			{
				Global::dll_params = (DllParams*)address;
				Global::dll_params->dll_size = PeHeader(address)->OptionalHeader.SizeOfImage;

				break;
			}
		}

		/*	restore entrypoint hook		*/
		if (DEFAULT_HOOK_MODE == 1)
		{
			auto dxgi = (uintptr_t)GetModuleHandle(L"dxgi.dll");

			auto present_address = Utils::FindPattern(
				dxgi, PeHeader(dxgi)->OptionalHeader.SizeOfImage,
				"\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x60", 35, 0x00
			) - 5;

			// IntelVisor::RemoveEptHookUsermode((void*)present_address);
		}
		else if (DEFAULT_HOOK_MODE == 2)
		{
			auto dxgi = (uintptr_t)GetModuleHandle(L"dxgi.dll");

			uintptr_t present_address = 0;

			for (present_address = (uintptr_t)(dxgi + 0x9000); present_address; present_address -= sizeof(uint16_t))
			{
				if (*(uintptr_t*)present_address == (uintptr_t)HookEntryPoint)
				{
					present_address -= 6;

					break;
				}
			}

			memcpy((void*)present_address, Global::dll_params->original_present_bytes, Global::dll_params->o_present_bytes_size);
		}

		PE::ResolveImports((uint8_t*)Global::dll_params->dll_base);

		StartBELogger();
	}

	return 0;
}

extern "C" __declspec(dllexport) void __fastcall CreateUserThreadEntry()
{
	for (uintptr_t address = (uintptr_t)&PE::ResolveImports; address; address -= sizeof(uint16_t))
	{
		if (*(uint32_t*)address == INJECTOR_CONSTANTS::mapped_dll_header)
		{
			Global::dll_params = (DllParams*)address;
			Global::dll_params->dll_base = (uintptr_t)Global::dll_params + PAGE_SIZE;
			Global::dll_params->dll_size = PeHeader(address)->OptionalHeader.SizeOfImage;

			break;
		}
	}

	PE::ResolveImports((uint8_t*)Global::dll_params->dll_base);

	StartBELogger();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		Global::dll_params = new DllParams;
		Global::dll_params->dll_size = PeHeader(hModule)->OptionalHeader.SizeOfImage;
		Global::dll_params->dll_base = (uintptr_t)hModule;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartBELogger, NULL, NULL, NULL);

		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
