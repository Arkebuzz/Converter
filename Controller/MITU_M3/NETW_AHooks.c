/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>


/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "GlobalData.h"

#define TCPPORT_ECHO 1003
#define TCPPORT_ECHO2 1004
#define TCPPORT_EXCHANGE 1000
#define TCPPORT_OSCI 1001

#define TCPHANDLERSTACK 800

typedef unsigned char  UINT8;
typedef void *         HANDLE;
typedef unsigned int   UINT32;
typedef unsigned int   uint;
typedef UINT32         IPN;             /* IP Address in NETWORK format */
#define CFG_DOMAIN_MAX  64
#define CFGTAG_IPNET            0x0004          /* IP Network */

/* IPNet Instance */
typedef struct _ci_ipnet {
        uint    NetType;                /* Network address type flags */
        IPN     IPAddr;                 /* IP Address */
        IPN     IPMask;                 /* Subnet Mask */
        HANDLE  hBind;                  /* Binding handle (resets to NULL) */
        char    Domain[CFG_DOMAIN_MAX]; /* IPNet Domain Name */
        } CI_IPNET;


/*
 *  ======== netOpenHook ========
 *  NDK network open hook used to initialize IPv6
 */
void netOpenHook()
{
    /*Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;

    Task_Handle taskHandle2;
	Task_Params taskParams2;
	Error_Block eb2;*/

	Task_Handle taskHandleOsci;
	Task_Params taskParamsOsci;
	Error_Block eb_Osci;

	Task_Handle taskHandleExchange;
	Task_Params taskParamsExchange;
	Error_Block eb_Exchange;

	Task_Handle taskHandle_MBUS;
	Task_Params taskParams_MBUS;
	Error_Block eb_MBUS;


    CI_IPNET NA;
	HANDLE      hCfgIpAddr;

	/* Setup manual IP address */
	bzero(&NA, sizeof(NA));
	NA.IPAddr  = inet_addr(IPAddr_cfg);
	NA.IPMask  = inet_addr(SubnetMask_cfg);
	strcpy(NA.Domain, DomainName_cfg);
	NA.NetType = 0;

	/* get the current static IP entry */
	CfgGetEntry(0, CFGTAG_IPNET, 1, 1, &hCfgIpAddr);

	/* remove the current static IP entry */
	CfgRemoveEntry(0, hCfgIpAddr);

	/* add a new static IP entry */
	CfgAddEntry(0, CFGTAG_IPNET, 1, 0, sizeof(CI_IPNET), (UINT8 *)&NA, 0);


    // Create the Task that farms out incoming TCP connections. arg0 will be the port that this task listens to
    /*Error_init(&eb); // Make sure Error_Block is initialized
    Task_Params_init(&taskParams);
    taskParams.stackSize = TCPHANDLERSTACK;
    taskParams.priority = 1;
    taskParams.arg0 = TCPPORT_ECHO;
    taskHandle = Task_create((Task_FuncPtr)tcpHandler, &taskParams, &eb);
    if (taskHandle == NULL) {
        System_printf("netOpenHook: Failed to create tcpHandler Task\n");
    }
    System_flush();*/


    //Create the task for data exchange with external controls.
	/*Task_Params_init(&taskParamsExchange);
	Error_init(&eb_Exchange);
	taskParamsExchange.stackSize = TCPHANDLERSTACK;
	taskParamsExchange.priority = 1;
	taskParamsExchange.arg0 = TCPPORT_EXCHANGE;
	taskHandleExchange = Task_create((Task_FuncPtr)DataExchangeListener, &taskParamsExchange, &eb_Exchange);
	if (taskHandleExchange == NULL) {System_printf("Failed to create Common Data Exchange thread \n");}
	else{System_printf("Started Common Data Exchange thread. Initializing Oscillogramms thread... \n");}
	System_flush();*/

	//Create the task for oscillogramms sending
	Task_Params_init(&taskParamsOsci);
	Error_init(&eb_Osci);
	taskParamsOsci.stackSize = 1024;
	taskParamsOsci.priority = 1;
	taskParamsOsci.arg0 = TCPPORT_OSCI;
	taskHandleOsci = Task_create((Task_FuncPtr)OscillogrammsListener, &taskParamsOsci, &eb_Osci);
	if (taskHandleOsci == NULL) {
		System_printf("Failed to create Oscillogramms Data Exchange thread \n");
		}
	System_printf("Started Oscillogramms Data Exchange thread. Initializing Modbus thread...\n");
	System_flush();

	//Modbus Initialisation
	Task_Params_init(&taskParams_MBUS);
	Error_init(&eb_MBUS);
	taskParams_MBUS.stackSize = 1024;
	taskParams_MBUS.priority = 1;
	taskHandle_MBUS = Task_create((Task_FuncPtr)ModbusThread, &taskParams_MBUS, &eb_MBUS);
	if (taskHandle_MBUS == NULL)
	{System_printf("Can't start protocol stack!\n"); System_flush();}
	System_printf("Started Modbus thread \n");
	System_flush();


   /*Task_Params_init(&taskParams2);
     Error_init(&eb2);
     taskParams2.stackSize = TCPHANDLERSTACK;
     taskParams2.priority = 1;
     taskParams2.arg0 = TCPPORT_ECHO2;
     taskHandle2 = Task_create((Task_FuncPtr)tcpHandler, &taskParams2, &eb2);
     if (taskHandle2 == NULL) {
       System_printf("netOpenHook: Failed to create tcpHandler 2 Task\n");
       System_printf("Err ID: %d\n",eb2.id);
       }
    System_flush();*/

}
