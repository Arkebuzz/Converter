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

#define ERROR_MAIN_TIMER_DEAD 		2  // Главный цикл завис, поскольку прерывание от таймера не пришло
#define ERROR_CPU_OVERLOAD			3  // Цикл ожидания был пропущен
#define ERROR_FPGA_CONNECT_FAIL		4  // Потеряно соединение с FPGA
#define ERROR_OSCI_BUF_OVERFLOW		5  // M3 не успевает читать осцилограммы

#endif /* ERRORLIST_H_ */
