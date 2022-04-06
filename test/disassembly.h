#pragma once
#include "includes.h"

namespace Disasm
{
	extern ZydisDecoder zydis_decoder;

	ZydisDecodedInstruction Disassemble(uint8_t* instruction);

	/*	Gets total instructions length closest to byte_length	*/
	int	LengthOfInstructions(void* address, int byte_length);

	int Init();
};