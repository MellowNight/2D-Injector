#include "communicate.h"
#include "manual_map.h"
#include "utils.h"

#define TARGET L"RainbowSix.exe"
#define ENTRYPOINT_NAME "HookEntryPoint"

extern "C" int main()
{
	std::string cheat_dll_name;

	std::cout << "Enter the name of the DLL to inject: " << std::endl;
	std::cin >> cheat_dll_name;

	Driver::Init();

	auto target_pid = Util::GetProcId(TARGET);

	uint8_t* cheat_dll_raw = NULL;

	auto image_size = Util::LoadFileIntoMemory(cheat_dll_name.c_str(), &cheat_dll_raw);

	auto image_real_size = PeHeader(cheat_dll_raw)->OptionalHeader.SizeOfImage;

	auto alloc_base = Driver::AllocateMemory(target_pid, image_real_size + PAGE_SIZE);
	
	auto cheat_base = alloc_base + PAGE_SIZE;


	uint8_t* cheat_mapped = NULL;

	PE::RemapImage(cheat_dll_raw, &cheat_mapped, target_pid, cheat_base);

	auto entry_point = (uintptr_t)PE::GetExport((uintptr_t)cheat_mapped, ENTRYPOINT_NAME);

	Driver::InvokeRemoteFunc(entry_point + cheat_base, target_pid, alloc_base, image_real_size);
	

	Driver::ExitDriver();

	std::cin.get();
}
