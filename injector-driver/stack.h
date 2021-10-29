#pragma once
#include "util.h"
#include "communicate.h"

namespace Stack
{
    bool FindValue(int start_pos, uintptr_t value, int max_difference = 15)
    {
        uintptr_t* frame = (uintptr_t*)_AddressOfReturnAddress();

        frame += start_pos;

        for (; frame <= (uintptr_t*)PsGetCurrentThreadStackBase() - 1; ++frame)
        {
            auto diff = *frame > value ? *frame - value : value - *frame;

            if (diff <= max_difference)
            {
                return true;
            }
        }

        return false;
    }

    PIRP    FindIRP(int max_search_depth, int skip_amount = 0)
    {
        PVOID*  stack_data = (PVOID*)_AddressOfReturnAddress() + skip_amount;
        int     stack_index = skip_amount;
        PIRP    irp = 0;

        for (; stack_data <= (PVOID*)PsGetCurrentThreadStackBase() - 1; ++stack_data, ++stack_index)
        {
            PIRP irp = (PIRP)*stack_data;

            if (stack_index > max_search_depth)
                break;

            if (!MmIsAddressValid(irp) || (irp == NULL))    // basic address checks
                continue;

            if (!MmIsAddressValid(irp->AssociatedIrp.SystemBuffer) ||                       // Is system buffer valid
                (irp->RequestorMode != KernelMode && irp->RequestorMode != UserMode) ||     // Is requestor mode valid
                !(irp->StackCount > 0 && irp->StackCount < 20))                             // is IRP stack count is valid
            {
                continue;
            }

            if (((Interface::Msg*)irp->AssociatedIrp.SystemBuffer)->command_key == COMMAND_KEY)
            {
                return irp;
            }

            if (MmIsAddressValid(irp->Tail.Overlay.CurrentStackLocation) &&       // Is stack location valid
                (irp->Tail.Overlay.CurrentStackLocation != NULL))
            {
                irp = (PIRP)*stack_data;
                return irp;
            }
        }

        DbgPrint("IRP Not found! \n");
    }
}
