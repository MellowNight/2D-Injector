#pragma once
#include "util.h"
#include "offsets.h"

struct InjectInfo
{
	int   header = 0x1234;
	void*** swapchain_ptr;
	void** o_swapchain_vmt;
	uintptr_t section_size;
};

bool SetR6PresentHook(uintptr_t base, PVOID hook_handler, InjectInfo* info)
{
	uintptr_t camera_manager;
	utils::BBScan(".text", R6::camera_manager_sig, 0x00, 15, &camera_manager, (PVOID)base);

	camera_manager = RELATIVE_ADDR(camera_manager, 3, 7);
	camera_manager = *(uintptr_t*)camera_manager;

	auto chain1 = *(uintptr_t*)(camera_manager + R6::OFFSET::game_d3d);
	auto swapchain = *(uintptr_t*)(chain1 + R6::OFFSET::swapchain);

	// Get Present's address out of game's swapchain vmt
	auto vmt = *(uintptr_t**)swapchain;

	// shadow VMT

	void** new_vmt = (void**)0;
	SIZE_T size = 512;

	auto status = ZwAllocateVirtualMemory(NtCurrentProcess(),
		(void**)&new_vmt,
		0,
		&size,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	);

	memcpy(new_vmt, vmt, 0x1000);

	new_vmt[R6::VMT_PRESENT_IDX] = hook_handler;

	*(PVOID**)swapchain = new_vmt;

	info->header = 0x1234;
	info->o_swapchain_vmt = (void**)vmt;
	info->swapchain_ptr = (void***)swapchain;

	return true;
}