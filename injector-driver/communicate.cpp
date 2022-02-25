#include "communicate.h"
#include "disk_hook.h"

namespace Interface
{
	PEPROCESS client;

	bool Init()
	{
		NTSTATUS status = STATUS_SUCCESS;
		ULONG disk_size;

		auto	disk_base = (uintptr_t)utils::GetKernelModule(&disk_size, RTL_CONSTANT_STRING(L"disk.sys"));
		auto	nt_base = (PVOID)utils::GetKernelModule(NULL, RTL_CONSTANT_STRING(L"ntoskrnl.exe"));

		if (disk_base == 0)
		{
			DbgPrint("Failed to get disk base \n");
			return false;
		}

		/*   48 89 3D  */

		uintptr_t  WPP_GLOBAL_Control;
		utils::BBScan("PAGE", (PCUCHAR)"\x48\x89\x3D", '\x00', 3, &WPP_GLOBAL_Control, (PVOID)disk_base);

		if (WPP_GLOBAL_Control == NULL)
		{
			DbgPrint("Failed to get WPP_GLOBAL_Control \n");
			return false;
		}
		DbgPrint("WPP_GLOBAL_Control sig found at %p \n", WPP_GLOBAL_Control);
		WPP_GLOBAL_Control = RELATIVE_ADDR(WPP_GLOBAL_Control, 3, 7);
		DbgPrint("WPP global control timer %p \n", ((PDEVICE_OBJECT)WPP_GLOBAL_Control)->Timer);

		(*(PDEVICE_OBJECT*)WPP_GLOBAL_Control)->Timer = (PIO_TIMER)0xFFFFFFFFFFFFFFFF;

		uintptr_t wpp_trace;
		/*  48 8B 05 ? ? ? ? 48 83    */
		utils::BBScan(".text", (PCUCHAR)"\x48\x8B\x05\x00\x00\x00\x00\x48\x83", '\x00', 9, &wpp_trace, (PVOID)disk_base);

		if (wpp_trace == NULL)
		{
			DbgPrint("Failed to get wpp_trace \n");
			return false;
		}
		DbgPrint("wpp_trace sig found at %p disk_base %p nt_base %p \n", wpp_trace, disk_base, nt_base);
		wpp_trace = RELATIVE_ADDR(wpp_trace, 3, 7);
		DbgPrint("wpp_trace found at %p \n", wpp_trace);
		DbgPrint("wpp_trace value %p \n", *(pfnWppTraceMessage*)wpp_trace);

		/*  PpmPdcNotifyMediaBufferingUpdate 40 8A CF 40 88 3D ? ? ? ? E8 ? ? ? ? 
			this is paged code, must lock
		*/
		uintptr_t     NotifyMediaBufferingUpdate;

		utils::BBScan(".text", (PCUCHAR)"\x40\x8A\xCF\x40\x88\x3D\x00\00\x00\x00\xE8", '\x00', 11,
			&NotifyMediaBufferingUpdate, nt_base);

		NotifyMediaBufferingUpdate = (uintptr_t)RELATIVE_ADDR(NotifyMediaBufferingUpdate + 10, 1, 5);

		utils::LockPages((PVOID)NotifyMediaBufferingUpdate, IoReadAccess);

		if (NotifyMediaBufferingUpdate == NULL)
		{
			DbgPrint("Failed to get NotifyMediaBufferingUpdate \n");
			return false;
		}

		/*	90 E9 ? ? ? ? 48 8D 54	this is for verifying that the hook was called from the original disk IOCTL handler	*/
		utils::BBScan(".text", (PCUCHAR)"\x90\xE9\x00\x00\x00\x00\x48\x8D\x54", '\x00', 9, &ioctl_ret_addr, (PVOID)disk_base);
		DbgPrint("ioctl_ret_addr %p \n", ioctl_ret_addr);


		/*  chain WppTraceMessage with PpmPdcNotifyMediaBufferingUpdate */
		o_WppTraceMessage = *(pfnWppTraceMessage*)wpp_trace;
		*(PVOID*)wpp_trace = (PVOID)NotifyMediaBufferingUpdate;

		/*  point the 2nd function pointer to our hook */
		uintptr_t   function_ptr = RELATIVE_ADDR((NotifyMediaBufferingUpdate + 4), 3, 7);
		*(PVOID*)function_ptr = DiskHookHandler;

		return TRUE;
	}
}
