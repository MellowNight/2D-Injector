#include "communicate.h"
#include "manual_map.h"
#include "utils.h"
#include <iostream>

#define TARGET L"ShooterGame-Win64-Shipping.exe"
#define ENTRYPOINT_NAME "HookEntryPoint"

extern "C" int main()
{
	system("C:\\Users\\lolxd\\Downloads\\KsDumper\\Driver\\kdmapper C:\\Users\\lolxd\\source\\repos\\AMD-Hypervisor\\x64\\Release\\AMD-Hypervisor.sys");
	system("C:\\Users\\lolxd\\Downloads\\KsDumper\\Driver\\kdmapper C:\\Users\\lolxd\\source\\repos\\BE-injector\\x64\\Release\\injector-driver.sys");
	
	std::string cheat_dll_name;

	std::cout << "Enter the name of the DLL to inject: " << std::endl;
	std::cin >> cheat_dll_name;

	Driver::Init();

	auto target_pid = Util::GetProcId(TARGET);

	uint8_t* cheat_dll_raw = NULL;

	auto image_size = Util::LoadFileIntoMemory(cheat_dll_name.c_str(), &cheat_dll_raw);

	auto image_real_size = PeHeader(cheat_dll_raw)->OptionalHeader.SizeOfImage;
	std::cout << std::hex << " image real size 0x" << image_real_size << std::endl;
	auto alloc_base = Driver::AllocateMemory(target_pid, image_real_size + PAGE_SIZE);
	std::cout << std::hex << " alloc_base 0x" << alloc_base << std::endl;

	auto cheat_base = alloc_base + PAGE_SIZE;


	uint8_t* cheat_mapped = NULL;

	PE::RemapImage(cheat_dll_raw, &cheat_mapped, target_pid, cheat_base);

	Driver::WriteMem(target_pid, cheat_base, cheat_mapped, image_real_size);

	auto entry_point = (uintptr_t)PE::GetExport((uintptr_t)cheat_mapped, ENTRYPOINT_NAME);
	std::cout << std::hex << " entry_point 0x" << entry_point + cheat_base << std::endl;
	Driver::InvokeRemoteFunc(entry_point + cheat_base, target_pid, alloc_base, image_real_size);

	Driver::ExitDriver();
}
