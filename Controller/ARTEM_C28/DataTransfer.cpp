/*
 * DataTransfer.cpp
 */

#include "FPGA_DataAddresses.h"
#include "MemFunctions.h"
#include "ErrorList.h"
#include "DataTransfer.h"


Uint16 *const S6_START  = (Uint16 *)0x00012000;
Uint16 *const S7_FINISH = (Uint16 *)0x00013FFF;


void ReadFPGAData(const Uint16 *Source, DataToM3 &Data) {
	Data.WatchDog     = Source[ADR_WATCHDOG];
	Data.Current_1    = Source[ADR_CURRENT_1];
	Data.Current_2    = Source[ADR_CURRENT_2];
	Data.Voltage_Inp  = Source[ADR_VOLTAGE_INP];
	Data.Voltage_Out  = Source[ADR_VOLTAGE_OUT];
	Data.PCON_Errors  = Source[ADR_ERROR_PCON];
	Data.ADCH1_Errors = Source[ADR_ERROR_ADCH1];
	Data.ADCH2_Errors = Source[ADR_ERROR_ADCH2];
}


void CheckFPGAConnect(const DataToM3 Data, Uint8 &WatchDog) {
	if (WatchDog != Data.WatchDog)
		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
	else if (ErrorLatch & (1 << ERROR_FPGA_CONNECT_FAIL))
		ErrorReset(ERROR_FPGA_CONNECT_FAIL);

	WatchDog++;
}


void WriteFPGAData(Uint8 WatchDog) {
	WriteWordTo_FPGA(ADR_WATCHDOG, WatchDog);
}


void WriteOSCI_S6S7(const DataToM3 Data) {
	// Отправляем сигналы на M3 при заполнении пакетов.
	// Имеем 8192 слова, пакеты по 1024 слова => 8 пакетов.
	// В 1 пакете 64 набора данных с 64 моментов времени (64 итераций).
	// Набор данных - 16 слов (берем с запасом) - результат одного цикла C28.

	static Uint8 NumPocket = 0;
	static Uint8 NumIter = 0;
	static Uint16 *Dest = S6_START;

	if (NumIter == 64) {
		if (NumPocket == 0) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC9  == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC8 = 1;
		} else if (NumPocket == 1) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC10 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC9 = 1;
		} else if (NumPocket == 2) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC11 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC10 = 1;
		} else if (NumPocket == 3) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC12 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC11 = 1;
		} else if (NumPocket == 4) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC13 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC12 = 1;
		} else if (NumPocket == 5) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC14 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC13 = 1;
		} else if (NumPocket == 6) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC15 == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC14 = 1;
		} else if (NumPocket == 7) {
			if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC8  == 1)  ErrorSet(ERROR_OSCI_BUF_OVERFLOW);
			else ErrorReset(ERROR_OSCI_BUF_OVERFLOW);
			CtoMIpcRegs.CTOMIPCSET.bit.IPC15 = 1;
		}

		NumIter = 0;
		NumPocket++;
	}
	if (NumPocket == 8) {
		NumPocket = 0;
		Dest = S6_START;
	}

	Uint16 CyclesCounter0 = (Uint16) ((Data.CycleCounter & 0x000000000000FFFF) >> 0);
	Uint16 CyclesCounter1 = (Uint16) ((Data.CycleCounter & 0x00000000FFFF0000) >> 16);
	Uint16 CyclesCounter2 = (Uint16) ((Data.CycleCounter & 0x0000FFFF00000000) >> 32);
	Uint16 CyclesCounter3 = (Uint16) ((Data.CycleCounter & 0xFFFF000000000000) >> 48);

	*Dest = CyclesCounter0; 		Dest++;  // 0
	*Dest = CyclesCounter1; 		Dest++;  // 1
	*Dest = CyclesCounter2; 		Dest++;  // 2
	*Dest = CyclesCounter3; 		Dest++;  // 3
	*Dest = Data.C28_Errors;		Dest++;  // 4
	*Dest = Data.PCON_Errors; 		Dest++;  // 5
	*Dest = Data.ADCH1_Errors; 		Dest++;  // 6
	*Dest = Data.ADCH2_Errors; 		Dest++;  // 7
	*Dest = Data.Voltage_Inp;		Dest++;  // 8
	*Dest = Data.Voltage_Out;		Dest++;  // 9
	*Dest = Data.Current_1;			Dest++;  // 10
	*Dest = Data.Current_2;			Dest++;  // 11
	*Dest = Data.WatchDog;			Dest++;  // 12
	*Dest = Data.FreeTimeCounter;	Dest++;  // 13

	NumIter++;
}
