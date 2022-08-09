#include "communicate.h"
#include "manual_map.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include "security.h"

uintptr_t LoadSignedHostDLL(int32_t pid)
{
	/*	use Overwolf's DLL to carry our cheat	*/

	auto overwolf_name = L"C:\\Users\\user123\\Desktop\\OWClient.dll";

	auto overwolf_path_size = wcslen(overwolf_name) + 1;

	auto loadlib_parameter = Driver::AllocateMemory(pid, overwolf_path_size);

	auto loadlib_address = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");


	/* Executing LoadLibrary using usermode APCs in target process */

	THREADENTRY32 thread_entry;

	std::vector<uint32_t> target_threads;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, 0);

	if (Thread32First(snapshot, &thread_entry)) 
	{
		do 
		{
			if (thread_entry.th32OwnerProcessID == pid) 
			{
				target_threads.push_back(thread_entry.th32ThreadID);
			}
		} 
		while (Thread32Next(snapshot, &thread_entry));
	}

	for (DWORD thread_id : target_threads)
	{
		auto hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);

		if (hthread == NULL)
		{
			std::cout << "[ :( ] Failed to OpenThread. GetLastError = 0x" << std::hex << GetLastError() << std::endl;
		}

		auto result = QueueUserAPC((PAPCFUNC)loadlib_address, hthread, loadlib_parameter);
		
		CloseHandle(hthread);
	}

	return Driver::GetModuleBase(L"OWClient.dll", pid);
}

extern "C" __declspec(dllexport) int InjectDLLBytes(int32_t pid, uint8_t* dll_raw, const char* entrypoint_name)
{
	if (*(int32_t*)dll_raw != INJECTOR_PASSWORD)
	{
		SecurityViolation();
	}
	else
	{
		*(int32_t*)dll_raw = 0x5A4D;
	}

	Driver::Init();

	/* Load the DLL we will be stealthily mapping our cheat on top of */

	auto host_dll_base = LoadSignedHostDLL(pid);

	auto cheat_base = host_dll_base + PAGE_SIZE;

	auto image_real_size = PeHeader(dll_raw)->OptionalHeader.SizeOfImage;

	auto alloc_size = image_real_size + PAGE_SIZE;

	/*	local copy of cheat dll	*/

	uint8_t* cheat_mapped = NULL;

	PE::RemapImage(dll_raw, &cheat_mapped, pid, cheat_base);

	std::cout << std::hex << " cheat_base 0x" << host_dll_base + PAGE_SIZE << std::endl;


	/*	write the dll	*/

	Driver::WriteMem(pid, cheat_base, cheat_mapped, image_real_size);



	/*	hide .text	*/

	auto section = (IMAGE_SECTION_HEADER*)(PeHeader(cheat_mapped) + 1);

	//for (int i = 0; i < pe_hdr->FileHeader.NumberOfSections; ++i)
	//{
	//	if (section[i].Characteristics & IMAGE_SCN_CNT_CODE)
	//	{
	//		std::cout << section[i].Name << std::endl;

	//		/*	randomize .text bytes	*/

	//		for (auto byte = cheat_mapped + section[i].VirtualAddress;
	//			byte < cheat_mapped + section[i].VirtualAddress + section[i].SizeOfRawData;
	//			byte += 8)
	//		{ 
	//			uint64_t value = 0xCCFFDDEE; 
	//			Driver::WriteMem(pid, cheat_base + (byte - cheat_mapped), (BYTE*)&value, 8);
	//		}

	//		for (auto page = cheat_mapped + section[i].VirtualAddress;
	//			page < cheat_mapped + section[i].VirtualAddress + section[i].SizeOfRawData;
	//			page += PAGE_SIZE)
	//		{
	//			Driver::SetNptHook(pid, PAGE_SIZE, cheat_base + (page - cheat_mapped), page);
	//		}
	//	}
	//}


	auto entry_point = (uintptr_t)PE::GetExport((uintptr_t)cheat_mapped, entrypoint_name);

	std::cout << std::hex << " entry_point 0x" << entry_point + cheat_base << std::endl;

	Driver::InvokeRemoteFunc(entry_point + cheat_base, pid, host_dll_base, image_real_size);

	Driver::ExitDriver();

	return 0;
}

#define ENTRYPOINT_NAME "HookEntryPoint"

extern "C" int main()
{
	std::string cheat_dll_name;

	std::cout << "Enter the name of the DLL to inject: " << std::endl;
	std::cin >> cheat_dll_name;


	std::wstring target_process;

	std::cout << "Enter the name of the target process: " << std::endl;
	std::wcin >> target_process;

	auto target_pid = Util::GetProcId(target_process.c_str());

	uint8_t* cheat_dll_raw = NULL;

	auto image_size = Util::LoadFileIntoMemory(cheat_dll_name.c_str(), &cheat_dll_raw);

	InjectDLLBytes(target_pid, cheat_dll_raw, ENTRYPOINT_NAME);

	std::cin.get();
}
