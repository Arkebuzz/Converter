#include "communication_FPGA.h"
#include "error_handling.h"

void ReadFPGAData(DataToM3 *Data) {
    // пока DMA не работает и поэтому читаем с адреса
    Uint16 Source = 0x340000; //FPGA_MSGRAM_LOCAL
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
