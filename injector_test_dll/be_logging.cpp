#include "be_logging.h"
#include "hooks.h"

void SandboxRegion(uintptr_t base, uintptr_t size)
{
	for (auto offset = base; offset < base + size; offset += PAGE_SIZE)
	{
		Utils::TriggerCOWAndPageIn((uint8_t*)offset);

		ForteVisor::SetNptHook((uintptr_t)offset, (uint8_t*)offset, PAGE_SIZE, NCR3_DIRECTORIES::sandbox, NULL);
	}

}

void StartBELogger()
{
	MessageBoxA(NULL, "StartBELogger() Hello! \n", "test 3", NULL);

	/*	Only allow BEClient pages to execute in 3rd NCR3 	*/

	uintptr_t beclient = NULL;

	while (!beclient)
	{
		Sleep(500);
		beclient = (uintptr_t)GetModuleHandle(L"BEClient.dll");
	}

	SandboxRegion(beclient, PeHeader(beclient)->OptionalHeader.SizeOfImage);
}
