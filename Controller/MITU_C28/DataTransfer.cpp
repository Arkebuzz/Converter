/*
 * DataTransfer.cpp
 */

#include "GlobalData.h"
#include "FPGA_DataAddresses.h"
#include "MemFunctions.h"
#include "ErrorList.h"


void ReadFPGAData() {
	FPGA_WatchDogRead = DMABufFPGA[ADR_WATCHDOG];

	if (FPGA_WatchDog != FPGA_WatchDogRead) {
		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
	}

	PCON_Errors = DMABufFPGA[ADR_ERROR_PCON];
	ADCH1_Errors = DMABufFPGA[ADR_ERROR_ADCH1];
	ADCH2_Errors = DMABufFPGA[ADR_ERROR_ADCH2];

	Current_1 = ((float)DMABufFPGA[ADR_CURRENT_1] - 1706.66) * 0.366210938;
	Current_2 = ((float)DMABufFPGA[ADR_CURRENT_2] - 1706.66) * 0.366210938;
	Voltage_Inp = ((float)DMABufFPGA[ADR_VOLTAGE_INP] - 2070.0) * 1.271565755;
	Voltage_Out = ((float)DMABufFPGA[ADR_VOLTAGE_OUT] - 2070.0) * 1.271565755;
}


void WriteFPGAData() {
	WriteWordTo_FPGA(ADR_WATCHDOG, ++FPGA_WatchDog);
}


void WriteOSCI_S6S7() {
	// Отправляем сигналы на M3 при заполнении пакетов.
	// Имеем 8192 слова, пакеты по 1024 слова => 8 пакетов.
	// В 1 пакете 64 набора данных с 64 моментов времени (64 итераций).
	// Набор данных - 16 слов (берем с запасом) - результат одного цикла C28.

	static Uint8 NumPocket = 0;
	static Uint8 NumIter = 0;
	static Uint16 *Dest = S6_START;

	if (NumIter == 64) {
		if (NumPocket == 0) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC8 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC8 = 1;
		} else if (NumPocket == 1) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC9 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC9 = 1;
		} else if (NumPocket == 2) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC10 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC10 = 1;
		} else if (NumPocket == 3) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC11 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC11 = 1;
		} else if (NumPocket == 4) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC12 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC12 = 1;
		} else if (NumPocket == 5) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC13 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC13 = 1;
		} else if (NumPocket == 6) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC14 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC14 = 1;
		} else if (NumPocket == 7) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC15 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC15 = 1;
		}

		NumIter = 0;
		NumPocket++;
	}
	if (NumPocket == 8) {
		NumPocket = 0;
		Dest = S6_START;
	}

	startDest = (Uint32)Dest;

	Uint16 CyclesCounter0 = (Uint16) ((CyclesCounter & 0x000000000000FFFF) >> 0);
	Uint16 CyclesCounter1 = (Uint16) ((CyclesCounter & 0x00000000FFFF0000) >> 16);
	Uint16 CyclesCounter2 = (Uint16) ((CyclesCounter & 0x0000FFFF00000000) >> 32);
	Uint16 CyclesCounter3 = (Uint16) ((CyclesCounter & 0xFFFF000000000000) >> 48);

	*Dest = CyclesCounter0; 				Dest++;  // 0
	*Dest = CyclesCounter1; 				Dest++;  // 1
	*Dest = CyclesCounter2; 				Dest++;  // 2
	*Dest = CyclesCounter3; 				Dest++;  // 3
	*Dest = PCON_Errors; 					Dest++;  // 4
	*Dest = ADCH1_Errors; 					Dest++;  // 5
	*Dest = ADCH2_Errors; 					Dest++;  // 6
	*Dest = DMABufFPGA[ADR_VOLTAGE_INP];	Dest++;  // 7
	*Dest = DMABufFPGA[ADR_VOLTAGE_OUT];	Dest++;  // 8
	*Dest = DMABufFPGA[ADR_CURRENT_1];		Dest++;  // 9
	*Dest = DMABufFPGA[ADR_CURRENT_2];		Dest++;  // 10

	// Потом ещё напихаем, в том числе какое было требуемое напряжение
	NumIter++;
}
