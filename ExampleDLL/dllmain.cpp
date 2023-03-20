// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "portable_executable.h"
#include "utils.h"
#include "globals.h"

void StartCheat()
{
	MessageBoxA(NULL, "Injected!!", "2D-injector", MB_OK);
}

__declspec(dllexport) DllParams* Global::dll_params = 0;

bool entry_not_called = true;

extern "C" __declspec(dllexport) void HookEntryPoint()
{
	if (entry_not_called)
	{
		entry_not_called = false;

		auto dllparams = Global::dll_params;

		PE::ResolveImports((uint8_t*)dllparams->payload_dll_base);

		StartCheat();
	}

	return;
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
		Global::dll_params->dll_base = (uintptr_t)hModule;
		Global::dll_params->dll_size = PeHeader(hModule)->OptionalHeader.SizeOfImage;

		StartCheat();
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}