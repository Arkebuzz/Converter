#ifndef COMMUNICATION_M3_H
#define COMMUNICATION_M3_H

#include "communication_FPGA.h"

typedef struct {
	Uint16 C28_Errors;
	Uint16 C28_Errors_Latch;
	Uint16 FPGA_Errors;
	Uint16 FPGA_Errors_Latch;
} Osci_Errors;

typedef struct {
	Uint16 FlashBuf[128];
	Uint32 FlashAddress;
	Uint16 FlashDataSize;
	enum {
		FLASH_CMD_DONE,
		FLASH_CMD_BUSY,
		FLASH_CMD_READ,
		FLASH_CMD_WRITE,
		FLASH_CMD_ERASE_4K,
	} FlashCmd;
} Flash_Data;

typedef struct {
	Osci_Errors errors;
	Uint16 SRAM_offset;
	Flash_Data FlashData;
} CTOM_Data;

typedef struct {
	Uint16 CycleCounter[4];
	Osci_Errors errors;
	Uint16 Current_1;
	Uint16 Current_2;
	Uint16 Voltage_Inp;
	Uint16 Voltage_Out;
	Uint16 FreeTimeCounter;
	Uint16 WatchDog;
	Uint16 __pad[2];
} Osci_Packet;

extern volatile CTOM_Data *CTOM_DATA;

short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value);
short WriteTo_CTOM_MSGRAM(unsigned short offset, short value);
short ReadFrom_MTOC_MSGRAM(short offset);
float ReadFrom_MTOC_MSGRAM_Float(short offset);

void WriteToM3Data(const DataToM3 Data);

#endif // COMMUNICATION_M3_H
