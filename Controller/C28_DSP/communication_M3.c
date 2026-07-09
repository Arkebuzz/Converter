#include "communication_M3.h"
#include "error_handling.h"

// IVAN: uses only 0x100 of 0x400 available
#pragma DATA_SECTION(CTOM_MSGRAM, "CTOM_MSGRAM")
volatile Uint16 CTOM_MSGRAM[0x100];

// 4 u16
typedef struct {
	Uint16 C28_Errors;
	Uint16 C28_Errors_Latch;
	Uint16 FPGA_Errors;
	Uint16 FPGA_Errors_Latch;
} Osci_Errors;

// 5 u16
typedef struct {
	Osci_Errors errors;
	Uint16 SRAM_offset;
} CTOM_Data;

// 16 u16
typedef struct {
	Uint16 CycleCounter[4];
	Uint16 Current_1;
	Uint16 Current_2;
	Uint16 Voltage_Inp;
	Uint16 Voltage_Out;
	Uint16 FreeTimeCounter;
	Uint16 WatchDog;
	Uint16 __pad[6];
} Osci_Packet;

// Communication with M3 uses SRAM6-SRAM7
#pragma DATA_SECTION(SHARERAMS6, "SHARERAMS6")
volatile Uint16 SHARERAMS6[0x1000];

#pragma DATA_SECTION(SHARERAMS7, "SHARERAMS7")
volatile Uint16 SHARERAMS7[0x1000];

#define S6_START SHARERAMS6
#define S7_END   (&SHARERAMS7[sizeof(SHARERAMS7) / sizeof(SHARERAMS7[0])])

void WriteToM3Data(const DataToM3 Data) {
	static volatile Osci_Packet *osci_packet_ptr = (volatile Osci_Packet *)S6_START;

	// IVAN: записываем пакет измерения
	Uint16 *cycle_counter = (Uint16 *)&Data.CycleCounter;
	osci_packet_ptr->CycleCounter[0] = cycle_counter[0];
	osci_packet_ptr->CycleCounter[1] = cycle_counter[1];
	osci_packet_ptr->CycleCounter[2] = cycle_counter[2];
	osci_packet_ptr->CycleCounter[3] = cycle_counter[3];
	osci_packet_ptr->Current_1 		 = Data.Current_1;
	osci_packet_ptr->Current_2 		 = Data.Current_2;
	osci_packet_ptr->Voltage_Inp 	 = Data.Voltage_Inp;
	osci_packet_ptr->Voltage_Out 	 = Data.Voltage_Out;
	osci_packet_ptr->FreeTimeCounter = Data.FreeTimeCounter;
	osci_packet_ptr->WatchDog 		 = (Uint16)Data.WatchDog;

	// IVAN: запишем данные (ошибки и оффсет в буфере) в CTOM
	volatile CTOM_Data *ctom_data = (volatile CTOM_Data *)CTOM_MSGRAM;
	ctom_data->errors.C28_Errors 		= Data.C28_Errors;
	ctom_data->errors.C28_Errors_Latch	= Data.C28_Errors_Latch;
	ctom_data->errors.FPGA_Errors 		= Data.FPGA_Errors;
	ctom_data->errors.FPGA_Errors_Latch = Data.FPGA_Errors_Latch;
	ctom_data->SRAM_offset =
			(Uint16 *)osci_packet_ptr - (Uint16 *)S6_START;

	osci_packet_ptr++;
	if ((Uint16 *)osci_packet_ptr >= (Uint16*)S7_END) {
		osci_packet_ptr = (Osci_Packet *)S6_START;
	}
}
