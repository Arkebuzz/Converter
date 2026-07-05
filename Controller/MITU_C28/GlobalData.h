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


#define C28_FREQ			100  // C28 работает на 100 м√ц
#define MAIN_CYCLE_US		300  // √лавный цикл —28 - 300 мкс


// main
extern bool CPU_OverloadFlag;
extern Uint16 FreeTimeCounter;
extern Uint16 WaitCyclesCounter;

extern volatile Uint16 DMABufFPGA[100];


#endif /* PCON_C28_H_ */
