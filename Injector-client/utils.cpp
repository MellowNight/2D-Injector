#pragma once
#include "includes.h"
#include "communicate.h"

#define StrToWStr(s) (std::wstring(s, &s[strlen(s)]).c_str())

namespace Util
{
	size_t LoadFileIntoMemory(const char* path, uint8_t** buffer)
	{
		auto file_handle = CreateFileA(
			path, GENERIC_ALL, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
		);

		auto size = GetFileSize(file_handle, NULL);

		*buffer = (uint8_t*)VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		SetFilePointer(file_handle, 0, 0, 0);

		uint32_t bytes;

		ReadFile(file_handle, *buffer, size, (LPDWORD)&bytes, NULL);

		CloseHandle(file_handle);

		return size;
	}

	bool IsAddressValid(PVOID address)
	{
		if (((uintptr_t)address < 0x7FFFFFFFFFFF) && ((uintptr_t)address > 0x1000))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	DWORD GetProcId(const wchar_t* procName)
	{
		/*DWORD procId = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnap != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 procEntry;
			procEntry.dwSize = sizeof(procEntry);

			if (Process32First(hSnap, &procEntry))
			{
				do
				{
					if (!_wcsicmp(procEntry.szExeFile, procName))
					{
						procId = procEntry.th32ProcessID;
						break;
					}
				} while (Process32Next(hSnap, &procEntry));
			}
		}
		CloseHandle(hSnap);
		return procId;*/
		return Driver::GetProcessId(procName);
	}
};
