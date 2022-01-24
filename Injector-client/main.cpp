#include "communicate.h"
#include "manual_map.h"

#define TARGET L"VALORANT-Win64-Shipping.exe"
#define ENTRYPOINT_NAME "HookEntryPoint"

struct InjectInfo
{
	int     header = 0x1234;
	void*** swapchain_ptr;
	void** o_swapchain_vmt;
	uintptr_t dll_size;
	char username[60];
	char password[60];
	char original_bytes[60];
};



extern "C" int main()
{
	std::string CHEAT_DLL_NAME;
	std::cout << "Enter the name of the DLL to inject: " << std::endl;
	std::cin >> CHEAT_DLL_NAME;

	Driver::Init();

	auto file_handle = CreateFileA(
		CHEAT_DLL_NAME.c_str(), GENERIC_ALL, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);

	std::cout << "GetLastError " << GetLastError() << std::endl;

	auto disk_size = GetFileSize(file_handle, NULL);

	SetFilePointer(file_handle, 0, 0, 0);

	auto image_buf = new BYTE[disk_size];	// mapped to file offset

	DWORD bytes;

	ReadFile(file_handle, image_buf, disk_size, &bytes, NULL);
	
	auto pe_hdr = PeHeader(image_buf);

	auto real_virtual_size = pe_hdr->OptionalHeader.SizeOfImage;	// get real size of DLL

	int target = GetProcId(TARGET);

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

	/* maps the section to target process, enables Execute on page */

	auto section_base = Driver::MapMem(
		target, real_virtual_size + 0x1000, section_name
	);	

	/*  map our DLL 0x1000 bytes into the section to make space for parameters */
	auto map_base = section_base + 0x1000;	

	auto image = PrepareImage(file_handle, disk_size, map_base, target);

	CloseHandle(file_handle);

	Driver::WriteMem(target, map_base, (BYTE*)image, real_virtual_size);

	auto entry_offset = (uintptr_t)GetExport((uintptr_t)image, (char*)ENTRYPOINT_NAME);
 
	std::cout << "entry_offset " << entry_offset << "\n";

	Driver::StartThread(map_base + entry_offset, target, section_base, real_virtual_size);

	delete image;

	Driver::ExitDriver();
	std::cin.get();
}
