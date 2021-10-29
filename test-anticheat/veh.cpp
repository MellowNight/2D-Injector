#include "veh.h"
#include "utils.h"

std::vector<std::tuple<uintptr_t, BYTE>>checks;

LONG WINAPI veh::be_handler(struct _EXCEPTION_POINTERS* ExceptionInfo) {

	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		for (std::vector<std::tuple<uintptr_t, BYTE>>::iterator it = checks.begin(); it != checks.end(); ++it)
		{
			if (std::get<0>(*it) == (uintptr_t)ExceptionInfo->ContextRecord->Rip) 
			{
				DWORD old;
				VirtualProtect((LPVOID)ExceptionInfo->ContextRecord->Rip, 0x1, PAGE_EXECUTE_READWRITE, &old);
				*(BYTE*)ExceptionInfo->ContextRecord->Rip = 0xC3;
				VirtualProtect((LPVOID)ExceptionInfo->ContextRecord->Rip, 0x1, old, &old);

				utils::log("[ANTICHEAT] breakpoint hit! RIP %p, byte %p \n", ExceptionInfo->ContextRecord->Rip, *(BYTE*)ExceptionInfo->ContextRecord->Rip);

				uintptr_t return_address = *(uintptr_t*)ExceptionInfo->ContextRecord->Rsp;

				MEMORY_BASIC_INFORMATION mbi{ 0 };
				size_t return_length{ 0 };

				if (
					(VirtualQuery((PVOID)return_address, &mbi, sizeof(mbi)) < 0) ||
					mbi.State != MEM_COMMIT ||
					mbi.Type != MEM_IMAGE && mbi.RegionSize > 0x2000 ||
					*(WORD*)return_address == 0x23FF || //https://www.unknowncheats.me/forum/anti-cheat-bypass/268039-x64-return-address-spoofing-source-explanation.html
					*(WORD*)return_address == 0x26FF ||
					*(WORD*)return_address == 0x27FF ||
					*(WORD*)return_address == 0x65FF ||
					*(WORD*)return_address == 0xE3FF
					) {

					utils::log("cheater return address! %p \n", return_address);
				}
			}
		}


		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
		if (ExceptionInfo->ContextRecord->Rip >= 0x8000000000000000) 
		{ // Perfect Injector
			MessageBox(NULL, L"Perfect Injector dedected! \n", L"Perfect Injector dedected! \n", NULL);
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if (ExceptionInfo->ExceptionRecord->ExceptionCode == DBG_PRINTEXCEPTION_C)
	{
		// for OutputDebugString
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void veh::set_veh(uintptr_t func) 
{
	AddVectoredExceptionHandler(true, veh::be_handler);

	if (!func)
	{
		return;
	}
	
	utils::log("[Anticheat] func at %p \n", func);

	while (*(BYTE*)func != 0xC3)
	{
		func += 1;
	}

	utils::log("[Anticheat] func at %p \n", func);

	std::tuple<uintptr_t, BYTE> newcheck{ func, *(BYTE*)func };
	checks.push_back(newcheck);
	DWORD old;
	VirtualProtect((LPVOID)func, 0x1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)func = 0xCC;
	VirtualProtect((LPVOID)func, 0x1, old, &old);
}

void veh::remove_veh(uintptr_t func) {
	if (!func)
	{
		return;
	}

	RemoveVectoredExceptionHandler(veh::be_handler);

	for (auto it = checks.begin(); it != checks.end(); ++it)
	{
		if (std::get<0>(*it) == func)
		{
			DWORD old;
			VirtualProtect((LPVOID)func, 0x1, PAGE_EXECUTE_READWRITE, &old);
			*(BYTE*)func = 0xC3;
			VirtualProtect((LPVOID)func, 0x1, old, &old);
			checks.erase(it);
		}
	}
}