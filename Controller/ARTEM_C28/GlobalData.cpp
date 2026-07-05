/*
 * DataInit.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */
#include "GlobalData.h"

// Главный цикл
bool CPU_OverloadFlag = 0;
Uint16 FreeTimeCounter = 0;
Uint64 CyclesCounter = 0;


// Данные FPGA
volatile Uint16 DMABufFPGA[128];

float Current_1;
float Current_2;
float Voltage_Inp;
float Voltage_Out;

Uint16 PCON_Errors;
Uint16 ADCH1_Errors;
Uint16 ADCH2_Errors;

Uint8 FPGA_WatchDog = 0;
Uint8 FPGA_WatchDogRead = 0;
Uint16 PWM_Counter = 0;

// Осцилограммы
Uint32 startDest;  // Адрес новой записи в CtoM
