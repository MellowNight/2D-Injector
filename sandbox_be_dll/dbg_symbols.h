#pragma once
#include <string>

#pragma comment(lib, "dbghelp.lib")

namespace Symbols
{
    void Init();

    uintptr_t LoadSymbolsForModule(
        std::string image_name,
        uintptr_t mapped_base,
        uintptr_t image_size
    );

    std::string GetSymFromAddr(
        uintptr_t addr
    );
}