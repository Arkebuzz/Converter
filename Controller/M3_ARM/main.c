#include <string.h> // for memset

#include "Board.h"

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "inc/hw_ipc.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"

#include "driverlib/ram.h"
#include "driverlib/ipc.h"
#include "driverlib/sysctl.h"

extern char * IPAddr_cfg;
extern char * SubnetMask_cfg;
extern char * DomainName_cfg;

// IVAN: TODO: what is ext data and ext control
#define EXT_DATA_SIZE 100
short* ExtControl_Data;

#include <xdc/runtime/Memory.h>

// IVAN: originally called Buffers_Init
void setup_buffers(void) {
	/* Make sure Error_Block is initialized */
	Error_Block eb;
	Error_init(&eb);

	// IVAN: CTOM_Data and MTOC_Data are statically allocated now
//	CTOM_Data = Memory_alloc(NULL, CTOM_DATA_SIZE*sizeof(short), 0, &eb);
//	    if (CTOM_Data == NULL) {
//	        System_printf("failed to alloc memory!! \n");
//	    }
//	memset(CTOM_Data,0,CTOM_DATA_SIZE*sizeof(short));
//
//	MTOC_Data = Memory_alloc(NULL, MTOC_DATA_SIZE*sizeof(short), 0, &eb);
//		    if (MTOC_Data == NULL) {
//		        System_printf("failed to alloc memory!! \n");
//		    }
//	memset(MTOC_Data,0,MTOC_DATA_SIZE*sizeof(short));

	ExtControl_Data = Memory_alloc(NULL, EXT_DATA_SIZE*sizeof(short), 0, &eb);
		if (ExtControl_Data == NULL) {
		 System_printf("failed to alloc memory!! \n");
		}
	memset(ExtControl_Data,0,EXT_DATA_SIZE*sizeof(short));
}

#pragma DATA_SECTION(CTOM_MSGRAM, "CTOM_MSGRAM")
volatile Uint8 CTOM_MSGRAM[0x800];

#pragma DATA_SECTION(MTOC_MSGRAM, "MTOC_MSGRAM")
volatile Uint8 MTOC_MSGRAM[0x800];

// IVAN: data size IN 16 BIT WORDS
Uint16 CTOM_Data[100];
Uint16 MTOC_Data[100];

// IVAN: ЗНАЧЕНИЕ С НИХ НИГДЕ В КОДЕ ДАЛЕРА НЕ СЧИТЫВАЕТСЯ
unsigned short MTOC_IO_Count = 0;
unsigned short CTOM_IO_Count = 0;

// IVAN: moves data between C28 and M3, toggles the watchdog chip
Void DataProcessor(UArg arg0, UArg arg1) {
	// IVAN: toggles on some error pin likely to indicate that the task has started
	GPIO_write(TMDXDOCKH52C1_ERR, GPIO_TURN_ON);

	// IVAN: the task runs forever
	Uint16 i = 0; // IVAN: for watchdog pin
	Uint16 j = 0; // IVAN: for LED
	while (1) {
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
		for (Uint16 k = 0; k < sizeof(CTOM_Data) / sizeof(CTOM_Data[0]); k++) {
			CTOM_Data[k] = ((volatile Uint16 *)CTOM_MSGRAM)[k];
		}
		CTOM_IO_Count++;

		// IVAN: copy data from M3 to C28
		for (Uint16 k = 0; k < sizeof(MTOC_Data) / sizeof(MTOC_Data[0]); k++) {
			((volatile Uint16 *)MTOC_MSGRAM)[k] = MTOC_Data[k];
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
