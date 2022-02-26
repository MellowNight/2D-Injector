#include "veh.h"

int main()
{
    auto kernel_base_dll = GetModuleHandle(L"kernel32.dll");
    auto is_bad_read = GetProcAddress(kernel_base_dll, "IsBadReadPtr");

    while (1)
    {
        Sleep(5000);

        veh::remove_veh((uintptr_t)is_bad_read);
        veh::set_veh((uintptr_t)is_bad_read);
    }
}