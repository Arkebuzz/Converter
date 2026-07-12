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

#include "communication.h"


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

    // IVAN: likely needed for network interface to set up properly
    for (Uint32 i=0; i < 10000000; i++) {} //DALER: Wait for board powerup

    // IVAN: debug prints
    // System_printf formats a string and puts it into a memory buffer to be sent to
    // PC via the JTAG debug probe (TI's System Analyzer)
    System_printf("Init complete. Performing FPGA Test...\n");
    System_flush();

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

    System_printf("C28 core init complete. Creating tasks...\n");
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
	taskHandleData = Task_create((Task_FuncPtr)KeepAliveTask, &taskParamsData, &eb_Data);
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
}
