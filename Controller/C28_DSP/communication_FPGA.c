#include "communication_FPGA.h"
#include "error_handling.h"


short WriteWordToFPGA(short offset, short value) {
	short *XMEM_pw;
	if (offset > 0x80) {  // 128 слов
		return -1;
	} else {
		XMEM_pw = (short *) (0x340000 + offset); // CS1 выделено с 0x340000 по 0x340080
		*XMEM_pw = value;
		return 0;
	}
}


void ReadFPGAData(const Uint16 *Source, DataToM3 *Data) {
	volatile Uint8 *sixseven = 0x340000;
	Data->WatchDog     = sixseven[ADR_WATCHDOG];
	Data->Current_1    = sixseven[ADR_CURRENT_1];
	Data->Current_2    = sixseven[ADR_CURRENT_2];
	Data->Voltage_Inp  = sixseven[ADR_VOLTAGE_INP];
	Data->Voltage_Out  = sixseven[ADR_VOLTAGE_OUT];
	Data->FPGA_Errors  = sixseven[ADR_ERRORS];
	Data->FPGA_Errors_Latch = sixseven[ADR_ERRORS_LATCH];
}


void CheckFPGAConnect(const DataToM3 Data, Uint8 *WatchDog) {
	if (*WatchDog != Data.WatchDog) {
		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
	} else {
		ErrorReset(ERROR_FPGA_CONNECT_FAIL);
	}
	(*WatchDog)++;
	WriteWordToFPGA(ADR_WATCHDOG, *WatchDog);
}


void WriteFPGAData(Uint16 CTRL_Converter) {
	WriteWordToFPGA(ADR_CONV_CTRL, CTRL_Converter);
}
