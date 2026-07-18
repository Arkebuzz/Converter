#ifndef COMMUNICATION_FPGA_H
#define COMMUNICATION_FPGA_H

#include "F28M35x_Device.h"

typedef struct {
    Uint16 C28_Errors;
    Uint16 C28_Errors_Latch;
    Uint16 FPGA_Errors;
    Uint16 FPGA_Errors_Latch;
	Uint64 CycleCounter;
    Uint16 Current_1;
    Uint16 Current_2;
    Uint16 Voltage_Inp;
    Uint16 Voltage_Out;
    Uint16 FreeTimeCounter;
	Uint8  WatchDog;
} DataToM3;

void CheckFPGAConnect(const DataToM3 Data, Uint8 *WatchDog);
void ReadFPGAData(DataToM3 *Data);
void WriteFPGAData(Uint16 PWM_Counter, Bool mode_up, Bool converter_on, Bool reset_errors);

/*
 * Список адресов всех данных с FPGA, должен быть синхронизирован с аналогичным файлом для FPGA.
 */

// volatile Uint16 FPGA_MSGRAM_LOCAL[51];

// FPGA -> C28		(0-49)

#define ADR_ERRORS			5
#define ADR_ERRORS_LATCH	6

#define ADR_VOLTAGE_INP		10
#define ADR_VOLTAGE_OUT		11
#define ADR_CURRENT_1		20
#define ADR_CURRENT_2		21

// C28 -> FPGA		(50-127)

#define ADR_WATCHDOG		50
#define ADR_CONV_CTRL       60
#define ADR_PWM_COUNTER		65

#endif // COMMUNICATION_FPGA_H
