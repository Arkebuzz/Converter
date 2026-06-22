//###########################################################################
// FILE:   F28M35x_Ipc.c
// TITLE:  F28M35x Inter-Processor Communication (IPC) API Driver Functions.
// DESCRIPTION:
//         28x API functions for inter-processor communications between the
//         C28 control system and M3 master system. The IPC functions require
//         the usage of the CtoM and MtoC MSG RAM's to store the circular ring
//         buffer and indexes. Commands can be queued up in order on a single
//         IPC interrupt channel.  For those IPC commands which are not
//         interdependent, multiple IPC interrupt channels may be used.
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
//! \addtogroup ipc_api_c28
//! @{
//*****************************************************************************
#include "F28M35x_Device.h"     // Headerfile Include File
#include "F28M35x_Ipc_drivers.h"

//*****************************************************************************
// Map the following circular buffer and indexes to CTOM MSGRAM:
//     CtoM Put Buffer
//     CtoM Put Buffer Write Index
//     MtoC Get Buffer Read Index
//*****************************************************************************
#pragma DATA_SECTION(g_asIPCCtoMBuffers, "PUTBUFFER");
#pragma DATA_SECTION(g_usPutWriteIndexes, "PUTWRITEIDX");
#pragma DATA_SECTION(g_usGetReadIndexes, "GETREADIDX");

//*****************************************************************************
// Map the following circular buffer and indexes to MTOC MSGRAM:
//     MtoC Get Buffer
//     MtoC Get Buffer Write Index
//     CtoM Put Buffer Read Index
//*****************************************************************************
#pragma DATA_SECTION(g_asIPCMtoCBuffers, "GETBUFFER");
#pragma DATA_SECTION(g_usGetWriteIndexes, "GETWRITEIDX");
#pragma DATA_SECTION(g_usPutReadIndexes, "PUTREADIDX");

//*****************************************************************************
// Global Circular Buffer Definitions
//*****************************************************************************
tIpcMessage g_asIPCCtoMBuffers[NUM_IPC_INTERRUPTS][IPC_BUFFER_SIZE];
tIpcMessage g_asIPCMtoCBuffers[NUM_IPC_INTERRUPTS][IPC_BUFFER_SIZE];

//*****************************************************************************
// Global Circular Buffer Index Definitions
//*****************************************************************************
Uint16 g_usPutWriteIndexes[NUM_IPC_INTERRUPTS];
Uint16 g_usPutReadIndexes[NUM_IPC_INTERRUPTS];
Uint16 g_usGetWriteIndexes[NUM_IPC_INTERRUPTS];
Uint16 g_usGetReadIndexes[NUM_IPC_INTERRUPTS];

//*****************************************************************************
//! Initializes C28 control system IPC driver controller
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param usCIpcInterrupt specifies the C28 IPC interrupt number used by
//! psController.
//! \param usMIpcInterrupt specifies the M3 IPC interrupt number used by
//! psController.
//!
//! This function initializes the IPC driver controller with circular buffer
//! and index addresses for an IPC interrupt pair (C28-M3). The
//! \e usCIpcInterrupt and \e usMIpcInterrupt parameters can be 1 of the
//! following values:
//! \b IPC_INT1, \b IPC_INT2, \b IPC_INT3, \b IPC_INT4.
//!
//! \note If an interrupt is currently in use by an \e tIpcController instance,
//! that particular interrupt should not be tied to a second \e tIpcController
//! instance.
//!
//! \note For a particular usMIpcInterrupt - usCIpcInterrupt pair, there must be
//! an instance of tIpcController defined and initialized on both the M3 and the
//! C28 systems.
//!
//! \return None.
//*****************************************************************************
void
IPCCInitialize (volatile tIpcController *psController, Uint16 usCIpcInterrupt,
                Uint16 usMIpcInterrupt)
{
    // CtoMPutBuffer and Index Initialization
    psController->psPutBuffer = &g_asIPCCtoMBuffers[usMIpcInterrupt-1][0];
    psController->pusPutWriteIndex = &g_usPutWriteIndexes[usMIpcInterrupt-1];
    psController->pusPutReadIndex = &g_usPutReadIndexes[usMIpcInterrupt-1];
    psController->ulPutFlag = (Uint32)(1 << (usMIpcInterrupt - 1));

    // MtoCGetBuffer and Index Initialization
    psController->psGetBuffer = &g_asIPCMtoCBuffers[usCIpcInterrupt-1][0];
    psController->pusGetWriteIndex = &g_usGetWriteIndexes[usCIpcInterrupt-1];
    psController->pusGetReadIndex = &g_usGetReadIndexes[usCIpcInterrupt-1];

    // Initialize PutBuffer WriteIndex = 0 and GetBuffer ReadIndex = 0
    //
    *(psController->pusPutWriteIndex) = 0;
    *(psController->pusGetReadIndex) = 0;

}

//*****************************************************************************
//! Writes a message into the PutBuffer.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the address of the \e tIpcMessage instance to be
//! written to PutBuffer.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a free slot (1= wait until free spot available, 0 = exit with
//! STATUS_FAIL if no free slot).
//!
//! This function checks if there is a free slot in the PutBuffer. If so, it
//! puts the message pointed to by \e psMessage into the free slot and
//! increments the WriteIndex. Then it sets the appropriate IPC interrupt flag
//! specified by \e psController->usPutFlag.  The \e bBlock parameter can be
//! one of the following values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return \b STATUS_FAIL if PutBuffer is full. \b STATUS_PASS if Put occurs
//! successfully.
//*****************************************************************************
Uint16
IpcPut (volatile tIpcController *psController, tIpcMessage *psMessage,
        Uint16 bBlock)
{
    Uint16 writeIndex;
    Uint16 readIndex;

    writeIndex = *(psController->pusPutWriteIndex);
    readIndex = *(psController->pusPutReadIndex);

    // Wait until Put Buffer slot is free
    //
    while (((writeIndex + 1) & MAX_BUFFER_INDEX) == readIndex)
    {
        // If designated as a "Blocking" function, and Put buffer is full,
        // return immediately with fail status.
        //
        if (!bBlock)
        {
            return (STATUS_FAIL);
        }

        readIndex = *(psController->pusPutReadIndex);
    }

    // When slot is free, Write Message to PutBuffer, update PutWriteIndex,
    // and set M3 IPC INT Flag
    //
    psController->psPutBuffer[writeIndex] = *psMessage;

    writeIndex = (writeIndex + 1) & MAX_BUFFER_INDEX;
    *(psController->pusPutWriteIndex) = writeIndex;

    CtoMIpcRegs.CTOMIPCSET.all |= psController->ulPutFlag;

    return (STATUS_PASS);
}

//*****************************************************************************
//! Reads a message from the GetBuffer.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the address of the \e tIpcMessage instance where
//! the message from GetBuffer should be written to.
//! \param bBlock specifies whether to allow function to block until GetBuffer
//! has a message (1= wait until message available, 0 = exit with STATUS_FAIL
//! if no message).
//!
//! This function checks if there is a message in the GetBuffer. If so, it gets
//! the message in the GetBuffer pointed to by the ReadIndex and writes it to
//! the address pointed to by \e psMessage. The \e bBlock parameter can be one
//! of the following
//! values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return \b STATUS_PASS if GetBuffer is empty. \b STATUS_FAIL if Get occurs
//! successfully.
//*****************************************************************************
Uint16
IpcGet (volatile tIpcController *psController, tIpcMessage *psMessage,
        Uint16 bBlock)
{
    Uint16 writeIndex;
    Uint16 readIndex;

    writeIndex = *(psController->pusGetWriteIndex);
    readIndex = *(psController->pusGetReadIndex);

    // Loop while GetBuffer is empty
    //
    while (writeIndex == readIndex)
    {
        // If designated as a "Blocking" function, and Get buffer is empty,
        // return immediately with fail status.
        //
        if (!bBlock)
        {
            return (STATUS_FAIL);
        }

        writeIndex = *(psController->pusGetWriteIndex);
    }

    // If there is a message in GetBuffer, Read Message and update the ReadIndex
    //
    *psMessage = psController->psGetBuffer[readIndex];

    readIndex = (readIndex + 1) & MAX_BUFFER_INDEX;
    *(psController->pusGetReadIndex) = readIndex;

    return (STATUS_PASS);
}

//*****************************************************************************
//! Sends the command to read either a 16- or 32-bit data word from an
//! M3 master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 address to read from
//! \param pvData is a pointer to the 16/32-bit variable where read data will
//! be stored.
//! \param usLength designates 16- or 32-bit read (1 = 16-bit, 2 = 32-bit)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//! \param ulResponseFlag indicates the C to M Flag number mask used to report
//! when the read data is available at pvData. (\e ulResponseFlag MUST use IPC
//! flags 17-32, and not 1-16)
//!
//! This function will allow the C28 control system to send a 16/32-bit data
//! read command to the M3 master system and set a ResponseFlag to track the
//! status of the read.
//! The M3 master system will respond with a DataWrite command which will place
//! the data in the C28 address pointed to by \e pvData. When the C28 receives
//! the DataWrite command and writes the read data into \e *pvData location,
//! it will clear the ResponseFlag, indicating to the rest of the system that
//! the data is ready. The \e usLength parameter can be one of the
//! following values: \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e
//! bBlock parameter can be one of the following values: \b ENABLE_BLOCKING or
//! \b DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMDataRead (volatile tIpcController *psController, Uint32 ulAddress,
                 void *pvData, Uint16 usLength, Uint16 bBlock,
                 Uint32 ulResponseFlag)
{

    Uint16 status;
    tIpcMessage sMessage;

    // Set up read command, address, dataw1 = ResponseFlag | word length, dataw2
    // = address where word
    // should be written to when returned.
    sMessage.ulcommand = IPC_DATA_READ;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (ulResponseFlag & 0xFFFF0000)|(Uint32)usLength;
    sMessage.uldataw2 = (Uint32)pvData;

    // Set ResponseFlag (cleared once data is read into address at pvData)
    // Put Message into PutBuffer and set IPC INT flag
    CtoMIpcRegs.CTOMIPCSET.all |= (ulResponseFlag & 0xFFFF0000);
    status = IpcPut (psController, &sMessage, bBlock);

    return status;
    /* Note: Read Response will have sMessage.ulcommand = IPC_DATA_WRITE
     *                               sMessage.uladdress = (Uint32) pvData
     *                               sMessage.uldataw1  = ulStatusFlag |
     *                                                    (Uint32) usLength;
     *                               sMessage.uldataw2  = word to be read into
     *                                                    pvData address.
     */

}

//*****************************************************************************
//! Sends the command to read either a 16- or 32-bit data word from an
//! M3 master system address to a write-protected C28 address.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 address to read from
//! \param pvData is a pointer to the 16/32-bit variable where read data will
//! be stored.
//! \param usLength designates 16- or 32-bit read (1 = 16-bit, 2 = 32-bit)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//! \param ulResponseFlag indicates the C to M Flag number mask used to report
//! when the read data is available at pvData. (\e ulResponseFlag MUST use IPC
//! flags 17-32, and not 1-16)
//!
//! This function will allow the C28 control system to send a 16/32-bit data
//! read command to the M3 master system and set a ResponseFlag to track the
//! status of the read.
//! The M3 master system will respond with a DataWrite command which will place
//! the data in the C28 address pointed to by \e pvData. When the C28 receives
//! the DataWrite command and writes the read data into \e *pvData location, it
//! will clear the ResponseFlag, indicating to the rest of the system that the
//! data is ready. The \e usLength parameter can be one of the following
//! values: \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock
//! parameter can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMDataRead_Protected (volatile tIpcController *psController,
                           Uint32 ulAddress, void *pvData, Uint16 usLength,
                           Uint16 bBlock,
                           Uint32 ulResponseFlag)
{

    Uint16 status;
    tIpcMessage sMessage;

    // Set up read command, address, dataw1 = ResponseFlag | word length, dataw2
    // = address where word
    // should be written to when returned.
    sMessage.ulcommand = IPC_DATA_READ_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (ulResponseFlag & 0xFFFF0000)|(Uint32)usLength;
    sMessage.uldataw2 = (Uint32)pvData;

    // Set ResponseFlag (cleared once data is read into address at pvData)
    // Put Message into PutBuffer and set IPC INT flag
    CtoMIpcRegs.CTOMIPCSET.all |= (ulResponseFlag & 0xFFFF0000);
    status = IpcPut (psController, &sMessage, bBlock);

    return status;
    /* Note: Read Response will have sMessage.ulcommand = IPC_DATA_WRITE
     *                               sMessage.uladdress = (Uint32) pvData
     *                               sMessage.uldataw1  = ulStatusFlag |
     *                                                    (Uint32) usLength;
     *                               sMessage.uldataw2  = word to be read into
     *                                                    pvData address.
     */

}

//*****************************************************************************
//! Sets the designated bits in a 16-bit data word at an M3 master system
//! address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! \e ulAddress.
//! 16-bit masks should fill the lower 16-bits (upper 16-bits will be all
//! 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to set bits specified by the
//! \e ulMask variable in a 16/32-bit word on the M3 master system. The \e
//! usLength parameter can be one of the following values: \b IPC_LENGTH_16_BITS
//! or \b IPC_LENGTH_32_BITS. The \e bBlock parameter can be one of the
//! following values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMSetBits(volatile tIpcController *psController, Uint32 ulAddress,
               Uint32 ulMask, Uint16 usLength,
               Uint16 bBlock)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up set bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_SET_BITS;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (Uint32)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Sets the designated bits in a 16-bit write-protected data word at an M3
//! master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! \e ulAddress. 16-bit masks should fill the lower 16-bits (upper 16-bits
//! will be all 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to set bits specified by the
//! \e ulMask variable in a write-protected 16/32-bit word on the M3 master
//! system. The \e usLength parameter can be one of the  following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter can be
//! one of the following values:
//! \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMSetBits_Protected(volatile tIpcController *psController,
                         Uint32 ulAddress, Uint32 ulMask, Uint16 usLength,
                         Uint16 bBlock)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up set bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_SET_BITS_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (Uint32)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Clears the designated bits in a 16-bit data word at an M3 master system
//! address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be cleared
//! at \e ulAddress. 16-bit masks should fill the lower 16-bits (upper 16-bits
//! will be all 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to clear bits specified by
//! the \e ulMask variable in a 16/32-bit word on the M3 master system. The \e
//! usLength parameter can be one of the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter can be
//! one of the following values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMClearBits(volatile tIpcController *psController, Uint32 ulAddress,
                 Uint32 ulMask, Uint16 usLength,
                 Uint16 bBlock)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up clear bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_CLEAR_BITS;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (Uint32)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Clears the designated bits in a 16-bit write-protected data word at an M3
//! master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be cleared
//! at \e ulAddress. 16-bit masks should fill the lower 16-bits (upper 16-bits
//! will be all 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to set bits specified by the
//! \e ulMask variable in a write-protected 16/32-bit word on the M3 master
//! system. The \e usLength parameter can be  one of the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter can be
//! one of the following values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMClearBits_Protected(volatile tIpcController *psController,
                           Uint32 ulAddress, Uint32 ulMask, Uint16 usLength,
                           Uint16 bBlock)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up clear bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_CLEAR_BITS_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (Uint32)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to an M3 master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For 16-bit
//! words, only the lower 16-bits of ulData will be considered by the master
//! system.
//! \param usLength is the length of the word to write (1 = 16-bits, 2 =
//! 32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//! \param ulResponseFlag is used to pass the \e ulResponseFlag back to the M3
//! only when this function is called in response to \e IPCMtoCDataRead().
//! Otherwise, set to 0.
//!
//! This function will allow the C28 control system to write a 16/32-bit word
//! via the \e ulData variable to an address on the M3 master system.
//! The \e usLength parameter can be one of the following values:
//! \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMDataWrite(volatile tIpcController *psController, Uint32 ulAddress,
                 Uint32 ulData, Uint16 usLength, Uint16 bBlock,
                 Uint32 ulResponseFlag)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up write command, address, dataw1 = ResponseFlag | word length,
    // dataw2 = data to write
    sMessage.ulcommand = IPC_DATA_WRITE;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulResponseFlag |(Uint32)usLength;
    sMessage.uldataw2 = ulData;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to a write-protected M3 master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the write-protected M3 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For
//! 16-bit words, only the lower 16-bits of ulData will be considered by the
//! master system.
//! \param usLength is the length of the word to write (1 = 16-bits, 2 =
//! 32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//! \param ulResponseFlag is used to pass the \e ulResponseFlag back to the M3
//! only when this function is called in response to \e IPCMtoCDataRead().
//! Otherwise, set to 0.
//!
//! This function will allow the C28 control system to write a 16/32-bit word
//! via the \e ulData variable to a write-protected address on the M3 master
//! system. The \e usLength parameter can be one of the following values:
//!  \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 -
//! \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMDataWrite_Protected(volatile tIpcController *psController,
                           Uint32 ulAddress, Uint32 ulData, Uint16 usLength,
                           Uint16 bBlock,
                           Uint32 ulResponseFlag)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up write command, address, dataw1 = ResponseFlag | word length,
    // dataw2 = data to write
    sMessage.ulcommand = IPC_DATA_WRITE_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulResponseFlag |(Uint32)usLength;
    sMessage.uldataw2 = ulData;

    // Put Message into PutBuffer and set IPC INT flag

    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Sends the command to read a block of data from an M3 master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 memory block starting address to read
//! from.
//! \param ulShareAddress specifies the C28 shared memory address the read
//! block will read to.
//! \param usLength designates the block size in 16-bit words.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//! \param ulResponseFlag indicates the C to M Flag number mask used to report
//! when the read block data is available starting at /e ulShareAddress. (\e
//! ulResponseFlag MUST use IPC flags 17-32, and not 1-16)
//!
//! This function will allow the C28 control system to send a read block
//! command to the M3 master system and set a ResponseFlag to track the status
//! of the read. The M3 master system will process the read and place the data
//! in shared memory at the location specified in the \e ulShareAddress
//! parameter and then clear the ResponseFlag, indicating that the block is
//! ready. The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING. The \e ulResponseFlag parameter can
//! be any single one of the flags \b IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMBlockRead(volatile tIpcController *psController, Uint32 ulAddress,
                 Uint32 ulShareAddress, Uint16 usLength, Uint16 bBlock,
                 Uint32 ulResponseFlag)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up block read command, address, dataw1 = ResponseFlag | block length,
    // dataw2 = M3 address in shared memory
    // where block data should be read to (corresponding to C28 ulShareAddress).
    sMessage.ulcommand = IPC_BLOCK_READ;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (ulResponseFlag & 0xFFFF0000) |(Uint32)usLength;
    sMessage.uldataw2 = IPCCtoMSharedRamConvert(ulShareAddress);

    // Set ResponseFlag (cleared once data is read into Share Address location)
    // Put Message into PutBuffer and set IPC INT flag
    CtoMIpcRegs.CTOMIPCSET.all |= (ulResponseFlag & 0xFFFF0000);
    status = IpcPut (psController, &sMessage, bBlock);

    return status;
    /* Note: Read Block Response will occur in processing of ReadBlock (since M3
       has access to shared memory)
     */
}

//*****************************************************************************
//! Writes a block of data to an M3 master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 memory block starting address to write to.
//! \param ulShareAddress specifies the C28 shared memory address where data to
//! write from resides.
//! \param usLength designates the block size in 16- or 32-bit words (depends on
//! \e usWordLength).
//! \param usWordLength designates the word size (16-bits = 1 or 32-bits = 2).
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to write a block of data to
//! the M3 master system starting from the location specified by the
//! \e ulAdress parameter. Prior to calling this function, the C28 control
//! system code should place the data to write in shared memory starting at /e
//! ulShareAddress.
//! The \e usWordLength parameter can be one of the following values:
//! \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \note If the Sx SARAM blocks are used to pass the RAM block between the
//! processors, the IPCCtoMReqMemAccess() function must be called prior to the
//! C28 writing the block to /e ulShareAddress in order to give the C28 write-
//! access to the Sx memory block(s) used for passing data.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMBlockWrite(volatile tIpcController *psController, Uint32 ulAddress,
                  Uint32 ulShareAddress, Uint16 usLength, Uint16 usWordLength,
                  Uint16 bBlock)
{

    Uint16 status;
    tIpcMessage sMessage;

    // Set up block write command, address, dataw1 = block length, dataw2 = M3
    // shared mem address
    // where write data resides
    sMessage.ulcommand = IPC_BLOCK_WRITE;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ((Uint32)(usWordLength)<<16) + (Uint32)usLength;
    sMessage.uldataw2 = IPCCtoMSharedRamConvert(ulShareAddress);

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;

}

//*****************************************************************************
//! Writes a block of data to a write-protected M3 master system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the write-protected M3 memory block starting
//! address to write to.
//! \param ulShareAddress specifies the C28 shared memory address where data to
//! write from resides.
//! \param usLength designates the block size in 16- or 32-bit words (depends on
//! \e usWordLength).
//! \param usWordLength designates the word size (16-bits = 1 or 32-bits = 2).
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to write a block of data to
//! a write-protected region on the M3 master system starting from the location
//! specified by the \e ulAdress parameter. Prior to calling this function,
//! the C28 control system code should place the data to write in shared memory
//! starting at /e ulShareAddress.
//! The \e usWordLength parameter can be one of the following values:
//! \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \note If the Sx SARAM blocks are used to pass the RAM block between the
//! processors, the IPCCtoMReqMemAccess() function must be called prior to the
//! C28 writing the block to /e ulShareAddress in order to give the C28 write \
// //! access to the Sx memory block(s) used for passing data.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMBlockWrite_Protected(volatile tIpcController *psController,
                            Uint32 ulAddress, Uint32 ulShareAddress,
                            Uint16 usLength, Uint16 usWordLength,
                            Uint16 bBlock)
{

    Uint16 status;
    tIpcMessage sMessage;

    // Set up block write command, address, dataw1 = block length, dataw2 = M3
    // shared mem address
    // where write data resides
    sMessage.ulcommand = IPC_BLOCK_WRITE_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ((Uint32)(usWordLength)<<16) + (Uint32)usLength;
    sMessage.uldataw2 = IPCCtoMSharedRamConvert(ulShareAddress);

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;

}

//*****************************************************************************
//! Calls an M3 function with 1 optional parameter .
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the M3 function address
//! \param ulParam specifies the 32-bit optional parameter value. If not used,
//! this can be a dummy value.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to call a function on the
//! M3. The \e ulParam variable is a single optional 32-bit parameter to pass
//! to the function. The \e bBlock parameter can be one of the following values:
//! \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMFunctionCall(volatile tIpcController *psController, Uint32 ulAddress,
                    Uint32 ulParam,
                    Uint16 bBlock)
{
    Uint16 status;
    tIpcMessage sMessage;

    // Set up function call command, address, dataw1 = 32-bit parameter
    sMessage.ulcommand = IPC_FUNC_CALL;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulParam;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Sends generic message to M3 master system
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulCommand specifies 32-bit command word to insert into message.
//! \param ulAddress specifies 32-bit address word to insert into message.
//! \param ulDataW1 specifies 1st 32-bit data word to insert into message.
//! \param ulDataW2 specifies 2nd 32-bit data word to insert into message.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to send a generic message to
//! the M3 master system. Note that the user should create a corresponding
//! function on the M3 side to interpret/use the message or fill parameters in
//! such a way that the existing IPC drivers can recognize the command and
//! properly process the message.
//! The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMSendMessage(volatile tIpcController *psController, Uint32 ulCommand,
                   Uint32 ulAddress, Uint32 ulDataW1, Uint32 ulDataW2,
                   Uint16 bBlock)
{

    Uint16 status;
    tIpcMessage sMessage;

    // Package message to send
    sMessage.ulcommand = ulCommand;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulDataW1;
    sMessage.uldataw2 = ulDataW2;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! C28 Configures master R/W/Exe Access to Shared SARAM.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulMask specifies the 32-bit mask for the SxMSEL RAM control register
//! to indicate which Sx SARAM blocks the C28 is requesting master access to.
//! \param usMaster specifies whether the C28 or M3 are given master access to
//! the Sx blocks.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the C28 control system to configure master R/W/Exe
//! access to the Sx SARAM blocks specified by the /e ulMask parameter.  The
//! function calls the \e IPCCtoMSetBits_Protected() or \e
//! IPCCtoMClearBits_Protected() functions, and therefore in the M3 application
//! code, the corresponding functions should be called.
//! The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING. The \e usMaster parameter can be
//! either: \b IPC_SX_C28MASTER or \b IPC_SX_M3MASTER. The \e ulMask parameter
//! can be any of the options: \b S0_ACCESS - \b S7_ACCESS.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
Uint16
IPCCtoMReqMemAccess (volatile tIpcController *psController, Uint32 ulMask,
                     Uint16 usMaster,
                     Uint16 bBlock)
{

    Uint16 status = STATUS_PASS;

    if (usMaster == IPC_SX_C28MASTER)
    {
        if ((RAMRegs.CSxMSEL.all & ulMask) != ulMask)
        {
            status =
                IPCCtoMSetBits_Protected (psController, MSXMSEL_M3REG, ulMask,
                                          IPC_LENGTH_32_BITS,
                                          bBlock);
        }
    }
    else if (usMaster == IPC_SX_M3MASTER)
    {
        if ((RAMRegs.CSxMSEL.all & ulMask) != 0)
        {
            status =
                IPCCtoMClearBits_Protected (psController, MSXMSEL_M3REG, ulMask,
                                            IPC_LENGTH_32_BITS,
                                            bBlock);
        }
    }

    return status;
}

//*****************************************************************************
//! Responds to 16/32-bit data word write command from M3 master system
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system which includes the 16/32-bit data word to write to the C28
//! address.
//!
//! This function will allow the C28 control system to write a 16/32-bit word
//! received from the M3 master system to the address indicated in \e
//! *psMessage. In the event that the IPC_DATA_WRITE command was received as a
//! result of an IPC_DATA_READ command, this function will also clear the IPC
//! response flag tracking the read so other threads in the C28 system will be
//! aware that the data is ready.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCDataWrite(tIpcMessage *psMessage)
{
    // Data word length = dataw1 (15:0), responseFlag = valid only for IPC flags
    // 17-32
    //
    Uint16 length = (Uint16) psMessage->uldataw1;
    Uint32 responseFlag = (psMessage->uldataw1) & 0xFFFF0000;

    // Write 16/32-bit word to address
    //
    if (length == IPC_LENGTH_16_BITS)
    {
        *(Uint16 *)(psMessage->uladdress) = (Uint16)psMessage->uldataw2;
    }
    else if (length == IPC_LENGTH_32_BITS)
    {
        *(Uint32 *)(psMessage->uladdress) = psMessage->uldataw2;
    }

    // If data write command is in response to a data read command from C28
    // to M3, clear ResponseFlag, indicating read data from M3 is ready.
    //
    CtoMIpcRegs.CTOMIPCCLR.all |= responseFlag;
}

//*****************************************************************************
//! Responds to 16/32-bit write-protected data word write command from
//! M3 master system
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system which includes the 16/32-bit data word to write to the C28
//! address.
//!
//! This function will allow the C28 control system to write a 16/32-bit word
//! received from the M3 master system to the write-protected address indicated
//! in \e *psMessage.
//! In the event that the IPC_DATA_WRITE_PROTECTED command was received as a
//! result of an IPC_DATA_READ_PROTECTED command, this function will also clear
//! the IPC response flag tracking the read so other threads in the C28 system
//! will be aware that the data is ready.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCDataWrite_Protected(tIpcMessage *psMessage)
{
    // Data word length = dataw1 (15:0), responseFlag = valid only for IPC flags
    // 17-32
    //
    Uint16 length = (Uint16) psMessage->uldataw1;
    Uint32 responseFlag = (psMessage->uldataw1) & 0xFFFF0000;

    // Allow access to EALLOW-protected registers.
    EALLOW;

    // Write 16/32-bit word to EALLOW-protected address
    //
    if (length == IPC_LENGTH_16_BITS)
    {
        *(Uint16 *)(psMessage->uladdress) = (Uint16)psMessage->uldataw2;
    }
    else if (length == IPC_LENGTH_32_BITS)
    {
        *(Uint32 *)(psMessage->uladdress) = psMessage->uldataw2;
    }

    // Disable access to EALLOW-protected registers.
    //
    EDIS;

    // If data write command is in response to a data read command from C28
    // to M3, clear ResponseFlag, indicating read data from M3 is ready.
    //
    CtoMIpcRegs.CTOMIPCCLR.all |= responseFlag;
}

//*****************************************************************************
//! Responds to 16/32-bit data word read command from M3 master system.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the M3 master system to read a 16/32-bit data
//! word at the C28 address specified in /e psMessage, and send a Write command
//! with the read data back to the C28 control system. It will also send the
//! Response Flag used to track the read back to the M3.
//! The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCDataRead(volatile tIpcController *psController, tIpcMessage *psMessage,
                Uint16 bBlock)
{
    unsigned long ulReaddata;
    Uint16 usLength;

    // If data word length = 16-bits, read the 16-bit value at the given address
    // and cast as
    // 32-bit word to send back to M3.
    // If data word length = 32-bits, read the 32-bit value at the given
    // address.
    //
    usLength = (Uint16)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        ulReaddata = (unsigned long)(*(volatile Uint16 *)psMessage->uladdress);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        ulReaddata = *(unsigned long *)psMessage->uladdress;
    }

    // Send a Write command to write the requested data to the M3 read into
    // address.
    // psMessage->uldataw2 contains M3 address where readdata will be written.
    // psMessage->uldataw1 contains the read response flag in IPC flag 17-32.
    IPCCtoMDataWrite(psController, psMessage->uldataw2, ulReaddata, usLength,
                     bBlock,
                     (psMessage->uldataw1 & 0xFFFF0000));
}

//*****************************************************************************
//! Responds to 16/32-bit data word read command from M3 master system.
//! to read into a write-protected word on the M3 system.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the M3 master system to read a 16/32-bit data
//! word at the C28 address specified in /e psMessage, and send a Write
//! Protected command with the read data back to the M3 master system at a write
//! protected address. It will also send the Response Flag used to track the
//! read back to the M3. The \e bBlock parameter can be one of the following
//! values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCDataRead_Protected(volatile tIpcController *psController,
                          tIpcMessage *psMessage,
                          Uint16 bBlock)
{
    unsigned long ulReaddata;
    Uint16 usLength;

    // If data word length = 16-bits, read the 16-bit value at the given address
    // and cast as
    // 32-bit word to send back to M3.
    // If data word length = 32-bits, read the 32-bit value at the given
    // address.
    //
    usLength = (Uint16)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        ulReaddata = (unsigned long)(*(volatile Uint16 *)psMessage->uladdress);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        ulReaddata = *(unsigned long *)psMessage->uladdress;
    }

    // Send a Write command to write the requested data to the M3 read into
    // address.
    // psMessage->uldataw2 contains M3 address where readdata will be written.
    // psMessage->uldataw1 contains the read response flag in IPC flag 17-32.
    IPCCtoMDataWrite_Protected(psController, psMessage->uldataw2, ulReaddata,
                               usLength, bBlock,
                               (psMessage->uldataw1 & 0xFFFF0000));
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at a C28 control system
//! address
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//!
//! This function will allow the M3 master system to set the bits in a 16/32-bit
//! word on the C28 control system via a C28 address and mask passed in via
//! the \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCSetBits(tIpcMessage *psMessage)
{
    Uint16 usLength;

    // Determine length of word at psMessage->uladdress and then set bits based
    // on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    //
    usLength = (Uint16)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile Uint16*)psMessage->uladdress |= (Uint16) psMessage->uldataw2;
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress |=  psMessage->uldataw2;
    }
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit write-protected data word at a
//! C28 control system address
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//!
//! This function will allow the M3 master system to set the bits in a write-
//! protected 16/32-bit word on the C28 control system via a C28 address and
//! mask passed in via the \e psMessage.
//!
//! \return None
//*****************************************************************************
void
IPCMtoCSetBits_Protected(tIpcMessage *psMessage)
{
    Uint16 usLength;

    // Allow access to EALLOW-protected registers.
    EALLOW;

    // Determine length of word at psMessage->uladdress and then set bits based
    // on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    //
    usLength = (Uint16)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile Uint16*)psMessage->uladdress |= (Uint16) psMessage->uldataw2;
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress |= psMessage->uldataw2;
    }

    // Disable access to EALLOW-protected registers.
    EDIS;
}

//*****************************************************************************
//! Clears the designated bits in a 32-bit data word at a C28 control system
//! address
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//!
//! This function will allow the M3 master system to clear the bits in a
//! 16/32-bit word on the C28 control system via a C28 address and mask passed
//! in via the \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCClearBits(tIpcMessage *psMessage)
{

    Uint16 usLength;

    // Determine length of word at psMessage->uladdress and then clear bits
    // based on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    //
    usLength = (Uint16)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile Uint16*)psMessage->uladdress &=
            ~((Uint16) psMessage->uldataw2);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress &=
            ~(psMessage->uldataw2);
    }
}

//*****************************************************************************
//! Clears the designated bits in a write-protected 16/32-bit data word at a C28
//! control system address
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//!
//! This function will allow the M3 master system to clear the bits in a
//! 16/32-bit write-protected  word on the C28 control system via a C28 address
//! and mask passed in via the \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCClearBits_Protected(tIpcMessage *psMessage)
{

    Uint16 usLength;

    // Allow access to EALLOW-protected registers.
    EALLOW;

    // Determine length of word at psMessage->uladdress and then clear bits
    // based on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    //
    usLength = (Uint16)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile Uint16*)psMessage->uladdress &=
            ~((Uint16) psMessage->uldataw2);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress &=
            ~(psMessage->uldataw2);
    }

    // Disable access to EALLOW-protected registers.
    EDIS;
}

//*****************************************************************************
//! Reads a block of data from a M3 master system address and stores into
//! shared RAM
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//!
//! This function will respond to the M3 master system request to read a block
//! of data from the C28 control system, by reading the data and placing that
//! data into the shared RAM location specified in \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCBlockRead(tIpcMessage *psMessage)
{

    Uint16 usLength;
    volatile Uint16* pusRAddress;
    volatile Uint16* pusWAddress;
    Uint16 usIndex;

    pusRAddress = (volatile Uint16 *)psMessage->uladdress;
    pusWAddress = (volatile Uint16 *)psMessage->uldataw2;
    usLength = (Uint16)psMessage->uldataw1;

    for (usIndex=0; usIndex<usLength; usIndex++)
    {
        *pusWAddress = *pusRAddress;
        pusWAddress += 1;
        pusRAddress += 1;
    }

    CtoMIpcRegs.MTOCIPCACK.all |= (psMessage->uldataw1 & 0xFFFF0000);
}

//*****************************************************************************
//! Writes a block of data to a C28 control system address from shared RAM
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//!
//! This function will write a block of data to an address on the C28 control
//! system.
//! The data is first written by the M3 to shared RAM. This function reads the
//! shared RAM location, word size (16- or 32-bit), and block size from \e
//! psMessage and writes the block to the C28 address specified in \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCBlockWrite(tIpcMessage *psMessage)
{
    Uint16 usLength;
    Uint16 usWLength;
    Uint16 usIndex;

    usLength = (Uint16)psMessage->uldataw1;
    usWLength = (Uint16)((psMessage->uldataw1)>>16);

    // Determine data word access size to write to data block.
    //
    if (usWLength == IPC_LENGTH_16_BITS)
    {
        volatile Uint16 *pusWAddress = (volatile Uint16 *)psMessage->uladdress;
        volatile Uint16 *pusRAddress = (volatile Uint16 *)psMessage->uldataw2;
        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pusWAddress = *pusRAddress;
            pusWAddress += 1;
            pusRAddress += 1;
        }
    } else if (usWLength == IPC_LENGTH_32_BITS)
    {
        volatile unsigned long *pulWAddress =
            (volatile unsigned long *)psMessage->uladdress;
        volatile unsigned long *pulRAddress =
            (volatile unsigned long *)psMessage->uldataw2;

        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pulWAddress = *pulRAddress;
            pulWAddress += 1;
            pulRAddress += 1;
        }
    }
}

//*****************************************************************************
//! Writes a block of data to a C28 control system write-protected address from
//! shared RAM
//!
//! \param psMessage specifies the pointer to the message received from the M3
//! master system.
//!
//! This function will write a block of data to a write-protected group of
//! addresses on the C28 control system. The data is first written by the
//! M3 to shared RAM. This function reads the shared RAM location, word size
//! (16- or 32-bit), and block size from \e psMessage  and writes the block to
//! the C28 address specified in \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCBlockWrite_Protected(tIpcMessage *psMessage)
{
    Uint16 usLength;
    Uint16 usWLength;
    Uint16 usIndex;

    // Allow access to EALLOW-protected registers.
    EALLOW;

    usLength = (Uint16)psMessage->uldataw1;
    usWLength = (Uint16)((psMessage->uldataw1)>>16);

    // Determine data word access size to write to data block.
    // (Writes registers accessible via APB bus must be 32-bits wide)
    if (usWLength == IPC_LENGTH_16_BITS)
    {
        volatile Uint16 *pusWAddress = (volatile Uint16 *)psMessage->uladdress;
        volatile Uint16 *pusRAddress = (volatile Uint16 *)psMessage->uldataw2;
        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pusWAddress = *pusRAddress;
            pusWAddress += 1;
            pusRAddress += 1;
        }
    } else if (usWLength == IPC_LENGTH_32_BITS)
    {
        volatile unsigned long *pulWAddress =
            (volatile unsigned long *)psMessage->uladdress;
        volatile unsigned long *pulRAddress =
            (volatile unsigned long *)psMessage->uldataw2;

        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pulWAddress = *pulRAddress;
            pulWAddress += 1;
            pulRAddress += 1;
        }
    }

    // Disable access to EALLOW-protected registers.
    EDIS;
}

//*****************************************************************************
//! Calls a C28 function with a single optional parameter.
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will allow the M3 master system to call a C28 function with a
//! a single optional parameter. There is no return value from the executed
//! function.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCFunctionCall(tIpcMessage *psMessage)
{
    // Executes function call with parameter at given address.
    //
    tfIpcFuncCall func_call = (tfIpcFuncCall)psMessage->uladdress;
    func_call(psMessage->uldataw1);
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************


