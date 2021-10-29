//#pragma once
//#include "Zydis/Zydis.h"
//#include "Zycore/Zycore.h"
//#include "Zydis/Decoder.h"
//
//ZydisDecoder zy_decoder;
//
//ZydisDecodedInstruction	disasm(ULONG64 address)
//{
//    ZydisDecodedInstruction insn;
//    ZydisDecoderDecodeBuffer(&zy_decoder, (void*)address, 20,
//        &insn);
//
//    return insn;
//}
//
//int GetInsnLen(BYTE* Instruction)
//{
//    ZydisDecodedInstruction insn;
//    ZydisDecoderDecodeBuffer(&zy_decoder, Instruction, 20,
//        &insn);
//
//    return insn.length;
//}
//
///*	Gets total instructions length closest to BytesLength	*/
//int	LengthOfInsns(PVOID	address, int BytesLength)
//{
//    int InstructionLen = 0;
//    for (InstructionLen = 0; InstructionLen < BytesLength;)
//    {
//        int CurInstructionLen = GetInsnLen((BYTE*)address + InstructionLen);
//        InstructionLen += CurInstructionLen;
//    }
//
//    return InstructionLen;
//}
//
//void InitZydis()
//{
//    ZydisDecoderInit(&zy_decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
//}