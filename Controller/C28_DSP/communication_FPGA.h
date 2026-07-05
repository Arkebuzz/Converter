#ifndef COMMUNICATION_FPGA_H
#define COMMUNICATION_FPGA_H

#include "F28M35x_Device.h"

typedef struct {
	Uint64 CycleCounter;
	Uint8  WatchDog;
    Uint16 Current_1;
    Uint16 Current_2;
    Uint16 Voltage_Inp;
    Uint16 Voltage_Out;
    Uint16 C28_Errors;
    Uint16 PCON_Errors;
    Uint16 ADCH1_Errors;
    Uint16 ADCH2_Errors;
    Uint16 FreeTimeCounter;
} DataToM3;

void ReadFPGAData(const Uint16 *Source, DataToM3 *Data);
void WriteFPGAData(Uint8 WatchDog);
void CheckFPGAConnect(const DataToM3 Data, Uint8 *WatchDog);

/*
 * Список адресов всех данных с FPGA, должен быть синхронизирован с аналогичным файлом для FPGA.
 */

// FPGA -> C28		(0-49)

#define ADR_ERROR_PCON		5
#define ADR_ERROR_ADCH1		6
#define ADR_ERROR_ADCH2		7

#define ADR_VOLTAGE_INP		10
#define ADR_VOLTAGE_OUT		11
#define ADR_CURRENT_1		20
#define ADR_CURRENT_2		21

// C28 -> FPGA		(50-127)

#define ADR_WATCHDOG		50
#define ADR_PWM_COUNTER		60

#endif // COMMUNICATION_FPGA_H
