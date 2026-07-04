/*
 * ERROR_LIST.h
 *
 *  Created on: 23.01.2015
 *      Author: VKD
 */

#ifndef ERROR_LIST_H_
#define ERROR_LIST_H_

#define OK 1000

#define ERROR_DMA_OVERRIDE 			1  // DMA не успел выполнить копирование данных до перезапуска
#define ERROR_MAIN_TIMER_DEAD 		2  // √лавный цикл завис, поскольку прерывание от таймера не пришло
#define ERROR_CPU_OVERLOAD			3  // ÷икл ожидани€ был пропущен
#define ERROR_FPGA_CONNECT_FAIL		4  // ѕотер€но соединение с FPGA

#endif /* ERROR_LIST_H_ */
