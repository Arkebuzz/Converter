/*
 * DataInit.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */
#include "GlobalData.h"

bool CPU_OverloadFlag = 0;
Uint16 FreeTimeCounter = 0;
Uint16 WaitCyclesCounter = 0;

volatile Uint16 DMABufFPGA[100];
