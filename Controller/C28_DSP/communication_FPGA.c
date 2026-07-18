#include "communication_FPGA.h"
#include "error_handling.h"

#pragma DATA_SECTION(FPGA_MSGRAM, "FPGA_MSGRAM")
volatile Uint16 FPGA_MSGRAM[128];

void ReadFPGAData(DataToM3 *Data) {
    // пока DMA не работает и поэтому читаем с адреса
    // Uint16 *Source = (Uint16 *)0x340000; //FPGA_MSGRAM_LOCAL
	Data->WatchDog     = FPGA_MSGRAM[ADR_WATCHDOG];
	Data->Current_1    = FPGA_MSGRAM[ADR_CURRENT_1];
	Data->Current_2    = FPGA_MSGRAM[ADR_CURRENT_2];
	Data->Voltage_Inp  = FPGA_MSGRAM[ADR_VOLTAGE_INP];
	Data->Voltage_Out  = FPGA_MSGRAM[ADR_VOLTAGE_OUT];
	Data->FPGA_Errors  = FPGA_MSGRAM[ADR_ERRORS];
	Data->FPGA_Errors_Latch = FPGA_MSGRAM[ADR_ERRORS_LATCH];
}

void CheckFPGAConnect(const DataToM3 Data, Uint8 *WatchDog) {
	if (*WatchDog != Data.WatchDog) {
		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
	} else {
		ErrorReset(ERROR_FPGA_CONNECT_FAIL);
	}
	(*WatchDog)++;
	FPGA_MSGRAM[ADR_WATCHDOG] = *WatchDog;
}

void WriteFPGAData(Uint16 PWM_Counter, Bool mode_up, Bool converter_on, Bool reset_errors) {
	Uint16 CTRL_Converter =
			(PWM_Counter << 3) |
			(((Uint16)mode_up) << 2) |
			(((Uint16)converter_on) << 1) |
			((Uint16)reset_errors);
	FPGA_MSGRAM[ADR_CONV_CTRL] = CTRL_Converter; // Запись в FPGA
}
