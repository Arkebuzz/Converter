/*
 * ErrorList.h
 *
 *  Created on: 23.01.2015
 *      Author: VKD
 */

#ifndef ERRORLIST_H_
#define ERRORLIST_H_

#include "GlobalData.h"

void ErrorSet(Uint32 ErrNum);
void ErrorReset(void);

#define OK 1000

#define ERROR_DMA_OVERRIDE 			1  // DMA не успел выполнить копирование данных до перезапуска
#define ERROR_MAIN_TIMER_DEAD 		2  // √лавный цикл завис, поскольку прерывание от таймера не пришло
#define ERROR_CPU_OVERLOAD			3  // ÷икл ожидани€ был пропущен
#define ERROR_FPGA_CONNECT_FAIL		4  // ѕотер€но соединение с FPGA

#endif /* ERRORLIST_H_ */
