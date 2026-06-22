#include "GlobalData.h"

/*
 *  ======== main ========
 */
int main(void)
{
	long unsigned int i;
	UInt16 DataSRAM;

	for (i=0;i<1000000;i++);
	{i=i+1;}


	Task_Handle taskHandleData;
	Task_Params taskParamsData;
	Error_Block eb_Data;

	// Disable Protection
	HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

	// Sets up PLL, M3 running at 100MHz and C28 running at 100MHz
	SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xA) |
							 SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_1 |
							 SYSCTL_XCLKDIV_4);

	RAMMReqSharedMemAccess((S6_ACCESS | S7_ACCESS),SX_C28MASTER);

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initEMAC();
    Board_initEPI();
    Buffers_Init();

    for (i=0;i<10000000;i++)  	{i=i+1;} //Wait for board powerup

    System_printf("Init complete. Performing FPGA Test...\n");
    System_flush();

    DataSRAM = 100;
	for (i=1000;i<0x2000;i++)
	{
		WriteTo_FPGA((unsigned short)i,DataSRAM);
		DataSRAM++;
	}
	DataSRAM = 100;
	for (i=1000;i<0x2000;i++)
	{
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


    System_printf(" Initializing C28 core.\n");
    System_flush();
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);

    //Waiting for C28 core init. C28 must set IPC_CTOMIPCSTS_IPC1 after loading init data to SHMEM
    while ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC1) != IPC_CTOMIPCSTS_IPC1 &&
    		 (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC2) != IPC_CTOMIPCSTS_IPC2 ) {i++;}

    //Init M3 setup data

    HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCSTS_IPC1; //Acknowledge init data copy. Proceed to thread init

    IPAddr_cfg = "10.3.5.38";
	SubnetMask_cfg = "255.255.255.0";
	DomainName_cfg = "PMCB";

    System_printf("C28 core init complete. Creating tasks...\n");
    //System_printf("Sizeof Int= %d \n", sizeof(int));
    //System_printf("Sizeof Short= %d \n", sizeof(short));
    //System_printf("Sizeof long int= %d \n", sizeof(long int));
    System_flush();

    //Create the task for data exchange between M3 and C28 cores.
	Task_Params_init(&taskParamsData);
	Error_init(&eb_Data);
	taskParamsData.stackSize = 512;
	taskParamsData.priority = 1;
	taskHandleData = Task_create((Task_FuncPtr)DataProcessor, &taskParamsData, &eb_Data);
	if (taskHandleData == NULL) {System_printf("Failed to create Data Processor thread \n");}
	else{System_printf("Started Data Processor thread. Initializing Data Exchange thread... \n");}
	System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);

    blocking_loop:
	while(true)
	{i=i+1;}
}





