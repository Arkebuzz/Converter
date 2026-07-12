/*
 * GlobalData.h
 *
 *  Created on: 7 дек. 2016 г.
 *      Author: Volkodav
 */

#ifndef GLOBALDATA_H_
#define GLOBALDATA_H_

#include <string.h>

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Diags.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/heaps/HeapBuf.h>

#include <ti/sysbios/knl/Semaphore.h>


/* NDK BSD support */
#include <sys/socket.h>

/* Example/Board Header file */
#include "Board.h"

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

//Driverlib header files
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ipc.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ram.h"
#include "driverlib/debug.h"
#include "driverlib/flash.h"
#include "driverlib/ipc.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include <driverlib/epi.h>
#include "driverlib/ipc.h"
#include "driverlib/ram.h"
#include "driverlib/sysctl.h"

#include "Memfunctions.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
/* ----------------------- Modbus Defines ------------------------------------------*/

#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4
#define REG_HOLDING_START 1
#define REG_HOLDING_NREGS 255



#define CTOM_DATA_SIZE 100
#define MTOC_DATA_SIZE 100
#define EXT_DATA_SIZE 100

#define SETUP_DATA_SIZE 600
#define SETUP_DATA_START_ADDR 200
#define SETUP_UINT_DATA_COUNT 50
#define SETUP_FLOAT_DATA_COUNT 150

#define OSCI_SETUP_DATA_START_ADDR 1000

#define NUMTCPWORKERS 1
#define TCPPACKETSIZE 256

#define MODBUS_TCP_PORT 502         // for multi-client task
#define MODBUS_MAX_PDU  253         // spec: 253 bytes PDU
#define MODBUS_MAX_ADU  (7 + MODBUS_MAX_PDU)
#define MAX_CONN_WORKERS  8        // cap concurrent clients

#define MODBUS_FC_READ_HOLDING_REGS 0x03
#define MODBUS_FC_WRITE_SINGLE_REG  0x06


#define TOTAL_CHANNELS 30
#define HEADER_SIZE 12
#define PACK_SIZE 3
#define DATA_PACKS_TOTAL    33
#define SHMEM_BUFFER_SIZE    1024
#define SHMEM_BUFFERS_COUNT    7

#define MEM_SIZE                0x00008000 //32KB - 16K Words

#define TCPHANDLERSTACK 800

/* ----------------------- MBUS functions ---------------------------------*/
extern enum ThreadState eGetPollingThreadState( void );
extern void     eSetPollingThreadState( enum ThreadState eNewState );
extern void* pvPollingThread( void *pvParameter );

/* Prototypes */
extern Void tcpHandler(UArg arg0, UArg arg1);
extern Void tcpHandler1(UArg arg0, UArg arg1);
extern Void DataExchangeListener(UArg arg0, UArg arg1);
extern Void OscillogrammsListener(UArg arg0, UArg arg1);
extern Void ModbusThread(UArg arg0, UArg arg1);
extern Void ModbusThread2(UArg arg0, UArg arg1);

extern Void ModbusServerTask(UArg arg0, UArg arg1);

extern void Buffers_Init(void);
extern Void KeepAliveTask(UArg arg0, UArg arg1);

static enum ThreadState
{
    STOPPED,
    RUNNING,
    SHUTDOWN
} ePollThreadState, ePollThreadState2;

extern char * IPAddr_cfg;
extern char * SubnetMask_cfg;
extern char * DomainName_cfg;

extern short ModbusControlledInput;

extern short FirstThreadRun;
extern short SecondThreadRun;

extern short* osciBuf;
extern int osciBufSize;
extern int osciBufCnt;
extern int lastSent;
extern int lastWritten;
extern char bOverload;

extern unsigned int PWMAmpData;

extern short* CTOM_Data;
extern short* ExtControl_Data;
extern short* MTOC_Data;

extern unsigned short IO_Count;
extern unsigned short IO_Success_Count;

extern unsigned short MTOC_IO_Count;
extern unsigned short CTOM_IO_Count;

extern USHORT   usRegInputStart;
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];
extern USHORT   usRegHoldingStart;
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];


// Protect register writes across worker tasks
extern Semaphore_Handle g_regLock;
extern Semaphore_Handle g_connSlots;


#endif /* GLOBALDATA_H_ */
