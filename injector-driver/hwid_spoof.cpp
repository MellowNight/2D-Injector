#include "hwid_spoof.h"

Spoofer* Spoofer::spoofer = NULL;

Spoofer* Spoofer::Get()
{
    if (spoofer == 0)
    {
        spoofer = (Spoofer*)ExAllocatePool(NonPagedPool, sizeof(Spoofer));
        spoofer->Init();
    }

    return spoofer;
}

void Spoofer::SpoofDisk(uint32_t IoControlCode, void* InputBuffer, void* OutputBuffer, size_t OutputBufferLength)
{
    if (IoControlCode == IOCTL_STORAGE_QUERY_PROPERTY)
    {
        if (StorageDeviceProperty == ((STORAGE_PROPERTY_QUERY*)InputBuffer)->PropertyId)
        {
            auto descriptor = (STORAGE_DEVICE_DESCRIPTOR*)OutputBuffer;

            auto offset = descriptor->SerialNumberOffset;

            if (offset && offset < OutputBufferLength)
            {
                strcpy((char*)descriptor + offset, disk_serial);
            }
        }
    }
    else if (IoControlCode == SMART_RCV_DRIVE_DATA)
    {
        if (OutputBufferLength >= sizeof(SENDCMDOUTPARAMS))
        {
            char* serial = ((PIDSECTOR)((PSENDCMDOUTPARAMS)OutputBuffer)->bBuffer)->sSerialNumber;
                
            Utils::SwapEndianess(serial, disk_serial);
        }
	}
}