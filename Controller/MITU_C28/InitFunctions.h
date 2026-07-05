/*
 * InitFunctions.h
 *
 *  Created on: 30.07.2014
 *      Author: Daler
 */

#include "DSP28x_Project.h"
#ifndef INITFUNCTIONS_H_
#define INITFUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

void Init_SPI(void);
Uint32 INIT_GPIO_Setup(void);
Uint32 INIT_AssignInterrupts(void);
Uint32 INIT_Peripherals(void);
Uint32 INIT_Setup_ADC(void);
Uint32 INIT_Setup_Timers(unsigned short,unsigned short,unsigned short);
void Setup_DMA(Uint16 MeasBufSizeHalf);
void INIT_StartTimers(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* INITFUNCTIONS_H_ */
