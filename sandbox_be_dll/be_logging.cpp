#include "be_logging.h"
#include "hooks.h"
#include "logging.h"

#define LOG_FILE "C:\\Users\\user123\\Desktop\\testing_drivers\\test_logs.txt"

void ReadWriteExecuteHook(void* registers, void* return_address)
{
	Logger::Get()->Print(COLOR_ID::magenta, "return_address 0x%p \n", return_address);
}

void SandboxRegion(uintptr_t base, uintptr_t size)
{
	ForteVisor::RegisterSandboxHandler(ReadWriteExecuteHook);

	for (auto offset = base; offset < base + size; offset += PAGE_SIZE)
	{
		Utils::TriggerCOWAndPageIn((uint8_t*)offset);

		ForteVisor::SandboxPage((uintptr_t)offset, (uint8_t*)offset, PAGE_SIZE);
	}
}

void StartBELogger()
{
	Logger::Get()->Print(COLOR_ID::magenta, "StartBELogger() \n");
	Sleep(500000);

	/*	Only allow BEClient pages to execute in 3rd NCR3 	*/

	uintptr_t beclient = NULL;

	while (!beclient)
	{
		Sleep(500);
		beclient = (uintptr_t)GetModuleHandle(L"BEService.exe");
	}

	SandboxRegion(beclient, PeHeader(beclient)->OptionalHeader.SizeOfImage);
}
