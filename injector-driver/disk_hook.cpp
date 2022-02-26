#include "disk_hook.h"
#include "communicate.h"
#include "stack.h"
#include <ata.h>
#include <ntdddisk.h>
#include <ntddscsi.h>

void CommandHandler(PVOID context);

pfnWppTraceMessage o_WppTraceMessage = NULL;
uintptr_t ioctl_ret_addr;

CHAR alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

struct COMPLETION_CONTEXT
{
	PVOID   SystemBuffer;
	int     OutLength;
	PVOID   OldContext;
	PIO_COMPLETION_ROUTINE  OldRoutine;
	int		stack_index;    /*  for logging info    */
};

void SetIoCompletion(
	PIRP irp, 
	PIO_STACK_LOCATION ioc, 
	PIO_COMPLETION_ROUTINE routine, 
	int stack_index = 0
)
{
	COMPLETION_CONTEXT* context = (COMPLETION_CONTEXT*)ExAllocatePool(NonPagedPool, sizeof(COMPLETION_CONTEXT));

	context->stack_index = stack_index;
	context->SystemBuffer = irp->AssociatedIrp.SystemBuffer;
	context->OutLength = ioc->Parameters.DeviceIoControl.OutputBufferLength;
	context->OldContext = ioc->Context;
	context->OldRoutine = ioc->CompletionRoutine;

	ioc->Control = SL_INVOKE_ON_SUCCESS;
	ioc->Context = context;
	ioc->CompletionRoutine = routine;
}

VOID SwapEndianess(PCHAR dest, PCHAR src) {
	for (size_t i = 0, l = strlen(src); i < l; i += 2) {
		dest[i] = src[i + 1];
		dest[i + 1] = src[i];
	}
}

void RandomString(char* buffer, int length)
{
	ULONG64 time = 0;
	KeQuerySystemTime(&time);
	auto seed = (DWORD)time;

	for (int i = 0; i < length; ++i)
	{
		buffer[i] = alphabet[RtlRandomEx(&seed) % (sizeof(alphabet) - 1)];
	}

	buffer[length - 1] = '\0';
}

NTSTATUS StorageQueryCompletion(PDEVICE_OBJECT device, PIRP irp, PVOID context)
{
	COMPLETION_CONTEXT request = *(COMPLETION_CONTEXT*)context;
	ExFreePool(context);

	if (request.OutLength >= sizeof(STORAGE_DEVICE_DESCRIPTOR))
	{
		PSTORAGE_DEVICE_DESCRIPTOR desc = (PSTORAGE_DEVICE_DESCRIPTOR)request.SystemBuffer;

		ULONG offset = desc->SerialNumberOffset;

		if (offset && offset < request.OutLength)
		{
			char new_serial[17];

			RandomString(new_serial, 17);
			strcpy((PCHAR)desc + offset, new_serial);

			DbgPrint("handled StorageQueryIoc\n");
		}

	}

	if (request.OldRoutine && irp->StackCount > 1) {
		return request.OldRoutine(device, irp, request.OldContext);
	}


	return STATUS_SUCCESS;
}


NTSTATUS SmartComplete(PDEVICE_OBJECT device, PIRP irp, PVOID context) {

	COMPLETION_CONTEXT* request = (COMPLETION_CONTEXT*)context;

	DbgPrint("hello from smartComplete, stack_index %i \n", request->stack_index);

	if (request->OutLength >= sizeof(SENDCMDOUTPARAMS))
	{
		PVOID	data_buffer = ((PSENDCMDOUTPARAMS)request->SystemBuffer)->bBuffer;

		PCHAR	serial = (char*)((IDENTIFY_DEVICE_DATA*)data_buffer)->SerialNumber;

		char new_serial[17] = { 0 };

		RandomString(new_serial, 17);

		SwapEndianess(serial, new_serial);
	}

	if (request->OldRoutine && irp->StackCount > 1) {
		return request->OldRoutine(device, irp, request->OldContext);
	}

	ExFreePool(context);

	return STATUS_SUCCESS;
}



void DiskHookHandler()
{
	// make sure the hook call originated from disk wpp function
	if (Stack::FindValue(4, ioctl_ret_addr, 30) == false)
	{
		return;
	}

	auto irp = Stack::FindIRP(60, 33);

	if (!MmIsAddressValid(irp) || !MmIsAddressValid((BYTE*)irp + 206) || irp == NULL)
	{
		return;
	}

	auto mdl = Utils::LockPages(irp, IoModifyAccess, sizeof(IRP));

	if (!(irp->CurrentLocation <= irp->StackCount + 1))
	{
		// If there are no stack locations to go through
		return;
	}
	PIO_STACK_LOCATION  stackLoc = IoGetCurrentIrpStackLocation(irp);

	if (((Interface::Msg*)irp->AssociatedIrp.SystemBuffer)->command_key == COMMAND_KEY)
	{
		DbgPrint("command key found \n");
		CommandHandler(irp->AssociatedIrp.SystemBuffer);
	}

	switch (stackLoc->Parameters.DeviceIoControl.IoControlCode)
	{
		case SMART_RCV_DRIVE_DATA:
		{
			SetIoCompletion(irp, stackLoc, SmartComplete);

			break;
		}
		case IOCTL_STORAGE_QUERY_PROPERTY:
		{
			if (StorageDeviceProperty == ((PSTORAGE_PROPERTY_QUERY)irp->AssociatedIrp.SystemBuffer)->PropertyId)
			{
				SetIoCompletion(irp, stackLoc, StorageQueryCompletion);
			}
			break;
		}
		default:
		{
			break;
		}
	}

	Utils::UnlockPages(mdl);
	return;
}

