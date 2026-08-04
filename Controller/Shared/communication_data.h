#ifndef COMMUNICATION_DATA_H
#define COMMUNICATION_DATA_H

#include <stdint.h>

/////////// C28 <-> M3 ///////////

typedef struct {
	uint16_t C28_Errors;
	uint16_t C28_Errors_Latch;
	uint16_t FPGA_Errors;
	uint16_t FPGA_Errors_Latch;
} Osci_Errors;

typedef struct {
	uint16_t Buf[128];
	uint32_t Address;
	uint16_t DataSize;
	enum Flash_Cmd {
		FLASH_CMD_DONE = 0,
		FLASH_CMD_BUSY,
		FLASH_CMD_READ,
		FLASH_CMD_WRITE,
		FLASH_CMD_ERASE_4K,

		FLASH_CMD_SZ,
	} Cmd;
} Flash_Data;

typedef struct {
	Osci_Errors OsciErrors;
	uint16_t SRAM_Offset;
	Flash_Data FlashData;
} CTOM_Data;

// 16 u16
// TODO: Щас ошибки дублируются в пакете и в респонсе
typedef struct {
	uint16_t CycleCounter[4];
	Osci_Errors OsciErrors;
	uint16_t Current_1;
	uint16_t Current_2;
	uint16_t Voltage_Inp;
	uint16_t Voltage_Out;
	uint16_t FreeTimeCounter;
	uint16_t WatchDog;
	uint16_t __pad[2];
} Osci_Packet;

/////////// TCP ///////////

typedef enum {
	PACKET_CMD_ECHO = 0,
	PACKET_CMD_OSCI,
	PACKET_CMD_FULL,
	PACKET_CMD_INFO,
	PACKET_CMD_FLASH,
} Packet_Cmd;

typedef struct {
	uint16_t cmd;
	uint16_t arg;
} Osci_Request;

typedef struct {
	uint16_t cmd;
	uint16_t len;
	Osci_Errors errors;
} Osci_Response;

#endif // COMMUNICATION_DATA_H
