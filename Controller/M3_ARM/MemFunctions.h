/*
 * MemFunctions.h
 *
 *  Created on: 07 мая 2015 г.
 *      Author: MATLAB_user
 */

#ifndef MEMFUNCTIONS_H_
#define MEMFUNCTIONS_H_


short WriteTo_FPGA(short offset, short value);

short ReadFrom_FPGA(short offset);

short WriteTo_SRAM(long offset, short value);

short ReadFrom_SRAM(long offset);

short WriteTo_MTOC_MSGRAM(unsigned short offset, short value);

short WriteTo_MTOC_MSGRAM_Float(unsigned short offset, float value);

short ReadFrom_MTOC_MSGRAM(short offset);

short WriteTo_S6SHRAM(unsigned short offset, short value);

short ReadFrom_S6SHRAM(short offset);

short ReadFrom_CTOM_MSGRAM(short offset);

float ReadFrom_CTOM_MSGRAM_Float(short offset);


#endif /* MEMFUNCTIONS_H_ */
