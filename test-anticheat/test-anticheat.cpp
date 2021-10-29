#include "veh.h"

int main()
{
    auto kernel_base_dll = GetModuleHandle(L"kernelbase.dll");
    auto sleep_ex = GetProcAddress(kernel_base_dll, "SleepEx");

    while (1)
    {
        Sleep(5000);

        veh::remove_veh((uintptr_t)sleep_ex);
        veh::set_veh((uintptr_t)sleep_ex);
    }
}