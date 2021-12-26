// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "communicate.h"
#include "manual_map.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) int InjectDllBytes(BYTE* dll_bytes, int file_size, wchar_t* target_process, char* entrypoint_name)
{
    MessageBox(NULL, L"InjectDllBytes called", L"InjectDllBytes", MB_OK);
    
    Driver::Init();

    auto PeHdr = PeHeader(dll_bytes);

    auto real_virtual_size = PeHdr->OptionalHeader.SizeOfImage;	// get real size of DLL

    int target = GetProcId(target_process);


    /*	create a section that will be mapped into the target process	*/

    SECURITY_ATTRIBUTES oSecurityAttributes;
    SECURITY_DESCRIPTOR oSecurityDescriptor;

    InitializeSecurityDescriptor(&oSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&oSecurityDescriptor, TRUE, NULL, FALSE);
    SetSecurityDescriptorGroup(&oSecurityDescriptor, NULL, FALSE);
    SetSecurityDescriptorSacl(&oSecurityDescriptor, FALSE, NULL, FALSE);

    oSecurityAttributes.nLength = sizeof(oSecurityAttributes);
    oSecurityAttributes.lpSecurityDescriptor = &oSecurityDescriptor;
    oSecurityAttributes.bInheritHandle = FALSE;


    wchar_t section_name[] = L"Global\\r6iinternal";

    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        &oSecurityAttributes,
        PAGE_READWRITE,
        0,
        real_virtual_size + 0x1000,	// 0x1000 extra for parameters to pass to DLL
        section_name
    );

    std::cout << "hMapFile" << hMapFile << "GetLastError " << GetLastError() << std::endl;
    std::cout << "real_virtual_size " << real_virtual_size << std::endl;

    /* maps the section to target process, enables Execute on page */

    auto section_base = Driver::MapMem(
        target, real_virtual_size + 0x1000, section_name
    );

    /*  map our DLL 0x1000 bytes into the section to make space for parameters */
    auto map_base = section_base + 0x1000;

    auto image = MapImage(dll_bytes, file_size, map_base, target);

    Driver::WriteMem(target, map_base, (BYTE*)image, real_virtual_size);


    auto entry_offset = (uintptr_t)GetProcAddress((HMODULE)dll_bytes, entrypoint_name) - (uintptr_t)dll_bytes;

    Driver::StartThread(map_base + entry_offset, target, section_base, real_virtual_size);

    Sleep(2000);

    delete image;

    Driver::ExitDriver();

    return 0;
}