/*
 * InitFunctions.h
 *
 *  Created on: 30.07.2014
 *      Author: Daler
 */

#ifndef INITFUNCTIONS_H_
#define INITFUNCTIONS_H_

#include "GlobalData.h"

#ifdef __cplusplus
extern "C" {
#endif

void Init_SPI(void);
void INIT_GPIO_Setup(void);
void INIT_Setup_Timers(unsigned short, unsigned short);
void INIT_Setup_DMA(volatile Uint16 *, Uint16);
void INIT_Start_Timers(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* INITFUNCTIONS_H_ */
