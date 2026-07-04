//###########################################################################
// FILE:    F28M35x_Ipc_drivers.h
// TITLE:   Defines and Macros for the IPC Controller
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_IPC_DRIVERS_H
#define F28M35x_IPC_DRIVERS_H

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
// The following are values that are used to define the maximum size of the
// global circular buffer variables g_asIPCCtoMBuffers and g_asIPCMtoCBuffers.
// They are also used in the IpcPut() and IpcGet() functions.
// IPC_BUFFER_SIZE and NUM_IPC_INTERRUPTS are user-configurable.
//*****************************************************************************
#define IPC_BUFFER_SIZE             4     // # of tIpcMessage messages in
                                          // circular buffer
                                          // (must be interval of 2)
#define NUM_IPC_INTERRUPTS          4     // # of IPC interrupts using circular
                                          // buffer
                                          // (must be same number on both C28
                                          // and M3)

#define MAX_BUFFER_INDEX            IPC_BUFFER_SIZE - 1

//*****************************************************************************
// The following is used by IPCMtoCSharedRamConvert() function to
// calculate the C28 vs M3 conversion address for MSGRAM vs Sx memory.
// Note: THIS DEFINITION COULD CHANGE FOR FUTURE DEVICE FAMILIES
//*****************************************************************************
#define C28_CTOMMSGRAM_START        0x0003F800

//*****************************************************************************
// The following are values that can be passed to IPCCInitialize()
// as the usMIpcInterrupt and usCIpcInterrupt parameters
//*****************************************************************************
#define IPC_INT1                    0x0001
#define IPC_INT2                    0x0002
#define IPC_INT3                    0x0003
#define IPC_INT4                    0x0004

//*****************************************************************************
// The following are values that can be passed to IPCCtoMTaskBusy() as the
// ulFlags parameter.
// IPC_FLAG17 - IPC_FLAG32 can also be passed to IPCtoMDataRead() and
// IPCCtoMReadBlock() as the ulResponseFlag parameter.
//*****************************************************************************
#define NO_FLAG                     0x00000000  // NO FLAG
#define IPC_FLAG1                   0x00000001  // IPC FLAG 1
#define IPC_FLAG2                   0x00000002  // IPC FLAG 2
#define IPC_FLAG3                   0x00000004  // IPC FLAG 3
#define IPC_FLAG4                   0x00000008  // IPC FLAG 4
#define IPC_FLAG5                   0x00000010  // IPC FLAG 5
#define IPC_FLAG6                   0x00000020  // IPC FLAG 6
#define IPC_FLAG7                   0x00000040  // IPC FLAG 7
#define IPC_FLAG8                   0x00000080  // IPC FLAG 8
#define IPC_FLAG9                   0x00000100  // IPC FLAG 9
#define IPC_FLAG10                  0x00000200  // IPC FLAG 10
#define IPC_FLAG11                  0x00000400  // IPC FLAG 11
#define IPC_FLAG12                  0x00000800  // IPC FLAG 12
#define IPC_FLAG13                  0x00001000  // IPC FLAG 13
#define IPC_FLAG14                  0x00002000  // IPC FLAG 14
#define IPC_FLAG15                  0x00004000  // IPC FLAG 15
#define IPC_FLAG16                  0x00008000  // IPC FLAG 16
#define IPC_FLAG17                  0x00010000  // IPC FLAG 17
#define IPC_FLAG18                  0x00020000  // IPC FLAG 18
#define IPC_FLAG19                  0x00040000  // IPC FLAG 19
#define IPC_FLAG20                  0x00080000  // IPC FLAG 20
#define IPC_FLAG21                  0x00100000  // IPC FLAG 21
#define IPC_FLAG22                  0x00200000  // IPC FLAG 22
#define IPC_FLAG23                  0x00400000  // IPC FLAG 23
#define IPC_FLAG24                  0x00800000  // IPC FLAG 24
#define IPC_FLAG25                  0x01000000  // IPC FLAG 25
#define IPC_FLAG26                  0x02000000  // IPC FLAG 26
#define IPC_FLAG27                  0x04000000  // IPC FLAG 27
#define IPC_FLAG28                  0x08000000  // IPC FLAG 28
#define IPC_FLAG29                  0x10000000  // IPC FLAG 29
#define IPC_FLAG30                  0x20000000  // IPC FLAG 30
#define IPC_FLAG31                  0x40000000  // IPC FLAG 31
#define IPC_FLAG32                  0x80000000  // IPC FLAG 32

//*****************************************************************************
// The following are values that are returned from all of the IPCCtoM command
// functions to determine whether the command was successfully sent or not.
//*****************************************************************************
#define STATUS_FAIL                 0x0001
#define STATUS_PASS                 0x0000

//*****************************************************************************
// The following are values that can be passed to IPCCtoMReqMemAccess() as
// ulMask parameter to configure CSXMSEL register for master access to Sx
// RAM block.
//*****************************************************************************
#define S0_ACCESS                   0x00000001   // Master Access to S0
#define S1_ACCESS                   0x00000002   // Master Access to S1
#define S2_ACCESS                   0x00000004   // Master Access to S2
#define S3_ACCESS                   0x00000008   // Master Access to S3
#define S4_ACCESS                   0x00000010   // Master Access to S4
#define S5_ACCESS                   0x00000020   // Master Access to S5
#define S6_ACCESS                   0x00000040   // Master Access to S6
#define S7_ACCESS                   0x00000080   // Master Access to S7

//*****************************************************************************
// The following is the address of the MSXMSEL register on the M3 Master
// system used by IPCCtoMReqMemAccess() to configure master access to Sx
// RAM block.
// Note: THIS DEFINITION COULD CHANGE FOR FUTURE DEVICE FAMILIES
//*****************************************************************************
#define MSXMSEL_M3REG               0x400FB210   // Address of MSXMSEL Register
                                                 // on M3

//*****************************************************************************
// The following are values that can be passed to IPCCtoMReqMemAccess() as
// usMaster parameter to determine which processor to give master access to
// Sx blocks.
//*****************************************************************************
#define IPC_SX_C28MASTER            0x0001
#define IPC_SX_M3MASTER             0x0000

//*****************************************************************************
// The following are values that can be passed to IPCCtoMDataRead(),
// IPCCCtoMSetBits(),IPCCCtoMSetBits_Protected(), IPCCCtoMClearBits(),
// IPCCCtoMClearBits_Protected(), IPCCCtoMDataWrite(),
// IPCCtoMDataWrite_Protected()
// as usLength parameter to determine whether command applies to 16- or 32-bit
// data word.
//*****************************************************************************
#define IPC_LENGTH_16_BITS          0x00000001
#define IPC_LENGTH_32_BITS          0x00000002

//*****************************************************************************
// The following are values that can be passed to all IPCCtoM command functions
// as bBlock parameter to determine whether to wait/block until a slot in
// PutBuffer is available if it is full, or to exit with a failure status.
//*****************************************************************************
#define ENABLE_BLOCKING             0x0001
#define DISABLE_BLOCKING            0x0000

//*****************************************************************************
// The following are values that are used by all command functions and passed
// between processors in tIpcMessage.ulmessage or in the xTOyIPCCOM register
// to determine what command is requested by the sending processor.
//*****************************************************************************
#define IPC_SET_BITS_16                 0x00000001 // Used for IPC-Lite
#define IPC_SET_BITS_32                 0x00000002 // Used for IPC-Lite
#define IPC_CLEAR_BITS_16               0x00000003 // Used for IPC-Lite
#define IPC_CLEAR_BITS_32               0x00000004 // Used for IPC-Lite
#define IPC_DATA_WRITE_16               0x00000005 // Used for IPC-Lite
#define IPC_DATA_WRITE_32               0x00000006 // Used for IPC-Lite
#define IPC_DATA_READ_16                0x00000007 // Used for C28 Boot ROM
#define IPC_DATA_READ_32                0x00000008 // Used for C28 Boot ROM
#define IPC_DATA_READ                   0x00000008
#define IPC_SET_BITS_16_PROTECTED       0x00000009 // Used for IPC-Lite
#define IPC_SET_BITS_32_PROTECTED       0x0000000A // Used for IPC-Lite
#define IPC_CLEAR_BITS_16_PROTECTED     0x0000000B // Used for IPC-Lite
#define IPC_CLEAR_BITS_32_PROTECTED     0x0000000C // Used for IPC-Lite
#define IPC_DATA_WRITE_16_PROTECTED     0x0000000D // Used for IPC-Lite
#define IPC_DATA_WRITE_32_PROTECTED     0x0000000E // Used for IPC-Lite
// 0x0000000F and 0x0000010 are reserved by C28x boot ROM
#define IPC_BRANCH                      0x00000011
#define IPC_FUNC_CALL                   0x00000012
#define IPC_MTOC_EXECUTE_BOOTMODE_CMD   0x00000013

#define IPC_SET_BITS                    0x00010001
#define IPC_CLEAR_BITS                  0x00010002
#define IPC_DATA_WRITE                  0x00010003
#define IPC_BLOCK_READ                  0x00010004
#define IPC_BLOCK_WRITE                 0x00010005
#define IPC_DATA_READ_PROTECTED         0x00010007
#define IPC_SET_BITS_PROTECTED          0x00010008
#define IPC_CLEAR_BITS_PROTECTED        0x00010009
#define IPC_DATA_WRITE_PROTECTED        0x0001000A
#define IPC_BLOCK_WRITE_PROTECTED       0x0001000B

// C28 control system boot mode definitions (write to MTOCIPCBOOTCMD register)
#define CBROM_MTOC_BOOTMODE_COMMAND_ILLEGAL             0x00000000
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_RAM               0x00000001
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH             0x00000002
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_SCI               0x00000003
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_SPI               0x00000004
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_I2C               0x00000005
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_PARALLEL          0x00000006

//*****************************************************************************
// C28 control system boot rom status sent to M3 via CTOMIPCCOM register
//*****************************************************************************
#define ILLEGAL_COMMAND                                 0x00000000  
// Invalid command received by C28 Boot ROM
#define C_BOOTROM_IPC_CTOM_CONTROL_SYSTEM_IN_ITRAP      0x00000002  
// Control CPU received ITRAP, address where ITRAP occured is returned in 
// CTOMIPCADDR register
#define C_BOOTROM_IPC_CTOM_PIE_INTERRUPT_NOT_SUPPORTED  0x00000003  
// Control CPU received spurious PIE interrupt, Interrupt # is returned in
//CTOMIPCDATAW register
#define C_BOOTROM_IPC_CTOM_PIE_VECTOR_ADDRESS_MISMATCH  0x00000004
#define C_BOOTROM_IPC_CTOM_CONTROL_SYSTEM_IN_FLUNCERR   0x00000005
#define C_BOOTROM_IPC_CTOM_CONTROL_SYSTEM_IN_RAMUNCERR  0x00000006

//*****************************************************************************
//! A structure that defines an IPC message.  These fields are used by the
//! IPC drivers to determine handling of data passed between processors.
//! Although they have a defined naming scheme, they can also be used
// generically
//! to pass 32-bit data words between processors.
//*****************************************************************************
typedef struct
{
    // The command passed between processor systems.
    unsigned long ulcommand;

    // The receiving processor address the command is requesting action on.
    unsigned long uladdress;

    // A 32-bit variable, the usage of which is determined by ulcommand.
    // The most common usage is to pass length requirements
    // with the upper 16-bits storing a Response Flag for read commands.
    unsigned long uldataw1;

    // A 32-bit variable, the usage of which is determined by ulcommand.
    // For block transfers, this variable is generally the address in
    // shared memory used to pass data between processors.
    unsigned long uldataw2;
} tIpcMessage;

//*****************************************************************************
//! A structure that defines an IPC control instance.  These
//! fields are used by the IPC drivers, and normally it is not necessary for
//! user software to directly read or write fields in the table.
//*****************************************************************************
typedef struct
{
    // The address of the PutBuffer (in MSGRAM)
    tIpcMessage *psPutBuffer;

    // The IPC INT flag to set when sending messages
    // for this IPC controller instance.
    Uint32 ulPutFlag;

    // The address of thePutBuffer (inMSGRAM)
    //
    Uint16 *pusPutWriteIndex;

    // The address of the GetBuffer (in MSGRAM)
    //
    Uint16 *pusPutReadIndex;

    // The address of the GetBuffer (in MSGRAM)
    //
    tIpcMessage *psGetBuffer;

    // The address of the GetBuffer Write Index (in MSGRAM)
    Uint16 *pusGetWriteIndex;

    // The address of the GetBuffer Read Index (in MSGRAM)
    Uint16 *pusGetReadIndex;

} tIpcController;

//*****************************************************************************
// A type definition for the IPC function call command.
//*****************************************************************************
typedef unsigned long (*tfIpcFuncCall)(unsigned long ulParam);

//*****************************************************************************
// Prototypes for Circular Buffers
//*****************************************************************************
extern tIpcMessage g_asIPCCtoMBuffers[NUM_IPC_INTERRUPTS][IPC_BUFFER_SIZE];
extern tIpcMessage g_asIPCMtoCBuffers[NUM_IPC_INTERRUPTS][IPC_BUFFER_SIZE];

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
// IPC Driver Prototye Definitions
extern void IPCCInitialize (volatile tIpcController *psController,
                            Uint16 usCIpcInterrupt,
                            Uint16 usMIpcInterrupt);
extern Uint16 IpcPut (volatile tIpcController *psController,
                      tIpcMessage *psMessage,
                      Uint16 bBlock);
extern Uint16 IpcGet (volatile tIpcController *psController,
                      tIpcMessage *psMessage,
                      Uint16 bBlock);
extern Uint16 IPCCtoMDataRead (volatile tIpcController *psController,
                               Uint32 ulAddress, void *pvData, Uint16 usLength,
                               Uint16 bBlock,
                               Uint32 ulResponseFlag);
extern Uint16 IPCCtoMDataRead_Protected (volatile tIpcController *psController,
                                         Uint32 ulAddress, void *pvData,
                                         Uint16 usLength, Uint16 bBlock,
                                         Uint32 ulResponseFlag);
extern Uint16 IPCCtoMSetBits(volatile tIpcController *psController,
                             Uint32 ulAddress, Uint32 ulMask, Uint16 usLength,
                             Uint16 bBlock);
extern Uint16 IPCCtoMSetBits_Protected(volatile tIpcController *psController,
                                       Uint32 ulAddress, Uint32 ulMask,
                                       Uint16 usLength,
                                       Uint16 bBlock);
extern Uint16 IPCCtoMClearBits(volatile tIpcController *psController,
                               Uint32 ulAddress, Uint32 ulMask, Uint16 usLength,
                               Uint16 bBlock);
extern Uint16 IPCCtoMClearBits_Protected(volatile tIpcController *psController,
                                         Uint32 ulAddress, Uint32 ulMask,
                                         Uint16 usLength,
                                         Uint16 bBlock);
extern Uint16 IPCCtoMDataWrite(volatile tIpcController *psController,
                               Uint32 ulAddress, Uint32 ulData, Uint16 usLength,
                               Uint16 bBlock,
                               Uint32 ulResponseFlag);
extern Uint16 IPCCtoMDataWrite_Protected(volatile tIpcController *psController,
                                         Uint32 ulAddress, Uint32 ulData,
                                         Uint16 usLength, Uint16 bBlock,
                                         Uint32 ulResponseFlag);
extern Uint16 IPCCtoMBlockRead(volatile tIpcController *psController,
                               Uint32 ulAddress, Uint32 ulShareAddress,
                               Uint16 usLength, Uint16 bBlock,
                               Uint32 ulResponseFlag);
extern Uint16 IPCCtoMBlockWrite(volatile tIpcController *psController,
                                Uint32 ulAddress, Uint32 ulShareAddress,
                                Uint16 usLength, Uint16 usWordLength,
                                Uint16 bBlock);
extern Uint16 IPCCtoMBlockWrite_Protected(volatile tIpcController *psController,
                                          Uint32 ulAddress,
                                          Uint32 ulShareAddress,
                                          Uint16 usLength, Uint16 usWordLength,
                                          Uint16 bBlock);
extern Uint16 IPCCtoMFunctionCall(volatile tIpcController *psController,
                                  Uint32 ulAddress, Uint32 ulParam,
                                  Uint16 bBlock);
extern Uint16 IPCCtoMReqMemAccess (volatile tIpcController *psController,
                                   Uint32 ulMask, Uint16 ulMaster,
                                   Uint16 bBlock);
extern void IPCMtoCDataWrite(tIpcMessage *psMessage);
extern void IPCMtoCDataWrite_Protected(tIpcMessage *psMessage);
extern void IPCMtoCDataRead(volatile tIpcController *psController,
                            tIpcMessage *psMessage,
                            Uint16 bBlock);
extern void IPCMtoCDataRead_Protected(volatile tIpcController *psController,
                                      tIpcMessage *psMessage,
                                      Uint16 bBlock);
extern void IPCMtoCSetBits(tIpcMessage *psMessage);
extern void IPCMtoCSetBits_Protected(tIpcMessage *psMessage);
extern void IPCMtoCClearBits(tIpcMessage *psMessage);
extern void IPCMtoCClearBits_Protected(tIpcMessage *psMessage);
extern void IPCMtoCBlockRead(tIpcMessage *psMessage);
extern void IPCMtoCBlockWrite(tIpcMessage *psMessage);
extern void IPCMtoCBlockWrite_Protected(tIpcMessage *psMessage);
extern void IPCMtoCFunctionCall(tIpcMessage *psMessage);

// IPC Lite Driver Prototype Definitions
extern Uint16 IPCLiteCtoMGetResult (void *pvData, Uint16 usLength,
                                    Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMDataRead(Uint32 ulFlag, Uint32 ulAddress,
                                  Uint16 usLength,
                                  Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMSetBits(Uint32 ulFlag, Uint32 ulAddress, Uint32 ulMask,
                                 Uint16 usLength,
                                 Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMSetBits_Protected (Uint32 ulFlag, Uint32 ulAddress,
                                            Uint32 ulMask, Uint16 usLength,
                                            Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMClearBits (Uint32 ulFlag, Uint32 ulAddress,
                                    Uint32 ulMask, Uint16 usLength,
                                    Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMClearBits_Protected (Uint32 ulFlag, Uint32 ulAddress,
                                              Uint32 ulMask, Uint16 usLength,
                                              Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMDataWrite(Uint32 ulFlag, Uint32 ulAddress,
                                   Uint32 ulData, Uint16 usLength,
                                   Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMDataWrite_Protected(Uint32 ulFlag, Uint32 ulAddress,
                                             Uint32 ulData, Uint16 usLength,
                                             Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMFunctionCall(Uint32 ulFlag, Uint32 ulAddress,
                                      Uint32 ulParam,
                                      Uint32 ulStatusFlag);
extern Uint16 IPCLiteCtoMReqMemAccess (Uint32 ulFlag, Uint32 ulMask,
                                       Uint16 ulMaster,
                                       Uint32 ulStatusFlag);
extern void IPCLiteMtoCDataRead(unsigned long ulFlag,
                                unsigned long ulStatusFlag);
extern void IPCLiteMtoCSetBits(unsigned long ulFlag, unsigned long ulStatusFlag);
extern void IPCLiteMtoCSetBits_Protected (unsigned long ulFlag,
                                          unsigned long ulStatusFlag);
extern void IPCLiteMtoCClearBits(unsigned long ulFlag,
                                 unsigned long ulStatusFlag);
extern void IPCLiteMtoCClearBits_Protected (unsigned long ulFlag,
                                            unsigned long ulStatusFlag);
extern void IPCLiteMtoCDataWrite(unsigned long ulFlag,
                                 unsigned long ulStatusFlag);
extern void IPCLiteMtoCDataWrite_Protected(unsigned long ulFlag,
                                           unsigned long ulStatusFlag);
extern void IPCLiteMtoCFunctionCall(Uint32 ulFlag, Uint32 ulStatusFlag);

// IPC Utility Driver Prototype Definitions
//
extern Uint32 IPCCtoMSharedRamConvert (Uint32 ulShareAddress);
extern Uint16 IPCCtoMFlagBusy (Uint32 ulFlags);
extern void IPCCtoMFlagSet (Uint32 ulFlags);
extern void IPCCtoMFlagClear (Uint32 ulFlags);
extern void IPCMtoCFlagAcknowledge (Uint32 ulFlags);
extern Uint16 IPCMtoCFlagBusy (Uint32 ulFlags);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of F28M35x_IPC_DRIVERS_H definition


