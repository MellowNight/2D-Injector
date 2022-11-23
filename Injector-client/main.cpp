#include "communicate.h"
#include "manual_map.h"
#include "utils.h"
#include <iostream>
#include "security.h"

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

	auto image_real_size = PeHeader(dll_raw)->OptionalHeader.SizeOfImage;

	auto alloc_size = image_real_size + PAGE_SIZE;

	auto alloc_base = Driver::AllocateMemory(pid, alloc_size);
	
	Driver::ProtectMemory(pid, alloc_base, alloc_size, PAGE_EXECUTE_READWRITE);

	auto cheat_base = alloc_base + PAGE_SIZE;

	/*	local copy of cheat dll	*/

	uint8_t* cheat_mapped = NULL;

	PE::RemapImage(dll_raw, &cheat_mapped, pid, cheat_base);

	std::cout << std::hex << " cheat_base 0x" << cheat_base << std::endl;

	auto pe_hdr = PeHeader(cheat_mapped);

	/*	write the dll	*/

	Driver::WriteMem(pid, cheat_base, cheat_mapped, image_real_size);

	Driver::HideMem(pid, (uint8_t*)alloc_base, alloc_size, PAGE_READWRITE);

	auto section = (IMAGE_SECTION_HEADER*)(pe_hdr + 1);

	auto entry_point = (uintptr_t)PE::GetExport((uintptr_t)cheat_mapped, entrypoint_name);

	std::cout << std::hex << " entry_point 0x" << entry_point + cheat_base << std::endl;

	Driver::InvokeRemoteFunc(entry_point + cheat_base, pid, alloc_base, image_real_size);

	return 0;
}

#define ENTRYPOINT_NAME "CreateUserThreadEntry"

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
