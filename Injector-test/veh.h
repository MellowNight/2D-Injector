#pragma once
#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include "nt.h"
#include "beshellcode.h"
namespace veh {
    LONG WINAPI be_handler(struct _EXCEPTION_POINTERS* ExceptionInfo);
    void add_func(uintptr_t func);

}