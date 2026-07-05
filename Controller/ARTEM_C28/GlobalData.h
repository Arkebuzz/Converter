/*
 * PCON_C28.h
 *
 *  Created on: 11.12.2014
 *      Author: VKD
 */

#ifndef PCON_C28_H_
#define PCON_C28_H_

#include "string.h"
#include "DSP28x_Project.h"


#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#endif /* _FLASH */


// Глобальные параметры
#define C28_FREQ			100  // C28 работает на 100 мГц
#define MAIN_CYCLE_US		300  // Главный цикл С28 - 300 мкс


// Главный цикл
extern bool CPU_OverloadFlag;
extern Uint16 FreeTimeCounter;
extern Uint64 CyclesCounter;


// Данные FPGA
extern volatile Uint16 DMABufFPGA[128];

extern float Current_1;
extern float Current_2;
extern float Voltage_Inp;
extern float Voltage_Out;

extern Uint16 PCON_Errors;
extern Uint16 ADCH1_Errors;
extern Uint16 ADCH2_Errors;

extern Uint8 FPGA_WatchDog;
extern Uint8 FPGA_WatchDogRead;
extern Uint16 PWM_Counter;


// Осцилограммы
Uint16 *const S6_START  = (Uint16 *)0x00012000;
Uint16 *const S7_FINISH = (Uint16 *)0x00013FFF;

extern Uint32 startDest;  // Адрес новой записи в CtoM

#endif /* PCON_C28_H_ */
