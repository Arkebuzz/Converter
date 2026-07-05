/*
 * MemFunctions.cpp
 *
 *  Created on: 11.12.2014
 *      Author: VKD
 */

#include "MemFunctions.h"
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File


short ReadWordFrom_FPGA(short offset) {
	short *XMEM_pw;
	short result;
	if (offset > 0x80) {  // 128 слов
		return (0);
	}
	else {
		XMEM_pw = (short *) (0x340000 + offset); // CS1 выделено с 0x340000 по 0x340080
		result = *XMEM_pw;
		return result;
	}
}


short WriteWordTo_FPGA(short offset, short value) {
	short *XMEM_pw;
	if (offset > 0x80) {  // 128 слов
		return (-1);
	}
	else {
		XMEM_pw = (short *) (0x340000 + offset); // CS1 выделено с 0x340000 по 0x340080
		*XMEM_pw = value;
		return 0;
	}
}


short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value) {
	float *XMEM_pw;
	if (offset > 0x3FF) {
		return -1;
	}
	else {
		XMEM_pw = (float *) (0x0003F800 + offset);
		*XMEM_pw = value;
		return 0;
	}
}


short WriteTo_CTOM_MSGRAM(unsigned short offset, short value) {
	short *XMEM_pw;
	if (offset > 0x3FF) {
		return -1;
	}
	else {
		XMEM_pw = (short *) (0x0003F800 + offset);
		*XMEM_pw = value;
		return 0;
	}
}


short ReadFrom_MTOC_MSGRAM(short offset) {
	short *XMEM_pw;
	short result;
	if (offset > 0x3FF) {
		return 0;
	}
	else {
		XMEM_pw = (short *) (0x0003FC00 + offset);
		result = *XMEM_pw;
		return result;
	}
}

float ReadFrom_MTOC_MSGRAM_Float(short offset) {
	float *XMEM_pw;
	float result;
	if (offset > 0x3FF) {
		return 0;
	}
	else {
		XMEM_pw = (float *) (0x0003FC00 + offset);
		result = *XMEM_pw;
		return result;
	}
}
