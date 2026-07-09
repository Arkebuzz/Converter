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
	Data->WatchDog     = Source[ADR_WATCHDOG];
	Data->Current_1    = Source[ADR_CURRENT_1];
	Data->Current_2    = Source[ADR_CURRENT_2];
	Data->Voltage_Inp  = Source[ADR_VOLTAGE_INP];
	Data->Voltage_Out  = Source[ADR_VOLTAGE_OUT];
	Data->FPGA_Errors  = Source[ADR_ERRORS];
	Data->FPGA_Errors_Latch = Source[ADR_ERRORS_LATCH];
}


void CheckFPGAConnect(const DataToM3 Data, Uint8 *WatchDog) {
	if (*WatchDog != Data.WatchDog) {
		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
	} else {
		ErrorReset(ERROR_FPGA_CONNECT_FAIL);
	}
	*WatchDog++;
	WriteWordToFPGA(ADR_WATCHDOG, *WatchDog);
}


void WriteFPGAData(Uint16 CTRL_Converter, Uint16 PWM_Counter) {
	WriteWordToFPGA(ADR_CONV_CTRL, CTRL_Converter);
	WriteWordToFPGA(ADR_PWM_COUNTER, PWM_Counter);
}
