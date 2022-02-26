#pragma once
#include "util.h"

typedef __int64(__fastcall* pfnWppTraceMessage)(__int64, __int64, __int64, __int64, __int64);

extern pfnWppTraceMessage o_WppTraceMessage;
extern uintptr_t ioctl_ret_addr;

void DiskHookHandler();