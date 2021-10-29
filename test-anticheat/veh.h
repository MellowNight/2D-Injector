#pragma once
#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

namespace veh 
{
    LONG WINAPI be_handler(struct _EXCEPTION_POINTERS* ExceptionInfo);
    void set_veh(uintptr_t func);
    void remove_veh(uintptr_t func);
}