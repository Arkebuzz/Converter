/*
 * MemFunctions.h
 *
 *  Created on: 11.12.2014
 *      Author: VKD
 */

#ifndef MEMFUNCTIONS_H_
#define MEMFUNCTIONS_H_

short ReadWordFrom_FPGA(short offset);

short WriteWordTo_FPGA(short offset, short value);

short ReadWordFrom_SRAM(long offset);

short WriteWordTo_SRAM(long offset, short value);

//short WriteWordTo_FLASH(unsigned short offset, short value);
//short ReadWordFrom_FLASH(short offset);

//short WriteWordTo_EMIF(unsigned short offset, short value);
//short ReadWordFrom_EMIF(long offset);

short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value);
short WriteTo_CTOM_MSGRAM(unsigned short offset, short value);
short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value);
short ReadFrom_CTOM_MSGRAM(short offset);
short ReadFrom_MTOC_MSGRAM(short offset);
float ReadFrom_MTOC_MSGRAM_Float(short offset);
short WriteTo_SHRAM(unsigned short offset, short value);
short ReadFrom_SHRAM(short offset);

unsigned int ReadWordFromEPIFIFO(void);
short SetEPIRead0Size(unsigned int read_size);
short SetEPIRead0Adress(unsigned long read_addr);
unsigned int ReadEPIFIFOSize(void);
short SetEPIRead0Count(unsigned int count);


#endif /* MEMFUNCTIONS_H_ */
