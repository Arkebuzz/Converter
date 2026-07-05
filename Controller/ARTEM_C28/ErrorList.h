/*
 * ErrorList.h
 */

#ifndef ERRORLIST_H_
#define ERRORLIST_H_

#include "DSP28x_Project.h"

void ErrorSet(Uint8 ErrNum);
void ErrorReset(Uint8 ErrNum);

extern Uint16 ErrorLatch;

#define ERROR_MAIN_TIMER_DEAD 		0  // Главный цикл завис, поскольку прерывание от таймера не пришло
#define ERROR_CPU_OVERLOAD			1  // Цикл ожидания был пропущен
#define ERROR_FPGA_CONNECT_FAIL		2  // Потеряно соединение с FPGA
#define ERROR_OSCI_BUF_OVERFLOW		3  // M3 не успевает читать осцилограммы

#endif /* ERRORLIST_H_ */
