/*
 * FPGA_DataAddresses.h
 *
 * Список адресов всех данных с FPGA, должен быть синхронизирован с аналогичным файлом для FPGA.
 */

#ifndef DATAADRESSES_H_
#define DATAADRESSES_H_


// FPGA -> C28		(0-49)

#define ADR_ERROR_PCON		5
#define ADR_ERROR_ADCH1		6
#define ADR_ERROR_ADCH2		7

#define ADR_VOLTAGE_INP		10
#define ADR_VOLTAGE_OUT		11
#define ADR_CURRENT_1		20
#define ADR_CURRENT_2		21


// C28 -> FPGA		(50-127)

#define ADR_WATCHDOG		50

#define ADR_PWM_COUNTER		60


#endif /* DATAADRESSES_H_ */
