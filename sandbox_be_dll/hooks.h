#pragma once
#include "hook_shellcode.h"
//
//Hooks::JmpRipCode get_proc_address_hk;
//
//LPVOID GetProcAddress_hk(_In_ HMODULE hModule, _In_ LPCSTR lpProcName)
//{
//    uintptr_t retaddy = (uintptr_t)_ReturnAddress();
//    MEMORY_BASIC_INFORMATION mbi{ 0 };
//    size_t return_length{ 0 };
//
//    if (VirtualQuery((HANDLE)-1, (PVOID)retaddy, MemoryBasicInformation, &mbi, sizeof(mbi), &return_length) == 0)
//    {
//        if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_EXECUTE_READWRITE && mbi.RegionSize > 0x1000)
//        {
//            if (std::find(dumped_shellcodes.begin(), dumped_shellcodes.end(), (uintptr_t)mbi.AllocationBase) == dumped_shellcodes.end())
//            {
//                std::string to_stream = "C:\\Users\\weak\\Desktop\\r6dmps\\shellcode\\" + std::to_string((uintptr_t)mbi.BaseAddress) + ".dat"; //R6 has no admin rights so dont use paths like C:\file.dat
//                printf("Call from be-shellcode dumping: %s\n", to_stream.c_str());
//                uintptr_t possible_shellcode_start = utils::scanpattern((uintptr_t)mbi.BaseAddress, mbi.RegionSize, "4C 89");
//                printf("Possible entry-rva: %x\n", possible_shellcode_start - (uintptr_t)mbi.BaseAddress);
//                utils::CreateFileFromMemory(to_stream, (char*)mbi.BaseAddress, mbi.RegionSize);
//                dumped_shellcodes.push_back((uintptr_t)mbi.AllocationBase);
//            }
//        }
//    }
//    return static_cast<decltype(&GetProcAddress_hk)>(get_proc_address_hk.original_bytes)(hModule, lpProcName);
//}
//
//Hooks::JmpRipCode loadlibrary_hk;
//
//HMODULE LoadLibrary_hk(LPCSTR lpLibFileName)
//{
//    if (!strcmp(lpLibFileName, "BEClient2.dll"))
//    {
//
//    }
//   
//    return static_cast<decltype(&LoadLibrary_hk)>(loadlibrary_hk.original_bytes)(lpLibFileName);
//}