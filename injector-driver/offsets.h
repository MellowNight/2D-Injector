#pragma once
#include "util.h"

namespace R6
{
	const int VMT_PRESENT_IDX = 8;
	enum OFFSET
	{
		// game d3d manager, swapchain, for present hook
		game_d3d = 0x790,
		swapchain = 0x28,
	};

	/*	48 8B 05 ? ? ? ? 48 85 C0 75 28 48 89 CA	RELATIVE_ADDR(address, 3, 7);	*/
	UCHAR* const camera_manager_sig = (UCHAR*)"\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x28\x48\x89\xCA";
};

namespace nt
{
	const uintptr_t vad_root = 0x7d8;
}