#include "be_logging.h"
#include "hooks.h"
#include "logging.h"

#define LOG_FILE "C:\\Users\\user123\\Desktop\\testing_drivers\\test_logs.txt"

void ExecuteHook(GeneralRegisters* registers, void* return_address, void* o_guest_rip)
{
	Logger::Get()->Print(COLOR_ID::green, "[EXECUTE]	");  
	Logger::Get()->Print(COLOR_ID::none, "return_address 0x%p \n", return_address);

	Logger::Get()->Print(COLOR_ID::green, "[EXECUTE]	");
	Logger::Get()->Print(COLOR_ID::none, "rip = 0x%p \n", o_guest_rip);
}

void ReadWriteHook(GeneralRegisters* registers, void* o_guest_rip)
{
	Logger::Get()->Print(COLOR_ID::magenta, "[READ/WRITE]	");
	Logger::Get()->Print(COLOR_ID::none, "rip = 0x%p \n", o_guest_rip);
}

void SandboxRegion(uintptr_t base, uintptr_t size)
{
	ForteVisor::RegisterSandboxHandler(ForteVisor::readwrite_handler, ReadWriteHook);
	ForteVisor::RegisterSandboxHandler(ForteVisor::execute_handler, ExecuteHook);

	for (auto offset = base; offset < base + size; offset += PAGE_SIZE)
	{
		Utils::TriggerCOWAndPageIn((uint8_t*)offset);

		ForteVisor::SandboxPage((uintptr_t)offset, NULL);
	}

	auto kernel32 = GetModuleHandleA("kernel32.dll");

	ForteVisor::DenySandboxMemoryAccess(kernel32 + 0x1000);
}

void StartBELogger()
{
	Logger::Get()->Print(COLOR_ID::magenta, "StartBELogger() \n");

	/*	Only allow BEClient pages to execute in 3rd NCR3 	*/

	uintptr_t beclient = NULL;

	while (!beclient)
	{
		Sleep(500);
		beclient = (uintptr_t)GetModuleHandle(L"BEService.exe");
	}

	SandboxRegion(beclient, PeHeader(beclient)->OptionalHeader.SizeOfImage);
}
