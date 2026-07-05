/*
 * DataTransfer.h
 */

#ifndef DATA_TRANS_H_
#define DATA_TRANS_H_

#include "DSP28x_Project.h"

struct DataToM3 {
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
};

void ReadFPGAData(const Uint16 *Source, DataToM3 &Data);
void CheckFPGAConnect(const DataToM3 Data, Uint8 &WatchDog);
void WriteFPGAData(Uint8 WatchDog);
void WriteOSCI_S6S7(DataToM3 Data);

extern Uint16 *const S6_START;
extern Uint16 *const S7_FINISH;

#endif /* DATA_TRANS_H_ */
