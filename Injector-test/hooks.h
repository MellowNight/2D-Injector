#pragma once
#include "mono.h"
#include "disasm.h"
#include "game_math.h"
#include "physics_unity.h"
#include "player_unity.h"
#include "silent_aim.h"
#include "renderer_unity.h"

enum HOOK_MODE
{
	ept_hook = 1,
	npt_hook = 2,
	normal_hook = 3,
};

void HookWithHypervisor(void* hook, HOOK_MODE hook_mode, int32_t npthook_tag = NULL);

#define DECLARE_HOOK( shellcode_type, ret_type, name, args ) inline Hooks::##shellcode_type* name; ret_type hk_##name args;
#define APPLY_HOOK( shellcode_type, name, function_address, hook_mode )  Hooks::name = new Hooks::##shellcode_type((uintptr_t)function_address, (uintptr_t)Hooks::hk_##name); HookWithHypervisor((void*)Hooks::name, hook_mode);

namespace Hooks
{
	DECLARE_HOOK(JmpRaxCode, char, call_veh, (IN PEXCEPTION_RECORD ExceptionRecord, IN PCONTEXT Context, unsigned int a3));
	DECLARE_HOOK(JmpRaxCode, BOOL, is_bad_read_ptr, (void* lp, UINT_PTR ucb));
};