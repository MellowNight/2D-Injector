#include "veh.h"

#define LOG_FILE ("testlogs.txt")

void LogToFile(const char* file_name, const char* format, ...)
{
    auto file_handle = CreateFileA(
        file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );

    SetFilePointer(file_handle, 0, 0, FILE_END);

    if (GetLastError() != 183 && GetLastError() != 0)
    {			
        return;
    }
    char buffer[256] = { 0 };

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 255, format, args);

    size_t written;

    WriteFile(file_handle, buffer, strlen(buffer), (LPDWORD)&written, NULL);

    if (GetLastError() != 183 && GetLastError() != 0)
    {
        MessageBoxA(NULL, "WriteFile GetLastError", std::to_string(GetLastError()).c_str(), MB_OK);
    }

    va_end(args);

    CloseHandle(file_handle);
};


std::vector<std::tuple<uintptr_t, BYTE>>checks;
LONG WINAPI veh::be_handler(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
		for (std::vector<std::tuple<uintptr_t, BYTE>>::iterator it = checks.begin(); it != checks.end(); ++it)
		{
			if (std::get<0>(*it) == (uintptr_t)ExceptionInfo->ExceptionRecord->ExceptionAddress) {
				DWORD old;
				VirtualProtect((LPVOID)ExceptionInfo->ContextRecord->Rip, 0x1, PAGE_EXECUTE_READWRITE, &old);
				*(BYTE*)ExceptionInfo->ContextRecord->Rip = 0xC3;
				VirtualProtect((LPVOID)ExceptionInfo->ContextRecord->Rip, 0x1, old, &old);

	

				uintptr_t return_address = *(uintptr_t*)ExceptionInfo->ContextRecord->Rsp;
				MEMORY_BASIC_INFORMATION mbi{ 0 };
				size_t return_length{ 0 };
				if (
					(NtQueryVirtualMemory((HANDLE)-1, (PVOID)return_address, MemoryBasicInformation, &mbi, sizeof(mbi), &return_length) < 0) ||
					mbi.State != MEM_COMMIT ||
					mbi.Type != MEM_IMAGE && mbi.RegionSize > 0x2000 ||
					*(WORD*)return_address == 0x23FF || //https://www.unknowncheats.me/forum/anti-cheat-bypass/268039-x64-return-address-spoofing-source-explanation.html
					*(WORD*)return_address == 0x26FF ||
					*(WORD*)return_address == 0x27FF ||
					*(WORD*)return_address == 0x65FF ||
					*(WORD*)return_address == 0xE3FF
					) {

					//reportbuffer
        			LogToFile(LOG_FILE, "Illegal caller caught! \n");
				}
			

			}
		}
	
		
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
		if (ExceptionInfo->ContextRecord->Rip >= 0x8000000000000000) { //Perfect Injector
			LogToFile(LOG_FILE, "Illegal caller caught! \n");
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void veh::add_func(uintptr_t func) {
	if (!func)
	{
		return;
	}
	while (*(BYTE*)func != 0xC3)
		func += 1;

	std::tuple<uintptr_t, BYTE>newcheck{ func, *(BYTE*)func };
	checks.push_back(newcheck);
	DWORD old;
	VirtualProtect((LPVOID)func, 0x1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)func = 0xCC;
	VirtualProtect((LPVOID)func, 0x1, old, &old);
}