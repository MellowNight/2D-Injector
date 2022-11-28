#include "be_logging.h"
#include "hooks.h"
#include "logging.h"
#include "disassembly.h"
#include "dbg_symbols.h"

#define LOG_FILE "C:\\Users\\user123\\Desktop\\testing_drivers\\test_logs.txt"

struct AddressInfo
{
	AddressInfo(void* _address) : address(_address)
	{
		symbol = Symbols::GetSymFromAddr((uintptr_t)address);

		dll_name_address.second = Utils::ModuleFromAddress((uintptr_t)address, &dll_name_address.first);
	}

	void Print()
	{
		if (!symbol.empty())
		{
			Logger::Get()->Print(
				COLOR_ID::none, "%wZ!%s (0x%08x) \n", &dll_name_address.first, symbol.c_str(), address);
		}
		else if (dll_name_address.second)
		{
			Logger::Get()->Print(COLOR_ID::none, "%wZ + 0x%08x \n", 
				&dll_name_address.first, (uintptr_t)address - (uintptr_t)dll_name_address.second);
		}
		else
		{
			Logger::Get()->Print(COLOR_ID::none, "0x%08x \n", address);
		}
	}

	void* address;
	std::pair<UNICODE_STRING, void*> dll_name_address;
	std::string	symbol;
};


/*	log out-of-module function calls and jmps		*/

void ExecuteHook(GeneralRegisters* registers, void* return_address, void* o_guest_rip)
{
	AddressInfo retaddr_info = { return_address };
	AddressInfo rip_info = { o_guest_rip };

	Logger::Get()->Print(COLOR_ID::green, "[EXECUTE]	");  
	Logger::Get()->Print(COLOR_ID::none, "return address = ");

	retaddr_info.Print();

	Logger::Get()->Print(COLOR_ID::green, "[EXECUTE]	");
	Logger::Get()->Print(COLOR_ID::none, "RIP = ", o_guest_rip);

	rip_info.Print();

	Logger::Get()->Print(COLOR_ID::none, "\n\n");
}


/*	log reads and writes		*/

void ReadWriteHook(GeneralRegisters* registers, void* o_guest_rip)
{
	AddressInfo rip_info = { o_guest_rip };

	ZydisDecodedOperand operands[5] = { 0 };

	auto instruction = Disasm::Disassemble((uint8_t*)o_guest_rip, operands);

	Logger::Get()->Print(COLOR_ID::magenta, "[READ/WRITE]	");
	Logger::Get()->Print(COLOR_ID::none, "RIP = ", o_guest_rip);

	rip_info.Print();

	ZydisRegisterContext context;

	Disasm::MyRegContextToZydisRegContext(registers, &context, o_guest_rip);

	for (int i = 0; i < instruction.operand_count_visible; ++i)
	{
		auto mem_target = Disasm::GetMemoryAccessTarget(instruction, &operands[i], (ZyanU64)o_guest_rip, &context);

		if (operands[i].actions & ZYDIS_OPERAND_ACTION_MASK_WRITE)
		{
			Logger::Get()->Print(COLOR_ID::none, " write - ", mem_target);
		}
		else if (operands[i].actions & ZYDIS_OPERAND_ACTION_MASK_READ)
		{
			Logger::Get()->Print(COLOR_ID::none, " read - ", mem_target);
		}
	}

	Logger::Get()->Print(COLOR_ID::none, "\n");
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

	auto kernel32 = (uint8_t*)GetModuleHandleA("kernel32.dll");

	ForteVisor::DenySandboxMemAccess(kernel32 + 0x1000);
}

void StartBELogger()
{
	Disasm::Init();
	Symbols::Init();

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
