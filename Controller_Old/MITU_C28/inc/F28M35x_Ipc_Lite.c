//###########################################################################
// FILE:   F28M35x_Ipc_Lite.c
// TITLE:  F28M35x Inter-Processor Communication (IPC) Lite API Driver
//         Functions.
//
// DESCRIPTION:
//         28x API functions for inter-processor communications between the
//         C28 control system and M3 master system (Lite version). The IPC
//         Lite functions only allow for basic functions such as data writes,
//         reads, bit setting, and bit setting.  The Lite functions do not
//         require the usage of the MSG RAM's or shared memories and can only
//         be used with a single IPC interrupt channel.  Commands can only
//         be processed one at a time without queuing.
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
//! \addtogroup ipc_lite_api_c28 C28 IPC-Lite API Drivers
//! @{
//*****************************************************************************
#include "F28M35x_Device.h"     // Headerfile Include File
#include "F28M35x_Ipc_drivers.h"

void DelayLoop (void);

//*****************************************************************************
//! Reads single word data result of CtoM IPC command
//!
//! \param pvData is a pointer to the 16/32-bit variable where the result data
//! will be stored.
//! \param usLength designates 16- or 32-bit read.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master  system. If a status flag
//! was not used with the command call, set this parameter to 0.
//!
//! Allows the caller to read the 16/32-bit data result of non-blocking IPC
//! functions from the CTOMIPCDATAR register if the status flag is cleared
//! indicating the IPC command was successfully interpreted. If the status flag
//! is not cleared, the command was not recognized, and the function will
//! return STATUS_FAIL. To determine what data is read from a call to this
//! function, see the descriptions of the non-blocking IPC functions.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The  \e ulStatusFlag parameter
//! accepts any of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS or \b STATUS_FAIL.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
Uint16
IPCLiteCtoMGetResult (void *pvData, Uint16 usLength, Uint32 ulStatusFlag)
{
    // If Master System never acknowledged Status Task, indicates command
    // failure.
    if (CtoMIpcRegs.CTOMIPCFLG.all & ulStatusFlag)
    {
        return STATUS_FAIL;
    }
    else
    {
        // Read data.
        if (usLength == IPC_LENGTH_16_BITS)
        {
            *(Uint16 *)pvData = CtoMIpcRegs.CTOMIPCDATAR;
        }
        else if (usLength == IPC_LENGTH_32_BITS)
        {
            *(Uint32 *)pvData =  CtoMIpcRegs.CTOMIPCDATAR;
        }
        return STATUS_PASS;
    }
}

//*****************************************************************************
//! Reads either a 16- or 32-bit data word from an M3 Master System address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 address to read from
//! \param usLength designates 16- or 32-bit read (1 = 16-bit, 2 = 32-bit)
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master  system.
//!
//! This function will allow the C28 Control System to read 16/32-bit data from
//! the M3 Master System into the CTOMIPCDATAR register. After calling this
//! function, a call to \e IPCLiteCtoMGetResult() will read the data value in
//! the CTOMIPCDATAR register into a 16- or 32-bit variable in the C28
//! application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulStatusFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
Uint16
IPCLiteCtoMDataRead(Uint32 ulFlag, Uint32 ulAddress, Uint16 usLength,
                    Uint32 ulStatusFlag)
{
    // Return false if IPC C to M request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    // Set up read command, address, and word length.
    if (usLength == IPC_LENGTH_16_BITS)
    {
        CtoMIpcRegs.CTOMIPCCOM = IPC_DATA_READ_16;
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        CtoMIpcRegs.CTOMIPCCOM = IPC_DATA_READ_32;
    }
    CtoMIpcRegs.CTOMIPCADDR = ulAddress;

    // Force IPC event on selected request task and enable status-checking.
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at an M3 master system
//! address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! M3 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master system.
//!
//! This function will allow the C28 control system to set bits specified by the
//! \e usMask variable in a 16/32-bit word on the M3 master system. The data
//! word at /e ulAddress after the set bits command is then read into the
//! CTOMIPCDATAR register. After calling this function, a call to \e
//! IPCLiteCtoMGetResult() will read the data value in the CTOMIPCDATAR register
//! into a 16/32-bit variable in the C28 application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulStatusFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
Uint16
IPCLiteCtoMSetBits(Uint32 ulFlag, Uint32 ulAddress, Uint32 ulMask,
                   Uint16 usLength,
                   Uint32 ulStatusFlag)
{
    // Return false if IPC C to M request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    if (usLength == IPC_LENGTH_16_BITS)
    {
        // Set up 16-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_SET_BITS_16;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask & (0x0000FFFF);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        // Set up 32-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_SET_BITS_32;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask;
    }

    // Force IPC event on selected request task and enable status-checking.
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit write-protected data word at an
//! M3 master system address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 write-protected address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! M3 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master system.
//!
//! This function will allow the C28 control system to set bits specified by the
//! \e usMask variable in a write-protected 16/32-bit word on the M3 master
//! system.
//! The data word at /e ulAddress after the set bits command is then read into
//! the CTOMIPCDATAR register. After calling this function, a call to
//! \e IPCLiteCtoMGetResult() will read the data value in the CTOMIPCDATAR
//! register into a 16/32-bit variable in the C28 application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulStatusFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
Uint16
IPCLiteCtoMSetBits_Protected (Uint32 ulFlag, Uint32 ulAddress, Uint32 ulMask,
                              Uint16 usLength,
                              Uint32 ulStatusFlag)
{
    // Return false if IPC C to M request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    if (usLength == IPC_LENGTH_16_BITS)
    {
        // Set up 16-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_SET_BITS_16_PROTECTED;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask & (0x0000FFFF);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        // Set up 32-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_SET_BITS_32_PROTECTED;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask;
    }

    // Force IPC event on selected request task and enable status-checking.
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at an M3 master system
//! address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! M3 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master system.
//!
//! This function will allow the C28 control system to set bits specified by the
//! \e usMask variable in a 16/32-bit word on the M3 master system. The data
//! word at /e ulAddress after the set bits command is then read into the
//! CTOMIPCDATAR register. After calling this function, a call to \e
//! IPCLiteCtoMGetResult() will read the data value in the CTOMIPCDATAR register
//! into a 16/32-bit variable in the C28 application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulStatusFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
Uint16
IPCLiteCtoMClearBits(Uint32 ulFlag, Uint32 ulAddress, Uint32 ulMask,
                     Uint16 usLength,
                     Uint32 ulStatusFlag)
{
    // Return false if IPC C to M request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    if (usLength == IPC_LENGTH_16_BITS)
    {
        // Set up 16-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_CLEAR_BITS_16;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask & (0x0000FFFF);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        // Set up 32-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_CLEAR_BITS_32;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask;
    }

    // Force IPC event on selected request task and enable status-checking.
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! Clears the designated bits in a 16/32-bit write-protected data word at an
//! M3 master system address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 write-protected address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be cleared
//! at M3 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master system.
//!
//! This function will allow the C28 control system to clear bits specified by
//! the \e usMask variable in a write-protected 16/32-bit word on the M3 master
//! system.
//! The data word at /e ulAddress after the clear bits command is then read into
//! the CTOMIPCDATAR register. After calling this function, a call to
//! \e IPCLiteCtoMGetResult() will read the data value in the CTOMIPCDATAR
//! register into a 16/32-bit variable in the C28 application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulStatusFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
Uint16
IPCLiteCtoMClearBits_Protected (Uint32 ulFlag, Uint32 ulAddress, Uint32 ulMask,
                                Uint16 usLength,
                                Uint32 ulStatusFlag)
{
    // Return false if IPC C to M request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    if (usLength == IPC_LENGTH_16_BITS)
    {
        // Set up 16-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_CLEAR_BITS_16_PROTECTED;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask & (0x0000FFFF);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        // Set up 32-bit set bits command, address, and mask.
        CtoMIpcRegs.CTOMIPCCOM = IPC_CLEAR_BITS_32_PROTECTED;
        CtoMIpcRegs.CTOMIPCADDR = ulAddress;
        CtoMIpcRegs.CTOMIPCDATAW = ulMask;
    }

    // Force IPC event on selected request task and enable status-checking.
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to an M3 Master System address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For 16-bit
//! words, only the lower 16-bits of ulData will be considered by the master
//! system.
//! \param usLength is the length of the word to write (0 = 16-bits, 1 =
//! 32-bits)
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master  system.
//!
//! This function will allow the C28 Control System to write a 16/32-bit word
//! via the \e ulData variable to an address on the M3 Master System.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulStatusFlag paramete
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************

Uint16
IPCLiteCtoMDataWrite(Uint32 ulFlag, Uint32 ulAddress, Uint32 ulData,
                     Uint16 usLength,
                     Uint32 ulStatusFlag)
{
    // Return false if IPC C to M request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    // Set up data write command, address, and data. For 16-bit write, Master
    // system will
    // look at lower 16-bits only.
    if (usLength == IPC_LENGTH_16_BITS)
    {
        CtoMIpcRegs.CTOMIPCCOM = IPC_DATA_WRITE_16;
    } else
    {
        CtoMIpcRegs.CTOMIPCCOM = IPC_DATA_WRITE_32;
    }
    CtoMIpcRegs.CTOMIPCADDR = ulAddress;
    CtoMIpcRegs.CTOMIPCDATAW = ulData;

    // Force IPC event on selected request task and enable status-checking
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to a protected M3 Master System address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For 16-bit
//! words, only the lower 16-bits of ulData will be considered by the master
//! system.
//! \param usLength is the length of the word to write (0 = 16-bits, 1 =
//! 32-bits)
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master  system.
//!
//! This function will allow the C28 Control System to write a 16/32-bit word
//! via the \e ulData variable to a write-protected address on the M3 Master
//! System. The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulStatusFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************

Uint16
IPCLiteCtoMDataWrite_Protected(Uint32 ulFlag, Uint32 ulAddress, Uint32 ulData,
                               Uint16 usLength,
                               Uint32 ulStatusFlag)
{
    // Return false if IPC C to M request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    // Set up data write command, address, and data. For 16-bit write, Master
    // system will
    // look at lower 16-bits only.
    if (usLength == IPC_LENGTH_16_BITS)
    {
        CtoMIpcRegs.CTOMIPCCOM = IPC_DATA_WRITE_16_PROTECTED;
    } else
    {
        CtoMIpcRegs.CTOMIPCCOM = IPC_DATA_WRITE_32_PROTECTED;
    }
    CtoMIpcRegs.CTOMIPCADDR = ulAddress;
    CtoMIpcRegs.CTOMIPCDATAW = ulData;

    // Force IPC event on selected request task and enable status-checking
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! Calls a M3 function with 1 optional parameter and an optional return value.
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the M3 function address
//! \param ulParam specifies the 32-bit optional parameter value
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the control  system.
//!
//! This function will allow the C28 control system to call a function on the
//! M3. The \e ulParam variable is a single optional 32-bit parameter to pass to
//! the function. The \e ulFlag parameter accepts any one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32. The \e ulStatusFlag parameter accepts any
//! other one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b
//! NO_FLAG. The function returns \b STATUS_PASS if the command is successful
//! or \b STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************

Uint16
IPCLiteCtoMFunctionCall(Uint32 ulFlag, Uint32 ulAddress, Uint32 ulParam,
                        Uint32 ulStatusFlag)
{
    // Return false if IPC M to C request or status flags are not available.
    if (CtoMIpcRegs.CTOMIPCFLG.all & (ulFlag | ulStatusFlag))
    {
        return STATUS_FAIL;
    }

    // Set up function call command, address, and parameter.
    CtoMIpcRegs.CTOMIPCCOM = IPC_FUNC_CALL;
    CtoMIpcRegs.CTOMIPCADDR = ulAddress;
    CtoMIpcRegs.CTOMIPCDATAW = ulParam;

    // Force IPC event on selected request task and enable status-checking
    CtoMIpcRegs.CTOMIPCSET.all |= (ulFlag | ulStatusFlag);

    return STATUS_PASS;
}

//*****************************************************************************
//! C28 Requests Master R/W/Exe Access to Shared SARAM.
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulMask specifies the 32-bit mask for the SxMEMSEL RAM control
//! register to indicate which Sx SARAM blocks the C28 is requesting master
//! access to.
//! \param ulMaster specifies whether C28 or M3 should be the master of the Sx
//! RAM.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report the
//! status of the command sent back from the Master  system.
//!
//! This function will allow the C28 Control System to request C28 or M3
//! mastership of any of the Sx Shared SARAM blocks.
//! The \e ulMaster parameter accepts the following values: \b IPC_SX_C28MASTER
//! or \b IPC_SX_M3MASTER. The \e ulStatusFlag parameter accepts any one of the
//! flag values \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG. The function
//! returns \b STATUS_PASS if the command is successful or \b STATUS_FAIL if
//! the request or status flags are unavailable.
//! \note This function calls the \e IPCLiteCtoMSetBits_Protected() or the
//! \e IPCLiteCtoMClearBits_Protected function, and therefore in order to
//! process this function, the above 2 functions should be ready to be called on
//! the M3 system to process this command.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************

Uint16
IPCLiteCtoMReqMemAccess (Uint32 ulFlag, Uint32 ulMask, Uint16 ulMaster,
                         Uint32 ulStatusFlag)
{
    Uint16 status;

    if (ulMaster == IPC_SX_C28MASTER)
    {
        status =
            IPCLiteCtoMSetBits_Protected (ulFlag, MSXMSEL_M3REG, ulMask,
                                          IPC_LENGTH_32_BITS,
                                          ulStatusFlag);
    }
    else if (ulMaster == IPC_SX_M3MASTER)
    {
        status =
            IPCLiteCtoMClearBits_Protected (ulFlag, MSXMSEL_M3REG, ulMask,
                                            IPC_LENGTH_32_BITS,
                                            ulStatusFlag);
    }

    return status;
}

//*****************************************************************************
//! Reads either a 16- or 32-bit data word from an C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control  system.
//!
//! This function will allow the M3 master system to read 16/32-bit data from
//! the C28 control system. The \e ulFlag parameter accepts any one of the
//! flag values \b IPC_FLAG1 - \b IPC_FLAG32, and the \e ulStatusFlag parameter
//! accepts any other one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and
//! \b NO_FLAG.
//*****************************************************************************
void
IPCLiteMtoCDataRead(Uint32 ulFlag, Uint32 ulStatusFlag)
{

    Uint32* pulRAddress;
    Uint16* pusRAddress;

    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command and data length are correct for this function:
    // Then read from requested address and write 16/32-bit data
    // to MTOCIPCDATAR. Acknowledge the status flag
    // and the task flag.
    //
    if (CtoMIpcRegs.MTOCIPCCOM == IPC_DATA_READ_16)
    {
        // Perform 16-bit read.
        pusRAddress = (Uint16 *)CtoMIpcRegs.MTOCIPCADDR;
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)(*pusRAddress);
        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }
    else if (CtoMIpcRegs.MTOCIPCCOM == IPC_DATA_READ_32)
    {
        pulRAddress = (Uint32 *)CtoMIpcRegs.MTOCIPCADDR;
        CtoMIpcRegs.MTOCIPCDATAR = *pulRAddress;
        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at an C28 control system
//! address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to set bits specified by a
//! mask variable in a 16/32-bit word on the C28 control system, and then read
//! back the word into the MTOCIPCDATAR register. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32, and the
//! \e ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteMtoCSetBits(Uint32 ulFlag, Uint32 ulStatusFlag)
{

    Uint16* pusAddress;
    Uint32* pulAddress;

    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then set the mask bits at the requested address
    // and write back the 16/32-bit data to MTOCIPCDATAR.
    // Acknowledge the status flag and the task flag.
    //
    if (CtoMIpcRegs.MTOCIPCCOM == IPC_SET_BITS_16)
    {
        pusAddress = (Uint16 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pusAddress |= (Uint16)CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)*pusAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }
    else if (CtoMIpcRegs.MTOCIPCCOM == IPC_SET_BITS_32)
    {
        pulAddress = (Uint32 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pulAddress |= (Uint32)CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = *pulAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }
}

//*****************************************************************************
//! Sets the designated bits in a 16-bit data word at an C28 control system
//! write-protected address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to set bits specified by a
//! mask variable in a write-protected 16/32-bit word on the C28 control system,
//! and then read back the word into the MTOCIPCDATAR register. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteMtoCSetBits_Protected (Uint32 ulFlag, Uint32 ulStatusFlag)
{

    Uint16* pusAddress;
    Uint32* pulAddress;

    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then enable write access with EALLOW and
    // set the mask bits at the requested address.
    // Write back the 16-bit data to MTOCIPCDATAR.
    // Restore write-protection with EDIS.
    // Acknowledge the status flag and the task flag.
    //

    EALLOW;

    if (CtoMIpcRegs.MTOCIPCCOM == IPC_SET_BITS_16_PROTECTED)
    {
        pusAddress = (Uint16 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pusAddress |= (Uint16)CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)*pusAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }
    else if (CtoMIpcRegs.MTOCIPCCOM == IPC_SET_BITS_32_PROTECTED)
    {
        pulAddress = (Uint32 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pulAddress |= (Uint32)CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = *pulAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }

    EDIS;

}

//*****************************************************************************
//! Clears the designated bits in a 16/32-bit data word at an C28 control system
//! address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to clear bits specified by a
//! mask variable in a 16/32-bit word on the C28 control system, and then read
//! back the word into the MTOCIPCDATAR register. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteMtoCClearBits(Uint32 ulFlag, Uint32 ulStatusFlag)
{

    Uint16* pusAddress;
    Uint32* pulAddress;

    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then clear the mask bits at the requested address
    // and write back the 16/32-bit data to MTOCIPCDATAR.
    // Acknowledge the status flag and the task flag.
    //
    if (CtoMIpcRegs.MTOCIPCCOM == IPC_CLEAR_BITS_16)
    {
        pusAddress = (Uint16 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pusAddress &= ~((Uint16)CtoMIpcRegs.MTOCIPCDATAW);
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)*pusAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }
    else if (CtoMIpcRegs.MTOCIPCCOM == IPC_CLEAR_BITS_32)
    {
        pulAddress = (Uint32 *)CtoMIpcRegs.MTOCIPCADDR;
        *pulAddress &= ~((Uint32)CtoMIpcRegs.MTOCIPCDATAW);
        CtoMIpcRegs.MTOCIPCDATAR = *pulAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }
}

//*****************************************************************************
//! Clears the designated bits in a 16/32-bit data word at an C28 control system
//! write-protected address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to clear bits specified by a
//! mask variable in a 16/32-bit word on the C28 control system, and then read
//! back the word into the MTOCIPCDATAR register. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteMtoCClearBits_Protected (Uint32 ulFlag, Uint32 ulStatusFlag)
{

    Uint16* pusAddress;
    Uint32* pulAddress;

    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then enable write access with MWRALLOW and
    // clear the mask bits at the requested address.
    // Write back the 16/32-bit data to MTOCIPCDATAR.
    // Restore the status of the MWRALLOW register.
    // Acknowledge the status flag and the task flag.
    //
    EALLOW;

    if (CtoMIpcRegs.MTOCIPCCOM == IPC_CLEAR_BITS_16_PROTECTED)
    {

        pusAddress = (Uint16 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pusAddress &= ~((Uint16)CtoMIpcRegs.MTOCIPCDATAW);
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)*pusAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }
    else if (CtoMIpcRegs.MTOCIPCCOM == IPC_CLEAR_BITS_32_PROTECTED)
    {

        pulAddress = (Uint32 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pulAddress &= ~((Uint32)CtoMIpcRegs.MTOCIPCDATAW);
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)*pulAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }

    EDIS;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to an C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to write a 16/32-bit word
//!  to an address on the C28 control system. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteMtoCDataWrite(Uint32 ulFlag, Uint32 ulStatusFlag)
{
    Uint32* pulAddress;
    Uint16* pusAddress;

    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then write the 16/32-bit data to the requested address
    // and write back the 16/32-bit data to MTOCIPCDATAR.
    // Acknowledge the status flag and the task flag.
    //
    if (CtoMIpcRegs.MTOCIPCCOM == IPC_DATA_WRITE_16)
    {
        pusAddress = (Uint16 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pusAddress = (Uint16)CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)*pusAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }
    else if (CtoMIpcRegs.MTOCIPCCOM == IPC_DATA_WRITE_32)
    {
        pulAddress = (Uint32 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pulAddress = CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = *pulAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);

    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }
}

//*****************************************************************************
//! Writes a 16/32-bit data word to a write-protected C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to write a 16/32-bit word
//! to an address on the C28 control system. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************

void
IPCLiteMtoCDataWrite_Protected(Uint32 ulFlag, Uint32 ulStatusFlag)
{
    Uint32* pulAddress;
    Uint16* pusAddress;

    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then enable write access with MWRALLOW and
    // write the 16/32-bit data to the requested address
    // and write back the 16/32-bit data to MTOCIPCDATAR.
    // Acknowledge the status flag and the task flag.
    //
    EALLOW;

    if (CtoMIpcRegs.MTOCIPCCOM == IPC_DATA_WRITE_16_PROTECTED)
    {
        pusAddress = (Uint16 *)CtoMIpcRegs.MTOCIPCADDR;;
        *pusAddress = (Uint16)CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = (Uint32)*pusAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }
    else if (CtoMIpcRegs.MTOCIPCCOM == IPC_DATA_WRITE_32_PROTECTED)
    {
        pulAddress = (Uint32 *)CtoMIpcRegs.MTOCIPCADDR;
        *pulAddress = CtoMIpcRegs.MTOCIPCDATAW;
        CtoMIpcRegs.MTOCIPCDATAR = *pulAddress;

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);

    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }

    // Restore write-protection status.
    //
    EDIS;
}

//*****************************************************************************
//! Calls a C28 function with a single optional parameter and return value.
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report the
//! status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to call a C28 function with a
//! a single optional parameter and places an optional return value in the
//! MTOCIPCDATAR register. The \e ulFlag parameter accepts any one of the flag
//! values \b IPC_FLAG1 - \b IPC_FLAG32, and  the \e ulStatusFlag parameter
//! accepts any other one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and
//! \b NO_FLAG.
//*****************************************************************************
void
IPCLiteMtoCFunctionCall(Uint32 ulFlag, Uint32 ulStatusFlag)
{
    // Wait until IPC M to C request task is flagged
    while (!(CtoMIpcRegs.MTOCIPCSTS.all & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then call function at requested address
    // and if there is a return value, insert into
    // MTOCIPCDATAR register.
    // Acknowledge the status flag and the task flag.
    //
    if (CtoMIpcRegs.MTOCIPCCOM == IPC_FUNC_CALL)
    {
        tfIpcFuncCall func_call = (tfIpcFuncCall)CtoMIpcRegs.MTOCIPCADDR;
        CtoMIpcRegs.MTOCIPCDATAR = func_call(CtoMIpcRegs.MTOCIPCDATAW);

        CtoMIpcRegs.MTOCIPCACK.all |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    //
    else
    {
        CtoMIpcRegs.MTOCIPCACK.all |= (ulFlag);
    }
}

void DelayLoop (void)
{
    __asm(" nop");
    __asm(" nop");
    __asm(" nop");
    __asm(" nop");
    __asm(" nop");
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************


