//###########################################################################
// FILE:   F28M35x_Ipc_Util.c
// TITLE:  F28M35x Inter-Processor Communication (IPC) API Driver Utility
//         Functions
// DESCRIPTION:
//         28x API functions for inter-processor communications between the
//         C28 control system and M3 master system.
//         The driver functions in this file are available only as
//         sample functions for application development.  Due to the generic
//         nature of these functions and the cycle overhead inherent to a
//         function call, the code is not intended to be used in cases where
//         maximum efficiency is required in a system.
//###########################################################################
// $TI Release:
// $Release Date:
//###########################################################################

//*****************************************************************************
//! \addtogroup ipc_util_api_c28
//! @{
//*****************************************************************************
#include "F28M35x_Device.h"     // Headerfile Include File
#include "F28M35x_Ipc_drivers.h"

//*****************************************************************************
//! C28 Acknowledges MtoC IPC Flag.
//!
//! \param ulFlags specifies the IPC flag mask for flags being acknowledged.
//!
//! This function will allow the C28 control system to acknowledge/clear the IPC
//! flag set by the M3 master system. The \e ulFlags parameter can be any of
//! the IPC flag values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCFlagAcknowledge (Uint32 ulFlags)
{
    CtoMIpcRegs.MTOCIPCACK.all |= ulFlags;
}

//*****************************************************************************
//! Determines whether the given MtoC IPC flags are busy or not.
//!
//! \param ulFlags specifies MtoC IPC Flag number masks to check the status of.
//!
//! Allows the caller to determine whether the designated IPC flags are
//! pending. The \e ulFlags parameter can be any of the IPC flag
//! values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return Returns \b 1 if the IPC flags are busy or \b 0 if designated
//! IPC flags are free.
//*****************************************************************************
Uint16
IPCMtoCFlagBusy (Uint32 ulFlags)
{
    if ((CtoMIpcRegs.MTOCIPCSTS.all & ulFlags) == 0)
    {
        return (0);
    }
    else
    {
        return (1);
    }
}

//*****************************************************************************
//! Determines whether the given IPC flags are busy or not.
//!
//! \param ulFlags specifies C to M IPC Flag number masks to check the status
//! of.
//!
//! Allows the caller to determine whether the designated IPC flags are
//! available for further control to master system communication. If \b 0 is
//! returned, then all designated tasks have completed and are available.
//! The \e ulFlags parameter can be any of the IPC flag
//! values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return Returns \b 1 if the IPC flags are busy or \b 0 if designated
//! IPC flags are free.
//*****************************************************************************
Uint16
IPCCtoMFlagBusy (Uint32 ulFlags)
{
    if ((CtoMIpcRegs.CTOMIPCFLG.all & ulFlags) == 0)
    {
        return (0);
    }
    else
    {
        return (1);
    }
}

//*****************************************************************************
//! C28 Sets CtoM IPC Flag
//!
//! \param ulFlags specifies the IPC flag mask for flags being set.
//!
//! This function will allow the C28 control system to set the designated IPC
//! flags to send to the M3 master system. The \e ulFlags parameter can be any
//! of the IPC flag values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMFlagSet (Uint32 ulFlags)
{
    CtoMIpcRegs.CTOMIPCSET.all |= ulFlags;
}

//*****************************************************************************
//! C28 Clears CtoM IPC Flag
//!
//! \param ulFlags specifies the IPC flag mask for flags being set.
//!
//! This function will allow the C28 control system to set the designated IPC
//! flags to send to the M3 master system. The \e ulFlags parameter can be any
//! of the IPC flag values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMFlagClear (Uint32 ulFlags)
{
    CtoMIpcRegs.CTOMIPCCLR.all |= ulFlags;
}

//*****************************************************************************
//! Converts a C28 shared RAM address to its M3 equivalent address
//!
//! \param ulShareAddress specifies the C28 address in shared RAM (Sx RAM
//! or MSG RAM).
//!
//! This function will convert a shared RAM address location on the C28 control
//! system to its equivalent M3 master system address (applies to MSG RAM's and
//! Sx shared RAM blocks).
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
Uint32
IPCCtoMSharedRamConvert (Uint32 ulShareAddress)
{
    // MSG RAM address conversion
    if (ulShareAddress >= C28_CTOMMSGRAM_START)
    {
        return ((ulShareAddress<<1) + 0x20000000);
    }

    // Sx RAM address conversion
    //
    else
    {
        return ((ulShareAddress<<1) + 0x1FFF0000);
    }
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************


