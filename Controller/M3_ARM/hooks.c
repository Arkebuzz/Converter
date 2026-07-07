
// ИВАН: основной хедер для работы с сетью ndk
#include <ti/ndk/inc/netmain.h>

char *IPAddr_cfg = "10.1.3.12";
char *SubnetMask_cfg = "255.0.0.0";
char *DomainName_cfg = "PMCB";

// Это присваивается в main.c
// И какой из них оставить??
// IPAddr_cfg = "10.3.5.38";
// SubnetMask_cfg = "255.255.255.0";
// DomainName_cfg = "PMCB";

// ИВАН: этот хук вызовется как только IP и Ethernet уже засетапились
// => можно создавать TCP
void netOpenHook() {
	// ИВАН: айпи адрес, маска и т.д. можно настроить в .cfg файлике
	// через интерфейс, но мы делаем это в коде
	// наверное на случай если прямо во время работы когда-либо понадобится поменять
	// эти настройки
	// Конфиг это дерево из ключей, достаем оттуда нужные ключи,
	// удаляем их, заменяем своими

	HANDLE hCfgIpAddr;
	if (CfgGetEntry(
			NULL,               // hCfg: NULL - глобальный конфиг
			CFGTAG_IPNET,       // Tag: IP Network
			1,                  // Item: 1й физический интерфейс (EMAC порт)
			1,                  // Index: 1й IP адрес назначеный на этот интерфейс
								// (к одному интерфейсу может быть прикреплено несколько IP)
			&hCfgIpAddr         // Output: Засовывает найденое значение в hCfgIpAddr
	) == 1) {
		CfgRemoveEntry(
		    NULL,               // hCfg: NULL - глобальный конфиг
		    hCfgIpAddr          // hCfgEntry: только что найденый ключ
		);
	}

	CI_IPNET IPNet_cfg_instance = {
		.NetType = 0,
		.IPAddr = inet_addr(IPAddr_cfg),
		.IPMask = inet_addr(SubnetMask_cfg),
		.hBind = 0,
		.Domain = {0},
	};
	strncpy(IPNet_cfg_instance.Domain, DomainName_cfg, CFG_DOMAIN_MAX - 1);

	CfgAddEntry(
	    NULL,               			// hCfg: NULL - глобальный конфиг
	    CFGTAG_IPNET,       			// Tag: IP Network
	    1,                  			// Item: 1й физический интерфейс (EMAC порт)
	    0,                  			// Mode: 0 - "Default behavior" (Применить найстройку прямо сейчас)
	    sizeof(IPNet_cfg_instance),     // Size: Размер засовываемой структуры
	    (UINT8 *)&IPNet_cfg_instance,   // pData: Засовываемая структура
	    NULL               			    // Output: NULL - нам не нужен хендл обратно
	);
}


// COPY PASTE

//#include <xdc/runtime/Error.h>
//#include <xdc/runtime/System.h>
//
//
///* BIOS Header files */
//#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/knl/Semaphore.h>
//
//#define TCPPORT_ECHO 1003
//#define TCPPORT_ECHO2 1004
//#define TCPPORT_EXCHANGE 1000
//
////#define TCPPORT_OSCI 1001
//#define TCPPORT_OSCI 1124
//
//#define TCPHANDLERSTACK 800
//
//typedef unsigned char  UINT8;
//typedef void *         HANDLE;
//typedef unsigned int   UINT32;
//typedef unsigned int   uint;
//typedef UINT32         IPN;             /* IP Address in NETWORK format */
//#define CFG_DOMAIN_MAX  64
//#define CFGTAG_IPNET            0x0004          /* IP Network */
//
///* IPNet Instance */
//typedef struct _ci_ipnet {
//        uint    NetType;                /* Network address type flags */
//        IPN     IPAddr;                 /* IP Address */
//        IPN     IPMask;                 /* Subnet Mask */
//        HANDLE  hBind;                  /* Binding handle (resets to NULL) */
//        char    Domain[CFG_DOMAIN_MAX]; /* IPNet Domain Name */
//        } CI_IPNET;
//
//
///* ----------------------- MBUS variables ---------------------------------*/
//Semaphore_Handle g_regLock;
//Semaphore_Handle g_connSlots;
//
//
//Void OscillogrammsListener(UArg arg0, UArg arg1)
//{
//    int lSocket;
//    struct sockaddr_in sLocalAddr;
//    int clientfd;
//    struct sockaddr_in client_addr;
//    int addrlen=sizeof(client_addr);
//    int optval;
//    int optlen = sizeof(optval);
//    int status;
//    Task_Handle taskHandle;
//    Task_Params taskParams;
//    Error_Block eb;
//
//    lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (lSocket < 0) {
//        System_printf("tcpHandler: socket failed\n");
//        goto shutdown;
//    }
//
//    memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
//    sLocalAddr.sin_family = AF_INET;
//    sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    sLocalAddr.sin_port = htons(arg0);
//
//    status = bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr));
//    if (status < 0) {
//        System_printf("tcpHandler: bind failed\n");
//        goto shutdown;
//    }
//
//    status = listen(lSocket, NUMTCPWORKERS);
//	if (status == -1) {
//		System_printf("Error: listen failed.\n");
//		goto shutdown;
//	}
//
//	optval = 1;
//    if (setsockopt(lSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
//        System_printf("tcpHandler: setsockopt failed\n");
//        goto shutdown;
//    }
//
//    optval = 1;
//    if (setsockopt(lSocket, IPPROTO_TCP, TCP_NODELAY, &optval, optlen) < 0) {
//		System_printf("tcpHandler: setsockopt TCP_NODELAY failed\n");
//		goto shutdown;
//	}
//
//
//    /* Wait for incoming request */
//    while ((clientfd =
//       		accept(lSocket, (struct sockaddr*)&client_addr, &addrlen)) != -1) {
//        System_printf("tcpHandler: Creating thread clientfd = %d\n", clientfd);
//
//        /* Init the Error_Block */
//        Error_init(&eb);
//
//        /* Initialize the defaults and set the parameters. */
//        Task_Params_init(&taskParams);
//        taskParams.arg0 = (UArg)clientfd;
//        taskParams.stackSize = 1024;
//        taskHandle = Task_create((Task_FuncPtr)OscillogrammsWorker, &taskParams, &eb);
//        if (taskHandle == NULL) {
//            System_printf("tcpHandler: Failed to create new Task\n");
//            close(clientfd);
//		}
//
//		/* addrlen is a value-result param, must reset for next accept call */
//		addrlen = sizeof(client_addr);
//    }
//
//    shutdown:
//	if (lSocket > 0) {
//		close(lSocket);
//	}
//}
//
//#define MAX_CONN_WORKERS  8        // cap concurrent client
//void netOpenHook()
//{
//    /*Task_Handle taskHandle;
//    Task_Params taskParams;
//    Error_Block eb;
//
//    Task_Handle taskHandle2;
//	Task_Params taskParams2;
//	Error_Block eb2;*/
//
//	Task_Handle taskHandleOsci;
//	Task_Params taskParamsOsci;
//	Error_Block eb_Osci;
//
//	Task_Handle taskHandleExchange;
//	Task_Params taskParamsExchange;
//	Error_Block eb_Exchange;
//
//	Task_Handle taskHandle_MBUS;
//	Task_Params taskParams_MBUS;
//	Error_Block eb_MBUS;
//
//    Task_Handle taskHandle_MBUS_v2;
//    Task_Params taskParams_MBUS_v2;
//    Error_Block eb_MBUS_v2;
//
//
//	Task_Handle taskHandle_MBUS2;
//	Task_Params taskParams_MBUS2;
//	Error_Block eb_MBUS2;
//
//
//    CI_IPNET NA;
//	HANDLE      hCfgIpAddr;
//
//	/* Setup manual IP address */
//	bzero(&NA, sizeof(NA));
//	NA.IPAddr  = inet_addr(IPAddr_cfg);
//	NA.IPMask  = inet_addr(SubnetMask_cfg);
//	strcpy(NA.Domain, DomainName_cfg);
//	NA.NetType = 0;
//
//	/* get the current static IP entry */
//	CfgGetEntry(0, CFGTAG_IPNET, 1, 1, &hCfgIpAddr);
//
//	/* remove the current static IP entry */
//	CfgRemoveEntry(0, hCfgIpAddr);
//
//	/* add a new static IP entry */
//	CfgAddEntry(0, CFGTAG_IPNET, 1, 0, sizeof(CI_IPNET), (UINT8 *)&NA, 0);
//
//
//    // Create the Task that farms out incoming TCP connections. arg0 will be the port that this task listens to
//    /*Error_init(&eb); // Make sure Error_Block is initialized
//    Task_Params_init(&taskParams);
//    taskParams.stackSize = TCPHANDLERSTACK;
//    taskParams.priority = 1;
//    taskParams.arg0 = TCPPORT_ECHO;
//    taskHandle = Task_create((Task_FuncPtr)tcpHandler, &taskParams, &eb);
//    if (taskHandle == NULL) {
//        System_printf("netOpenHook: Failed to create tcpHandler Task\n");
//    }
//    System_flush();*/
//
//
//    //Create the task for data exchange with external controls.
//	/*Task_Params_init(&taskParamsExchange);
//	Error_init(&eb_Exchange);
//	taskParamsExchange.stackSize = TCPHANDLERSTACK;
//	taskParamsExchange.priority = 1;
//	taskParamsExchange.arg0 = TCPPORT_EXCHANGE;
//	taskHandleExchange = Task_create((Task_FuncPtr)DataExchangeListener, &taskParamsExchange, &eb_Exchange);
//	if (taskHandleExchange == NULL) {System_printf("Failed to create Common Data Exchange thread \n");}
//	else{System_printf("Started Common Data Exchange thread. Initializing Oscillogramms thread... \n");}
//	System_flush();*/
//
//	//Create the task for oscillogramms sending
//	Task_Params_init(&taskParamsOsci);
//	Error_init(&eb_Osci);
//	taskParamsOsci.stackSize = 1024;
//	taskParamsOsci.priority = 1;
//	taskParamsOsci.arg0 = TCPPORT_OSCI;
//	taskHandleOsci = Task_create((Task_FuncPtr)OscillogrammsListener, &taskParamsOsci, &eb_Osci);
//	if (taskHandleOsci == NULL) {
//		System_printf("Failed to create Oscillogramms Data Exchange thread \n");
//		}
//	System_printf("Started Oscillogramms Data Exchange thread. Initializing Modbus thread...\n");
//	System_flush();
//
//	////////////////////////////////////
//	// Modbus Initialization
//
//	// CLASSIC driver (on 502 port)
//    Task_Params_init(&taskParams_MBUS);
//    Error_init(&eb_MBUS);
//    taskParams_MBUS.stackSize = 1024;
//    taskParams_MBUS.priority = 1;
//    taskHandle_MBUS = Task_create((Task_FuncPtr)ModbusThread, &taskParams_MBUS, &eb_MBUS);
//    if (taskHandle_MBUS == NULL)
//    {System_printf("Can't start protocol stack!\n"); System_flush();}
//    System_printf("Started Modbus thread \n");
//    System_flush();
//
//
//
//    // NEW multi-client driver (on 504 port)
//	// Create binary semaphore (mutex) for register map
//    Semaphore_Params sp1;
//    Semaphore_Params_init(&sp1);
//    g_regLock = Semaphore_create(1, &sp1, NULL);
//
//    // Pre-create a counting semaphore to limit concurrent workers
//    Semaphore_Params sp2;
//    Semaphore_Params_init(&sp2);
//    g_connSlots = Semaphore_create(MAX_CONN_WORKERS, &sp2, NULL);
//
//
///*
//	Task_Params_init(&taskParams_MBUS_v2);
//    Error_init(&eb_MBUS_v2);
//    taskParams_MBUS_v2.stackSize = 1024;
//    taskParams_MBUS_v2.priority = 2;
//    taskHandle_MBUS = Task_create((Task_FuncPtr)ModbusServerTask, &taskParams_MBUS_v2, &eb_MBUS_v2);
//    if (taskHandle_MBUS == NULL)
//    {System_printf("Can't start protocol stack!\n"); System_flush();}
//    System_printf("Started Modbus thread \n");
//    System_flush();
//*/
//
//
//
//	//Task_Params_init(&taskParams_MBUS2);
//	//Error_init(&eb_MBUS2);
//    //taskParams_MBUS2.stackSize = 1024;
//    //taskParams_MBUS2.priority = 1;
//    //taskHandle_MBUS2 = Task_create((Task_FuncPtr)ModbusThread2, &taskParams_MBUS2, &eb_MBUS2);
//    //if (taskHandle_MBUS2 == NULL)
//    //{System_printf("Can't start protocol stack #2!\n"); System_flush();}
//    //System_printf("Started Modbus thread #2 \n");
//    //System_flush();
//
//   /*Task_Params_init(&taskParams2);
//     Error_init(&eb2);
//     taskParams2.stackSize = TCPHANDLERSTACK;
//     taskParams2.priority = 1;
//     taskParams2.arg0 = TCPPORT_ECHO2;
//     taskHandle2 = Task_create((Task_FuncPtr)tcpHandler, &taskParams2, &eb2);
//     if (taskHandle2 == NULL) {
//       System_printf("netOpenHook: Failed to create tcpHandler 2 Task\n");
//       System_printf("Err ID: %d\n",eb2.id);
//       }
//    System_flush();*/
//
//}
