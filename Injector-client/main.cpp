#include "communicate.h"
#include "manual_map.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include "security.h"

struct DllParams
{
	uintptr_t module_base;
};

#define HOST_DLL_NAME	L"OWClient.dll"
#define HOST_DLL_PATH	"C:\\Users\\user123\\Desktop\\OWClient.dll"
#define ENTRYPOINT_NAME	"HookEntryPoint"

void InvokeSignedDllRemoteFunction(int32_t pid, uintptr_t host_dll_handle, uint8_t* entry_address)
{
	/*	find the threads associated with this target process	*/

	struct ProcessInfo
	{
		uint32_t thread_id;
		uint32_t pid;
	} proc_info;

	proc_info.pid = pid;

	EnumWindows(
		(WNDENUMPROC)[](HWND hwnd, LPARAM lparam) -> BOOL CALLBACK {
			
			auto proc_info = (ProcessInfo*)lparam;

			unsigned long process_id = 0;

			auto thread_id = GetWindowThreadProcessId(hwnd, &process_id);

			if (proc_info->pid != process_id || !(GetWindow(hwnd, GW_OWNER) == NULL)
				|| !IsWindowVisible(hwnd))
			{
				return TRUE;
			}
			else
			{
				/*	we found main window for setwindowshookex injection	*/

				proc_info->thread_id = thread_id;

				return FALSE;
			}
		},
	(LPARAM)&proc_info);

	auto hook = SetWindowsHookExA(WH_GETMESSAGE, (HOOKPROC)entry_address, (HINSTANCE)host_dll_handle, proc_info.thread_id);

	PostThreadMessageW(proc_info.thread_id, 0x123, 0, 0);

	Sleep(5);

	auto status = UnhookWindowsHookEx(hook);

	std::cout << "UnhookWindowsHookEx " << std::hex << status << " GetLastError() " << GetLastError() << std::endl;

	/*	write a return true (\xb0\x01\xC3) to the entry point to avoid crashes caused by dllmain being called multiple times.
		Every time SetWindowsHookEx is called, Dllmain is invoked.
	*/

	static bool entrypoint_patched = false;

	if (!entrypoint_patched)
	{
		auto dll_entrypoint = PeHeader(host_dll_handle)->OptionalHeader.AddressOfEntryPoint + host_dll_handle;

		Driver::SetNptHook(pid, 3, (uintptr_t)dll_entrypoint, (BYTE*)"\xb0\x01\xC3");

		entrypoint_patched = true;
	}
}

uintptr_t LoadSignedHostDLL(int32_t pid, const char* signed_dll_name)
{
	/*	use Overwolf's DLL to carry our cheat	*/

	auto host_dll = (uintptr_t)LoadLibraryExA(signed_dll_name, nullptr, DONT_RESOLVE_DLL_REFERENCES);

	/*	Find ret (0xC3) instruction	*/

	uintptr_t ret_byte = 0;

	for (ret_byte = (uintptr_t)host_dll + 0x1000; *(uint8_t*)ret_byte != 0xC3; ++ret_byte)
	{
	}

	/*	Spam execute a ret using SetWindowsHookEx in target process to load the signed DLL in the target process	*/

	InvokeSignedDllRemoteFunction(pid, host_dll, (uint8_t*)ret_byte);

	return Driver::GetModuleBase(L"OWClient.dll", pid);
}

extern "C" __declspec(dllexport) int InjectDLLBytes(int32_t pid, uint8_t* raw_cheat_dll, const char* entrypoint_name, const char* signed_dll_name)
{
	if (*(int32_t*)raw_cheat_dll != INJECTOR_PASSWORD)
	{
		SecurityViolation();
	}
	else
	{
		*(int32_t*)raw_cheat_dll = 0x5A4D;
	}

	Driver::Init();

	/* We will be stealthily mapping our cheat on top of a signed DLL */

	auto cheat_base = LoadSignedHostDLL(pid, signed_dll_name);

	auto host_dll_base = cheat_base;

	auto image_real_size = PeHeader(raw_cheat_dll)->OptionalHeader.SizeOfImage;


	/*	align .rdata section of our own DLL with the .data section of the host DLL, because we can't hide .rdata strings	*/

	uintptr_t rdata_offset = 0;

	auto section = (IMAGE_SECTION_HEADER*)(PeHeader(raw_cheat_dll) + 1);

	for (int i = 0; i < PeHeader(raw_cheat_dll)->FileHeader.NumberOfSections; ++i)
	{
		if (!strcmp(".rdata", (char*)section[i].Name))
		{
			rdata_offset = section[i].VirtualAddress;
		}
	}

	section = (IMAGE_SECTION_HEADER*)(PeHeader(host_dll_base) + 1);

	for (int i = 0; i < PeHeader(host_dll_base)->FileHeader.NumberOfSections; ++i)
	{
		if (!strcmp(".data", (char*)section[i].Name))
		{
			cheat_base += section[i].VirtualAddress;
			cheat_base -= rdata_offset;
		}
	}

	/*	prepare the cheat for manual mapping	*/

	uint8_t* cheat_mapped = NULL;

	PE::RemapImage(raw_cheat_dll, &cheat_mapped, pid, cheat_base);

	std::cout << std::hex << " cheat_base 0x" << cheat_base << std::endl;


	/*	write pe headers and then hide every section of the cheat DLL except for .rdata, .pdata and .data	*/

	section = (IMAGE_SECTION_HEADER*)(PeHeader(cheat_mapped) + 1);
	
	uint8_t buffer;

	auto header_size = PeHeader(cheat_mapped)->OptionalHeader.SizeOfHeaders;

	auto value = Driver::ReadMem(pid, cheat_base, &buffer, 1);
	Driver::SetNptHook(pid, header_size, cheat_base, cheat_mapped);

	//Driver::ProtectMemory(pid, cheat_base, PAGE_EXECUTE_READWRITE, header_size);
	//Driver::WriteMem(pid, cheat_base, cheat_mapped, header_size);

	for (int i = 0; i < PeHeader(cheat_mapped)->FileHeader.NumberOfSections; ++i)
	{
		for (auto page = cheat_mapped + section[i].VirtualAddress;
			page < cheat_mapped + section[i].VirtualAddress + section[i].SizeOfRawData;
			page += PAGE_SIZE)
		{
			if (strcmp((char*)section[i].Name, ".rdata") && strcmp((char*)section[i].Name, ".data") &&
				strcmp((char*)section[i].Name, ".pdata"))
			{
				auto value = Driver::ReadMem(pid, cheat_base + (page - cheat_mapped), &buffer, 1);
				Driver::SetNptHook(pid, PAGE_SIZE, cheat_base + (page - cheat_mapped), page);
				
				////Driver::ProtectMemory(pid, cheat_base + (page - cheat_mapped), PAGE_EXECUTE_READWRITE, PAGE_SIZE);
				//Driver::WriteMem(pid, cheat_base + (page - cheat_mapped), page, PAGE_SIZE);
			}
			else
			{
				Driver::WriteMem(pid, cheat_base + (page - cheat_mapped), page, PAGE_SIZE);
			}
		}
	}

	/*	write DLL parameters	*/

	DllParams params;

	params.module_base = cheat_base;

	auto params_location = Driver::AllocateMemory(pid, sizeof(DllParams));

	auto params_export = (uintptr_t)PE::GetExport((uintptr_t)cheat_mapped, "dll_params");

	Driver::WriteMem(pid, params_location, (BYTE*)&params, sizeof(params));
	Driver::WriteMem(pid, cheat_base + params_export, (BYTE*)&params_location, sizeof(DllParams*));


	/*	invoke the entry point	*/

	auto entry_point = (uintptr_t)PE::GetExport((uintptr_t)cheat_mapped, entrypoint_name);

	std::cout << std::hex << " entry_point 0x" << entry_point + cheat_base << std::endl;

	InvokeSignedDllRemoteFunction(pid, host_dll_base, (uint8_t*)entry_point + cheat_base);

	return 0;
}

extern "C" int main()
{
	std::string cheat_dll_name;

	std::cout << "Enter the name of the DLL to inject: " << std::endl;
	std::cin >> cheat_dll_name;


	int32_t target_pid;

	std::cout << "Enter the pid of the target process: " << std::endl;
	std::cin >> target_pid;

	uint8_t* cheat_dll_raw = NULL;

	auto image_size = Util::LoadFileIntoMemory(cheat_dll_name.c_str(), &cheat_dll_raw);

	InjectDLLBytes(target_pid, cheat_dll_raw, ENTRYPOINT_NAME, HOST_DLL_PATH);

	std::cin.get();
}
