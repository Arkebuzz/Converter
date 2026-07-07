#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include "F28M35x_Device.h"

Uint16 ErrorGetCurrent();
Uint16 ErrorGetLatch();
void ErrorSet(Uint8);
void ErrorReset(Uint8);
void ErrorResetAll();

#define ERROR_MAIN_TIMER_DEAD 		0  // Главный цикл завис, поскольку прерывание от таймера не пришло
#define ERROR_CPU_OVERLOAD			1  // Цикл ожидания был пропущен
#define ERROR_FPGA_CONNECT_FAIL		2  // Потеряно соединение с FPGA

#endif // ERROR_HANDLING_H
