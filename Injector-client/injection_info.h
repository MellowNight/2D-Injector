#pragma once
#include "includes.h"

struct DllParams
{
	uint32_t header;
	uintptr_t dll_base;
	uint32_t dll_size;
	uintptr_t payload_dll_base;
	uint32_t payload_dll_size;

	int o_present_bytes_size;
	uint8_t original_present_bytes[20];
};

enum INJECTOR_CONSTANTS
{
	mapped_dll_header = 0x12345678,
};

#define HOST_DLL_PATH	"C:\\Program Files (x86)\\Overwolf\\0.220.0.1\\OWClient.dll"
#define HOST_DLL_NAME	"OWClient.dll"
#define ENTRYPOINT_NAME	"HookEntryPoint"

#define FLS_CALLBACK_PATCH_OFFSET 0x1B7C10
#define ACRT_LOCALE_RELEASE_OFFSET 0x1C47BC
