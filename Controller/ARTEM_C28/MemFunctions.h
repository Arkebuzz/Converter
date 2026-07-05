/*
 * MemFunctions.h
 */

#ifndef MEMFUNCTIONS_H_
#define MEMFUNCTIONS_H_

#include "DSP28x_Project.h"

short ReadWordFrom_FPGA(short offset);
short WriteWordTo_FPGA(short offset, short value);

short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value);
short WriteTo_CTOM_MSGRAM(unsigned short offset, short value);
short ReadFrom_MTOC_MSGRAM(short offset);
float ReadFrom_MTOC_MSGRAM_Float(short offset);

#endif /* MEMFUNCTIONS_H_ */
