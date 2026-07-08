
#include "Board.h"

#include <ti/ndk/inc/netmain.h>

#include <ti/sysbios/knl/Task.h>

#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

#include "communication.h"


// по идее может быть любым от 1024 до 65535
#define TCPPORT_OSCI 1124

// Эти были обьявлены глобально гдето в файлах
char *IPAddr_cfg = "10.1.3.12"; // по идее этот айпи зависит от того какой у компа айпи в локальной сети
char *SubnetMask_cfg = "255.0.0.0";
char *DomainName_cfg = "PMCB"; // сюда по идее чо угодно можно писать

// Это присваивается в main.c
// И какой из них оставить??
// IPAddr_cfg = "10.3.5.38";
// SubnetMask_cfg = "255.255.255.0";
// DomainName_cfg = "PMCB";

#define NUM_TCP_WORKERS 1

#define PACKET_CMD_OSCI 0
#define PACKET_CMD_ECHO 1

// IVAN: max amount of measurement packets that can be requested
// in PACKET_CMD_OSCI
#define MAX_PACKETS_REQUEST_CNT 16

#pragma DATA_SECTION(CTOM_MSGRAM, "CTOM_MSGRAM")
volatile Uint16 CTOM_MSGRAM[0x100];

//#pragma DATA_SECTION(MTOC_MSGRAM, "MTOC_MSGRAM")
//volatile Uint16 MTOC_MSGRAM[0x100];

typedef __attribute__((packed)) struct {
	Uint16 C28_Errors;
	Uint16 C28_Errors_Latch;
	Uint16 FPGA_Errors;
	Uint16 FPGA_Errors_Latch;
	Uint16 SRAM_offset;
} CTOM_Data;

CTOM_Data ctom_data = {0};

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
		memcpy(&ctom_data, CTOM_MSGRAM, sizeof(data));

		// IVAN: sleep 25 system clock ticks
		Task_sleep(25);
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
//		taskParams.stackSize = 4096;
		taskParams.stackSize = 1024;

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

//#define PACKET_CMD_OSCI 0
//#define PACKET_CMD_ECHO 1
//
//// IVAN: max amount of measurement packets that can be requested
//// in PACKET_CMD_OSCI
//#define MAX_PACKETS_REQUEST_CNT 16

Void OscillogrammsWorker(UArg arg0, UArg arg1) {
	SOCKET client_fd = (SOCKET)arg0;
	Uint8 recv_buffer[TCP_PACKET_MAX_SIZE];

	System_printf("OscillogrammsWorker: started processing client_fd = %i", client_fd);

	while (1) {
		int n_bytes = recv(client_fd, recv_buffer, TCP_PACKET_MAX_SIZE, 0);
		if (n_bytes <= 0) {
			break;
		}

		Uint16 cmd = recv_buffer[0];
		switch (cmd) {
			case PACKET_CMD_OSCI: {
//				typedef struct {
//					Uint64 CycleCounter;
//					Uint8  WatchDog;
//				    Uint16 Current_1;
//				    Uint16 Current_2;
//				    Uint16 Voltage_Inp;
//				    Uint16 Voltage_Out;
//				    Uint16 C28_Errors;
//				    Uint16 C28_Errors_Latch;
//				    Uint16 FPGA_Errors;
//				    Uint16 FPGA_Errors_Latch;
//				    Uint16 FreeTimeCounter;
//				} DataToM3;
//				*Dest = CyclesCounter0; 		Dest++;  // 0
//				*Dest = CyclesCounter1; 		Dest++;  // 1
//				*Dest = CyclesCounter2; 		Dest++;  // 2
//				*Dest = CyclesCounter3; 		Dest++;  // 3
//				*Dest = Data.C28_Errors;		Dest++;  // 4
//				*Dest = Data.C28_Errors_Latch;  Dest++;  // 5
//				*Dest = Data.FPGA_Errors; 		Dest++;  // 6
//				*Dest = Data.FPGA_Errors_Latch; Dest++;  // 7
//				*Dest = Data.Voltage_Inp;		Dest++;  // 8
//				*Dest = Data.Voltage_Out;		Dest++;  // 9
//				*Dest = Data.Current_1;			Dest++;  // 10
//				*Dest = Data.Current_2;			Dest++;  // 11
//				*Dest = Data.WatchDog;			Dest++;  // 12
//				*Dest = Data.FreeTimeCounter;	Dest++;  // 13
//				Dest++;  // 14
//				Dest++;  // 15
			} break;

			case PACKET_CMD_ECHO: {

			} break;

			default: {
				System_printf(
					"OscillogrammsWorker: client_fd = %i sent unknown "
					"cmd = %i", client_fd, cmd
				);
			}
		}
	}

	fdClose(client_fd);
	return;
}
