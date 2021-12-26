// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "be_bypass.h"
#include "utils.h"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    MessageBoxA(NULL, "Hello world!", "Hello World!", NULL);

    return TRUE;
}

