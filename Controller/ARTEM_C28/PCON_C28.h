/*
 * PCON_C28.h
 *
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


#endif /* PCON_C28_H_ */
