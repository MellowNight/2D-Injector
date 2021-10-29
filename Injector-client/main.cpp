#include "communicate.h"
#include "manual_map.h"

#define CHEAT_DLL_NAME "test_bypass_BE_dll.dll"
#define TARGET L"test-anticheat.exe"

int main()
{
	Driver::Init();

	auto file_handle = CreateFileA(
		CHEAT_DLL_NAME, GENERIC_ALL, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);

	auto disk_size = GetFileSize(file_handle, NULL);

	SetFilePointer(file_handle, 0, 0, 0);

	auto image_buf = new BYTE[disk_size];	// mapped to file offset

	DWORD bytes;
	ReadFile(file_handle, image_buf, disk_size, &bytes, NULL);
	
	auto PeHdr = PeHeader(image_buf);
	auto real_virtual_size = PeHdr->OptionalHeader.SizeOfImage;	// get real size of DLL

	int target = GetProcId(TARGET);

	/*	create a section that will be mapped into the target process	*/

	wchar_t section_name[] = L"Global\\r6iinternal";

	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE, 
		NULL,     
		PAGE_EXECUTE_READWRITE,
		0,                
		real_virtual_size,
		section_name
	); 

	std::cout << "hMapFile" << hMapFile << "GetLastError " << GetLastError() << std::endl;

	auto map_base = Driver::MapMem(target, real_virtual_size, section_name);

	std::cin.get();

	auto image = PrepareImage(file_handle, disk_size, map_base, target);

	std::cout << "Writing image... map base " << std::hex << map_base << std::endl;

	Driver::WriteMem(target, map_base, (BYTE*)image, real_virtual_size);

	std::cout << "Telling the driver to start bypass thread... \n";

	auto nt = PeHeader(image);
	
	Driver::StartThread(map_base + nt->OptionalHeader.AddressOfEntryPoint, target);

	Sleep(5000);
	delete image;
	Driver::ExitDriver();
	std::cin.get();
}
