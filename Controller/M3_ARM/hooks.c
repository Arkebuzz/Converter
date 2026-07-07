
#include <ti/ndk/inc/netmain.h>
//#include <sys/socket.h>

#include <ti/sysbios/knl/Task.h>

#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

// по идее может быть любым от 1024 до 65535
#define TCPPORT_OSCI 1124

#define NUM_TCP_WORKERS 1

// Эти были обьявлены глобально гдето в файлах
char *IPAddr_cfg = "10.1.3.12"; // по идее этот айпи зависит от того какой у компа айпи в локальной сети
char *SubnetMask_cfg = "255.0.0.0";
char *DomainName_cfg = "PMCB"; // сюда по идее чо угодно можно писать

// Это присваивается в main.c
// И какой из них оставить??
// IPAddr_cfg = "10.3.5.38";
// SubnetMask_cfg = "255.255.255.0";
// DomainName_cfg = "PMCB";

Void OscillogrammsWorker(UArg arg0, UArg arg1) {}

// TCP сервер на BSD сокетах
// Сидим ждем пока ПК законнектится
// когда коннект случится спавним таск который его обработает
Void OscillogrammsTask(UArg arg0, UArg arg1) {
	SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd == INVALID_SOCKET) {
		System_printf("OscillogrammsTask: socket failed\n");
		return;
	}

    int status;
    struct sockaddr_in server_addr = {0};
    Uint16 port = arg0;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

	status = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (status < 0) {
		System_printf("OscillogrammsTask: bind failed\n");
		fdClose(server_fd);
		return;
	}

	status = listen(server_fd, NUM_TCP_WORKERS);
	if (status < 0) {
		System_printf("OscillogrammsTask: listen failed\n");
		fdClose(server_fd);
		return;
	}

	// IVAN: ставим SO_KEEPALIVE чтобы если вдруг выдернули кабель коннект сам закрылся
	// не дожидаясь FIN и не посылая данные в пустоту
    int opt;
	opt = 1;
	status = setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
	if (status < 0) {
		System_printf("OscillogrammsTask: setsockopt SO_KEEPALIVE failed\n");
		fdClose(server_fd);
		return;
	}

	// IVAN: ставим TCP_NODELAY чтобы отключить фичу TCP когда перед отправкой
	// данных ждет не захотим ли мы еще что-нибудь отправить
	// таким образом когда вызовем send() данные отправятся моментально, без ожиданий
	opt = 1;
	status = setsockopt(server_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	if (status < 0) {
		System_printf("OscillogrammsTask: setsockopt TCP_NODELAY failed\n");
		fdClose(server_fd);
		return;
	}

	// IVAN: accept TCP requests
	while (1) {
	    struct sockaddr_in client_addr = {0};
	    int client_addr_len = sizeof(client_addr);
		SOCKET client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
		if (client_fd == INVALID_SOCKET) {
			System_printf("OscillogrammsTask: accept failed\n");
			fdClose(server_fd);
			return;
		}

		System_printf("OscillogrammsTask: Creating thread to handle client_fd = %d\n", client_fd);

		// IVAN: create new Task to handle new connection
		Error_Block eb;
		Error_init(&eb);

		Task_Params taskParams;
		Task_Params_init(&taskParams);
		taskParams.arg0 = (UArg)client_fd;
		// IVAN: In TI docks it is recommended to use > 4096 stack for a TCP task
		taskParams.stackSize = 4096;
//		taskParams.stackSize = 1024;

		Task_Handle taskHandle = Task_create(
			(Task_FuncPtr)OscillogrammsWorker,
			&taskParams,
			&eb
		);
		if (taskHandle == NULL) {
			System_printf("OscillogrammsTask: failed to create new task for OscillogramsWorker\n");
			fdClose(client_fd);
		}
	}
}

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
	int entry_found = CfgGetEntry(
			NULL,               // hCfg: NULL - глобальный конфиг
			CFGTAG_IPNET,       // Tag: IP Network
			1,                  // Item: 1й физический интерфейс (EMAC порт)
			1,                  // Index: 1й IP адрес назначеный на этот интерфейс
								// (к одному интерфейсу может быть прикреплено несколько IP)
			&hCfgIpAddr         // Output: Засовывает найденое значение в hCfgIpAddr
	);
	if (entry_found == 1) {
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

	// DALER: Create the task for oscillogramms sending
	Error_Block eb_Osci;
	Error_init(&eb_Osci);

	Task_Params taskParamsOsci;
	Task_Params_init(&taskParamsOsci);
	taskParamsOsci.stackSize = 1024;
	taskParamsOsci.priority = 1;
	// IVAN: arg0 - первый аргумент которфй будет передан в OscillogrammsListener
	taskParamsOsci.arg0 = TCPPORT_OSCI;

	Task_Handle taskHandleOsci = Task_create(
		(Task_FuncPtr)OscillogrammsTask,
		&taskParamsOsci,
		&eb_Osci
	);
	if (taskHandleOsci == NULL) {
		System_printf("netOpenHook: failed to start OscillogrammsTask task\n");
	} else {
		System_printf("netOpenHook: started OscillogrammsTask task\n");
	}
	System_flush();
}
