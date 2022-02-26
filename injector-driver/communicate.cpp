#include "communicate.h"

namespace Interface
{
	bool Init()
	{
		ForteVisor::SetNptHook(NtDeviceIoControlFile)
	}
}
