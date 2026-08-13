#include "communication_M3.h"
#include "error_handling.h"

#pragma DATA_SECTION(CTOM_MSGRAM, "CTOM_MSGRAM")
volatile Uint16 CTOM_MSGRAM[0x400];
volatile CTOM_Data *CTOM_DATA = (CTOM_Data *)CTOM_MSGRAM;

#pragma DATA_SECTION(MTOC_MSGRAM, "MTOC_MSGRAM")
volatile Uint16 MTOC_MSGRAM[0x400];
volatile const MTOC_Data *MTOC_DATA = (MTOC_Data *)MTOC_MSGRAM;

// Communication with M3 uses SRAM6-SRAM7
#pragma DATA_SECTION(SHARERAMS6, "SHARERAMS6")
volatile Uint16 SHARERAMS6[4096];

#pragma DATA_SECTION(SHARERAMS7, "SHARERAMS7")
volatile Uint16 SHARERAMS7[4096];

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
	osci_packet_ptr->OsciErrors.C28_Errors 		  = Data.C28_Errors;
	osci_packet_ptr->OsciErrors.C28_Errors_Latch  = Data.C28_Errors_Latch;
	osci_packet_ptr->OsciErrors.FPGA_Errors 	  = Data.FPGA_Errors;
	osci_packet_ptr->OsciErrors.FPGA_Errors_Latch = Data.FPGA_Errors_Latch;
	osci_packet_ptr->Current_1 		 = Data.Current_1;
	osci_packet_ptr->Current_2 		 = Data.Current_2;
	osci_packet_ptr->Voltage_Inp 	 = Data.Voltage_Inp;
	osci_packet_ptr->Voltage_Out 	 = Data.Voltage_Out;
	osci_packet_ptr->FreeTimeCounter = Data.FreeTimeCounter;
	osci_packet_ptr->WatchDog 		 = (Uint16)Data.WatchDog;

	// IVAN: запишем данные (ошибки и оффсет в буфере) в CTOM
	CTOM_DATA->OsciErrors.C28_Errors 		= Data.C28_Errors;
	CTOM_DATA->OsciErrors.C28_Errors_Latch	= Data.C28_Errors_Latch;
	CTOM_DATA->OsciErrors.FPGA_Errors 		= Data.FPGA_Errors;
	CTOM_DATA->OsciErrors.FPGA_Errors_Latch = Data.FPGA_Errors_Latch;
	CTOM_DATA->SRAM_Offset = (Uint16 *)osci_packet_ptr - (Uint16 *)S6_START;

	osci_packet_ptr++;
	if ((Uint16 *)osci_packet_ptr >= (Uint16*)S7_END) {
		osci_packet_ptr = (Osci_Packet *)S6_START;
	}
}
