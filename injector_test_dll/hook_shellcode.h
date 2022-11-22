#pragma once
#include "utils.h"
#include "includes.h"

#define DEFAULT_HOOK_MODE (HOOK_MODE)2 // 1 == INTEL | 2 == AMD | 3 == normal

extern "C" size_t __fastcall LDE(const void* lpData, unsigned int size);

namespace Hooks
{
	struct JmpRipCode
	{
		uintptr_t fn_address;
		uintptr_t target_address;

		size_t hook_size;
		size_t orig_bytes_size;
		void* original_bytes;
		uint8_t* hook_code;

		JmpRipCode(uintptr_t hook_address, uintptr_t jmp_target)
		{
			fn_address = hook_address;
			target_address = jmp_target;

			for (hook_size = 0; hook_size < 14; hook_size += LDE((PCHAR)hook_address + hook_size, 64))
			{
				// Get the full size of instructions necessary to copy
			}

			orig_bytes_size = hook_size + 14;      /*  because orig_bytes includes jmp back code   */

			auto jmp_back_location = hook_address + hook_size;

			char jmp_rip[15] = "\xFF\x25\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC";

			original_bytes = (uint8_t*)VirtualAlloc(NULL, orig_bytes_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			memcpy(original_bytes, (void*)hook_address, hook_size);
			memcpy((uint8_t*)original_bytes + hook_size, jmp_rip, 14);
			memcpy((uint8_t*)original_bytes + hook_size + 6, &jmp_back_location, sizeof(uintptr_t));

			hook_code = (uint8_t*)VirtualAlloc(NULL, hook_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			memcpy(jmp_rip + 6, &jmp_target, sizeof(uintptr_t));
			memcpy(hook_code, jmp_rip, 14);
		}
	};

	struct JmpRaxCode
	{
		uintptr_t fn_address;
		uintptr_t target_address;

		size_t hook_size;
		size_t orig_bytes_size;
		void* original_bytes;
		uint8_t* hook_code;

		JmpRaxCode(uintptr_t hook_address, uintptr_t jmp_target)
		{
			fn_address = hook_address;
			target_address = jmp_target;

			for (hook_size = 0; hook_size < 12; hook_size += LDE((PCHAR)hook_address + hook_size, 64))
			{
				// Get the full size of instructions necessary to copy
			}

			orig_bytes_size = hook_size + 12;      /*  because orig_bytes includes jmp back code   */

			auto jmp_back_location = hook_address + hook_size;

			char jmp_rax[13] = "\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xE0";

			original_bytes = (uint8_t*)VirtualAlloc(NULL, orig_bytes_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			memcpy(original_bytes, (void*)hook_address, hook_size);
			memcpy((uint8_t*)original_bytes + hook_size, jmp_rax, 12);
			memcpy((uint8_t*)original_bytes + hook_size + 2, &jmp_back_location, sizeof(uintptr_t));

			hook_code = (uint8_t*)VirtualAlloc(NULL, hook_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			memcpy(jmp_rax + 2, &jmp_target, sizeof(uintptr_t));
			memcpy(hook_code, jmp_rax, 12);
		}
	};

	struct JmpRelative
	{
		uintptr_t fn_address;
		uintptr_t target_address;

		size_t hook_size;
		size_t orig_bytes_size;
		uint8_t* original_bytes;
		uint8_t* hook_code;
		uintptr_t jmp_back_location;

		JmpRelative(uintptr_t hook_address, uintptr_t jmp_target)
		{
			fn_address = hook_address;
			target_address = jmp_target;

			for (hook_size = 0; hook_size < 5; hook_size += LDE((PCHAR)hook_address + hook_size, 64))
			{
				// Get the full size of instructions necessary to copy
			}

			orig_bytes_size = hook_size + 12;      /*  because orig_bytes includes jmp back code   */

			jmp_back_location = hook_address + hook_size;

			char jmp_rel[6] = "\xE9\x00\x00\x00\x00";

			int32_t relative_offset = jmp_target - hook_address - 5;

			*(int32_t*)(jmp_rel + 1) = relative_offset;


			/*	setup Jmp Back code	*/

			char jmp_rax[13] = "\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xE0";

			original_bytes = (uint8_t*)VirtualAlloc(NULL, orig_bytes_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			memcpy(original_bytes, (void*)hook_address, hook_size);
			memcpy((uint8_t*)original_bytes + hook_size, jmp_rax, 12);
			memcpy((uint8_t*)original_bytes + hook_size + 2, &jmp_back_location, sizeof(uintptr_t));

			hook_code = (uint8_t*)VirtualAlloc(NULL, hook_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);


			/*	final hook shellcode setup	*/

			memcpy(hook_code, jmp_rel, 5);
		}
	};
};