#include "bypass.h"
#include "hooks.h"
#include "intel_api.h"
#include "veh.h"

namespace Hooks
{
	bool hk_is_bad_read_ptr(void* ptr, UINT_PTR size)
	{
        MEMORY_BASIC_INFORMATION mbi;
        BOOL ok;
        DWORD mask;
        BYTE* p = (BYTE*)ptr;
        BYTE* maxp = p + size;
        BYTE* regend = NULL;

        if (size == 0)
        {
            return FALSE;
        }

        if (p == NULL)
        {
            return TRUE;
        }

        mask = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

        do
        {
            if (p == ptr || p == regend)
            {
                if (VirtualQuery((LPCVOID)p, &mbi, sizeof(mbi)) == 0)
                {
                    return TRUE;
                }
                else
                {
                    regend = ((BYTE*)mbi.BaseAddress + mbi.RegionSize);
                }
            }

            ok = (mbi.Protect & mask) != 0;

            if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
            {
                ok = FALSE;
            }

            if (!ok)
            {
                return TRUE;
            }

            if (maxp <= regend) /* the whole address range is inside the current memory region */
            {
                return FALSE;
            }
            else if (maxp > regend) /* this region is a part of (or overlaps with) the address range we are checking */
            {
                p = regend; /* lets move to the next memory region */
            }
        } while (p < maxp);

        return FALSE;
	}

	char hk_call_veh(IN PEXCEPTION_RECORD ExceptionRecord, IN PCONTEXT Context, unsigned int a3)
	{
		ExceptionRecord->ExceptionInformation[13] = 0;

		if (*(BYTE*)Context->Rip == 0xCC)
		{
			auto kernel32 = GetModuleHandle(L"kernel32.dll");

			/*	save return address & spoof	*/

			ExceptionRecord->ExceptionInformation[13] = *(uintptr_t*)Context->Rsp;

			*(uintptr_t*)Context->Rsp = ((uintptr_t)kernel32 + 0x5000);
		}

		
		auto result = static_cast<decltype(&hk_call_veh)>((void*)Hooks::call_veh->original_bytes)(ExceptionRecord, Context, a3);

		if (ExceptionRecord->ExceptionInformation[13] != 0)
		{
			/*	restore return address	*/

			*(uintptr_t*)Context->Rsp = ExceptionRecord->ExceptionInformation[13];
		}

		return result;
	}
};

void WriteToReadOnly(void* address, uint8_t* bytes, size_t len)
{
    DWORD old_prot;
    VirtualProtect((LPVOID)address, len, PAGE_EXECUTE_READWRITE, &old_prot);
    memcpy((void*)address, (void*)bytes, len);
    VirtualProtect((LPVOID)address, len, old_prot, 0);
}


void BypassBattleye()
{
	auto ntdll = GetModuleHandle(L"ntdll.dll");
	auto kernel32 = GetModuleHandle(L"kernel32.dll");

	/*	45 33 C0 49 8B D7 48 8B CE E8 ? ? ? ? reference at + 9 */

	auto callvectoredhandlers = RELATIVE_ADDR(
		Utils::FindPattern((uintptr_t)ntdll, PeHeader(ntdll)->OptionalHeader.SizeOfImage, "\x45\x33\xC0\x49\x8B\xD7\x48\x8B\xCE\xE8\x00\x00\x00\x00", 14, 0x00) + 10,
		1, 5);

	APPLY_HOOK(JmpRaxCode, call_veh, callvectoredhandlers, (HOOK_MODE)DEFAULT_HOOK_MODE);

	/*	45 33 C0 49 8B D7 48 8B CE E8 ? ? ? ? reference at + 9 */

	auto isbadreadptr = GetProcAddress(kernel32, "IsBadReadPtr");

	APPLY_HOOK(JmpRaxCode, is_bad_read_ptr, isbadreadptr, (HOOK_MODE)DEFAULT_HOOK_MODE);
}

int main()
{
    BypassBattleye();

    auto result = IsBadReadPtr(0x004);
    auto result2 = IsBadReadPtr(&BypassBattleye);

    std::cout << "IsBadReadPtr() result: " << std::hex << result <<  << " result2: " << result2 << std::endl;

    veh::add_func(OpenFileMapping);
    Sleep(500);
    OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, wszHeaderName);    
}