#include "includes.h"
#include "hwid_spoof.h"

namespace Spoofer
{
    char disk_serial[13];

    void SpoofDisk(uint32_t IoControlCode, void* InputBuffer, void* OutputBuffer)
    {
        if (IoControlCode == IOCTL_STORAGE_QUERY_PROPERTY)
        {
            if (StorageDeviceProperty == ((STORAGE_PROPERTY_QUERY*)InputBuffer)->PropertyId)
            {
                auto descriptor = (STORAGE_DEVICE_DESCRIPTOR*)OutputBuffer;

                auto offset = descriptor->SerialNumberOffset;

                if (offset && offset < request.BufferLength)
                {
                    strcpy((char*)descriptor + offset, disk_serial);

                    printf("spoofed IOCTL_STORAGE_QUERY_PROPERTY \n");
                }
            }
        }
        else if (IoControlCode == SMART_RCV_DRIVE_DATA)
        {
            if (request.BufferLength >= sizeof(SENDCMDOUTPARAMS)) 
            {
                char* serial = ((PIDSECTOR)((PSENDCMDOUTPARAMS)request.Buffer)->bBuffer)->sSerialNumber;
                
                Utils::SwapEndianess(serial, disk_serial);

                printf("handled SMART_RCV_DRIVE_DATA\n");
            }
		}
    }
};
