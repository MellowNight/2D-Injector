#pragma once
#include "includes.h"

namespace Disasm
{
	static ZydisDecoder zydis_decoder;

	ZydisDecodedInstruction Disassemble(
		uint8_t* instruction,
		ZydisDecodedOperand* operands
	);

	/*	Gets total instructions length closest to byte_length	*/
	int	LengthOfInstructions(
		void* address,
		int byte_length
	);

	void MyRegContextToZydisRegContext(
		GuestRegisters* guest_regs,
		ZydisRegisterContext* context,
		void* o_guest_rip
	);

	ZyanU64 GetMemoryAccessTarget(
		ZydisDecodedInstruction& instruction,
		ZydisDecodedOperand* operand,
		ZyanU64 runtime_address,
		ZydisRegisterContext* registers
	);

	void ForEachInstruction(
		uint8_t* start,
		uint8_t* end,
		void(*Callback)(uint8_t* insn_addr, ZydisDecodedInstruction instruction)
	);

	int Init();
};