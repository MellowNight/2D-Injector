#pragma once
#include "util.h"
#include "offsets.h"

bool SetR6PresentHook(uintptr_t base, PVOID hook_handler, PVOID* o_present)
{
	uintptr_t camera_manager;
	utils::BBScan(".text", R6::camera_manager_sig, 0x00, 15, &camera_manager, (PVOID)base);

	camera_manager = RELATIVE_ADDR(camera_manager, 3, 7);
	camera_manager = *(uintptr_t*)camera_manager;

	auto chain1 = *(uintptr_t*)(camera_manager + R6::OFFSET::game_d3d);
	auto swapchain = *(uintptr_t*)(chain1 + R6::OFFSET::swapchain);

	// Get Present's address out of game's swapchain vmt
	auto vmt = *(uintptr_t**)swapchain;
	*o_present = decltype(o_present)(vmt[R6::VMT_PRESENT_IDX]);

	DWORD old_protect;
	DWORD old_protect2;

	// shadow VMT

	auto size = 512 * sizeof(PVOID);
	PVOID* new_vmt = NULL;

	auto status = NtAllocateVirtualMemory(
		(HANDLE)-1, (PVOID*)&new_vmt, NULL, &size,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);

	memcpy(new_vmt, vmt, 0x1000 * sizeof(PVOID));

	new_vmt[R6::VMT_PRESENT_IDX] = hook_handler;

	*(PVOID**)swapchain = new_vmt;

	return true;
}