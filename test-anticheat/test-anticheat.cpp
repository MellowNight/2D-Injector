#include "veh.h"
#include "utils.h"

int main()
{
    Utils::log("test_anticheat started \n");

    auto kernel_base_dll = GetModuleHandle(L"kernel32.dll");
    auto is_bad_read = GetProcAddress(kernel_base_dll, "IsBadReadPtr");
    veh::set_veh((uintptr_t)is_bad_read);
    Utils::log("set_veh called \n");

    while (1)
    {

    }
}