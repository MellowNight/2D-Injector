#include "disassembly.h"

namespace Disasm
{
	ZydisDecodedInstruction Disassemble(uint8_t* instruction)
	{
		ZydisDecodedInstruction zydis_insn;
		ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT_VISIBLE];

		ZydisDecoderDecodeFull(
			&zydis_decoder,
			instruction, 16,
			&zydis_insn,
			operands,
			ZYDIS_MAX_OPERAND_COUNT_VISIBLE,
			ZYDIS_DFLAG_VISIBLE_OPERANDS_ONLY
		);

		return zydis_insn;
	}

	/*	Gets total instructions length closest to byte_length	*/
	int	LengthOfInstructions(void* address, int byte_length)
	{
		int insns_len = 0;
		for (insns_len = 0; insns_len < byte_length;)
		{
			int cur_insn_len = Disassemble((uint8_t*)address + insns_len).length;
			insns_len += cur_insn_len;
		}

		return insns_len;
	}

	int Init()
	{
		return ZydisDecoderInit(&zydis_decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
	}
};