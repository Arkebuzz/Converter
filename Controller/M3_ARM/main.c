
// COPY PASTE FORM GlobalData.h

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
//extern Void DataProcessor(UArg arg0, UArg arg1);

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

// COPY END

// COPY PASTE FROM GlobalDataInit.c
// IVAN: originally called Buffers_Init
void setup_buffers(void) {
	/* Make sure Error_Block is initialized */
	Error_Block eb;
	Error_init(&eb);

	CTOM_Data = Memory_alloc(NULL, CTOM_DATA_SIZE*sizeof(short), 0, &eb);
	    if (CTOM_Data == NULL) {
	        System_printf("failed to alloc memory!! \n");
	    }
	memset(CTOM_Data,0,CTOM_DATA_SIZE*sizeof(short));

	MTOC_Data = Memory_alloc(NULL, MTOC_DATA_SIZE*sizeof(short), 0, &eb);
		    if (MTOC_Data == NULL) {
		        System_printf("failed to alloc memory!! \n");
		    }
	memset(MTOC_Data,0,MTOC_DATA_SIZE*sizeof(short));

	ExtControl_Data = Memory_alloc(NULL, EXT_DATA_SIZE*sizeof(short), 0, &eb);
		if (ExtControl_Data == NULL) {
		 System_printf("failed to alloc memory!! \n");
		}
	memset(ExtControl_Data,0,EXT_DATA_SIZE*sizeof(short));
}

// COPY END

// IVAN: moves data between C28 and M3, toggles the watchdog chip
Void DataProcessor(UArg arg0, UArg arg1) {
	// IVAN: toggles on some error pin likely to indicate that the task has started
	GPIO_write(TMDXDOCKH52C1_ERR, GPIO_TURN_ON);

	// IVAN: the task runs forever
	Uint16 i = 0; // IVAN: for watchdog pin
	Uint16 j = 0; // IVAN: for LED
	while (true) {
		i++;
		j++;

		// IVAN: blink the LED
		if (j == 40) {
			GPIO_write(TMDXDOCKH52C1_LED, GPIO_TURN_ON);
		} else if (j > 80) {
			j = 0;
			GPIO_write(TMDXDOCKH52C1_LED, GPIO_TURN_OFF);
		}


		// IVAN: pulses the watchdog hardware
		// watchdog is a safety chip that requires being pulsed to show
		// that CPU is still alive (or else it will reset the CPU)
		if (i % 2 == 0) {
			GPIO_write(TMDXDOCKH52C1_WD, GPIO_TURN_OFF);
		} else {
			GPIO_write(TMDXDOCKH52C1_WD, GPIO_TURN_ON);
		}

		// IVAN: copy data from C28 to M3
		for (Uint16 k = 0; k < CTOM_DATA_SIZE; k++) {
			CTOM_Data[k] = ReadFrom_CTOM_MSGRAM(k);
		}
		CTOM_IO_Count++;

		// IVAN: copy data from M3 to C28
		for (Uint16 k = 0; k < MTOC_DATA_SIZE; k++)	{
			WriteTo_MTOC_MSGRAM(k,MTOC_Data[k]);
		}
		MTOC_IO_Count++;

		// IVAN: sleep 25 system clock ticks
		Task_sleep(25);
	}
}

int main(void) {
	// IVAN: warming up the cpu a little
	for (Uint32 i=0; i < 1000000; i++) {}

	// IVAN: the registers that control the system clocks are locked by default =>
	// disable the protection
	HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

	// DALER: Sets up PLL, M3 running at 100MHz and C28 running at 100MHz
	// IVAN: часики с кристала медленные и наш проц умеет их ускорять
	SysCtlClockConfigSet(
		SYSCTL_USE_PLL | 				// use faster clock via multiplying the signal
		(SYSCTL_SPLLIMULT_M & 0xA) |	// set multiplier to 0xA = 10
		SYSCTL_SYSDIV_1 |				// set divider to 1
		SYSCTL_M3SSDIV_1 |				// set divider to 1
		SYSCTL_XCLKDIV_4				// configures clock divisor for C28
	);

	// IVAN: give master access of shared mem 6-7 to C28
	RAMMReqSharedMemAccess((S6_ACCESS | S7_ACCESS), SX_C28MASTER);

    /* DALER: Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initEMAC();
    Board_initEPI();
    setup_buffers();

    // IVAN: likely needed for network interface to set up properly
    for (Uint32 i=0; i < 10000000; i++) {} //DALER: Wait for board powerup

    // IVAN: debug prints
    // System_printf formats a string and puts it into a memory buffer to be sent to
    // PC via the JTAG debug probe (TI's System Analyzer)
    System_printf("Init complete. Performing FPGA Test...\n");
    System_flush();

	// IVAN: diagnostic tests
	// LIKELY NOT NEEDED
	/*
	UInt16 DataSRAM;
	DataSRAM = 100; // IVAN: try to send random data to FPGA
	for (Uint32 i = 1000; i < 0x2000; i++) {
		WriteTo_FPGA((unsigned short)i,DataSRAM);
		DataSRAM++;
	}
	DataSRAM = 100;
	for (Uint32 i = 1000; i < 0x2000; i++) {
		if (DataSRAM != (UInt16)ReadFrom_FPGA((unsigned short)i))
		{
		System_printf("Error testing FPGA! \n");
		System_printf("Data at adress %d ", i);
		System_printf(" = %d \n", (UInt16)ReadFrom_FPGA((unsigned short)i));
		System_printf("Stopping operation. \n");
		System_flush();
		goto blocking_loop;
		}
		WriteTo_SRAM(i,0);
		DataSRAM++;
	}
	System_printf("FPGA Test complete.\n");
	System_flush();
	*/

    System_printf("Initializing C28 core.\n");
    System_flush();
    // IVAN: boot C28 from FLASH
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);

    // DALER: Waiting for C28 core init. C28 must set IPC_CTOMIPCSTS_IPC1 after loading init data to SHMEM
    // IVAN: this loop breaks as soon as C28 sets either IPC_CTOMIPCSTS_IPC1 or IPC_CTOMIPCSTS_IPC2
    while (
    		(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC1) != IPC_CTOMIPCSTS_IPC1 &&
    		(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC2) != IPC_CTOMIPCSTS_IPC2
	) {}

    // DALER: Init M3 setup data
    // DALER: Acknowledge init data copy. Proceed to thread init
    // IVAN: C28 waits in infinite while (in it's main function)
    // until we set CtoMIpcRegs.CTOMIPCFLG.bit.IPC1 to 0 via ACK
    // LOOK IN README.TXT (9)
    HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= IPC_CTOMIPCSTS_IPC1;

    // IVAN: hardcoding net addresses
    // TODO: move where it belongs
    IPAddr_cfg = "10.3.5.38";
	SubnetMask_cfg = "255.255.255.0";
	DomainName_cfg = "PMCB";

    System_printf("C28 core init complete. Creating tasks...\n");
    //System_printf("Sizeof Int= %d \n", sizeof(int));
    //System_printf("Sizeof Short= %d \n", sizeof(short));
    //System_printf("Sizeof long int= %d \n", sizeof(long int));
    System_flush();


    // DALER: Create the task for data exchange between M3 and C28 cores.
	Task_Params taskParamsData;
	Task_Params_init(&taskParamsData);

	taskParamsData.stackSize = 512;
	taskParamsData.priority = 1;

	Error_Block eb_Data;
	Error_init(&eb_Data);

	// IVAN: create background task DataProcessor
	Task_Handle taskHandleData;
	taskHandleData = Task_create((Task_FuncPtr)DataProcessor, &taskParamsData, &eb_Data);
	if (taskHandleData == NULL) {
		System_printf("Failed to create Data Processor thread \n");
	} else {
		System_printf("Started Data Processor thread. Initializing Data Exchange thread... \n");
	}
	System_flush();

    // DALER: Start BIOS
	// IVAN: start the TIRTOS - it will take care of our DataProcessor task and tcpHandler
    BIOS_start();

    // IVAN: unreachable since BIOS_start never returns
    return 0;

//    blocking_loop:
//	while (true) {}
}
