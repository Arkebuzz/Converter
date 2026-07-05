
#include "F28M35x_Device.h"

#include "communication_FPGA.h"
#include "error_handling.h"

void ReadFPGAData(const Uint16 *Source, DataToM3 *Data) {
	// COPY PASTE

	Data->WatchDog     = Source[ADR_WATCHDOG];
	Data->Current_1    = Source[ADR_CURRENT_1];
	Data->Current_2    = Source[ADR_CURRENT_2];
	Data->Voltage_Inp  = Source[ADR_VOLTAGE_INP];
	Data->Voltage_Out  = Source[ADR_VOLTAGE_OUT];
	Data->PCON_Errors  = Source[ADR_ERROR_PCON];
	Data->ADCH1_Errors = Source[ADR_ERROR_ADCH1];
	Data->ADCH2_Errors = Source[ADR_ERROR_ADCH2];
}


void CheckFPGAConnect(const DataToM3 Data, Uint8 *WatchDog) {
	// COPY PASTE

	if (*WatchDog != Data.WatchDog) {
		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
	} else if (ErrorLatch & (1 << ERROR_FPGA_CONNECT_FAIL)) {
		ErrorReset(ERROR_FPGA_CONNECT_FAIL);
	}

	*WatchDog++;
}


short WriteWordToFPGA(short offset, short value) {
	// COPY PASTE

	short *XMEM_pw;
	if (offset > 0x80) {  // 128 слов
		return -1;
	} else {
		XMEM_pw = (short *) (0x340000 + offset); // CS1 выделено с 0x340000 по 0x340080
		*XMEM_pw = value;
		return 0;
	}
}

void WriteFPGAData(Uint8 WatchDog) {
	// COPY PASTE

	WriteWordToFPGA(ADR_WATCHDOG, WatchDog);
}
