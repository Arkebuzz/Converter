/*
 * MemFunctions.cpp
 *
 *  Created on: 11.12.2014
 *      Author: VKD
 */

#include "MemFunctions.h"
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File


unsigned int ReadWordFromEPIFIFO(void)
{
	unsigned int *XMEM_pw;
	unsigned int result;
	XMEM_pw = (unsigned int *)(0x7C00 + 0x038);
	result = *XMEM_pw;
	return result;
}

short SetEPIRead0Size(unsigned int read_size)
{
	unsigned int *XMEM_pw;
	XMEM_pw = (unsigned int *)(0x7C00 + 0x010);
	*XMEM_pw  =  read_size;
	return 0;
}

short SetEPIRead0Adress(unsigned long read_addr)
{
	unsigned long *XMEM_pw;
	XMEM_pw = (unsigned long *)(0x7C00 + 0x012);
	*XMEM_pw  =  read_addr;
	return 0;
}

short SetEPIRead0Count(unsigned int count)
{
	unsigned int *XMEM_pw;
	XMEM_pw = (unsigned int *)(0x7C00 + 0x014);
	*XMEM_pw  =  count;
	return 0;
}

unsigned int ReadEPIFIFOSize(void)
{
	unsigned int *XMEM_pw;
	unsigned int result;
	XMEM_pw = (unsigned int *)(0x7C00 + 0x036);
	result = *XMEM_pw;
	return result;
}

short ReadWordFrom_FPGA(short offset) {
	short *XMEM_pw;
	short result;
	if (offset > 0x80) {  // 128 слов
		return (0);
	}
	else {
		XMEM_pw = (short *) (0x340000 + offset); // CS1 выделено с 0x340000 по 0x037FFFF

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
		XMEM_pw = (short *) (0x340000 + offset); // CS1 выделено с 0x340000 по 0x037FFFF
		*XMEM_pw = value;
		return 0;
	}
}

short ReadWordFrom_SRAM(long offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x3FFFF) {return (0);} // 19 бит = 0xFFFFF
	else
		{
			XMEM_pw = (short *)(0x300000 + offset); // CS0 выделено с 0x300000 по 0x33FFFF
			result = *XMEM_pw;
			return result;
		}
}
short WriteWordTo_SRAM(long offset, short value)
{
	short *XMEM_pw;
	if (offset>0x3FFFF) {return (-1);} // не выходить за пределы выделенного пространства
	else
	{
		XMEM_pw = (short *)(0x300000 + offset); // CS0 выделено с 0x300000 по 0x033FFFF
		*XMEM_pw  =  value;
		return 0;
	}

}

/*
short WriteWordTo_FLASH(unsigned short offset, short value)
{
	short *XMEM_pw;
	if (offset>0x8000) {return (-1);}
	else
	{
		XMEM_pw = (short *)(0x00128000 + offset);
		*XMEM_pw  =  value;
		return 0;
	}

}

short ReadWordFrom_FLASH(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x8000) {return (0);}
	else
	{
		XMEM_pw = (short *)(0x00128000 + offset);
		result = *XMEM_pw;
		return result;
	}
}
*/


/*
short WriteWordTo_EMIF(unsigned short offset, short value)
{
	short *XMEM_pw;
	if (offset>0x3FF) {return (-1);}
	else
	{
		XMEM_pw = (short *)(0x00380000 + offset);
		*XMEM_pw  =  value;
		return 0;
	}

}

short ReadWordFrom_EMIF(long offset)
{
	short *XMEM_pw;
	short result;
//	if (offset>0x3FFF) {return (0);}
//	else
//	{
		XMEM_pw = (short *)(offset);
		result = *XMEM_pw;
		return result;
//	}
}

*/

short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value)
{
	float *XMEM_pw;
	if (offset>0x3FF) {return (-1);}
	else
	{
		XMEM_pw = (float *)(0x0003F800 + offset);
		*XMEM_pw  =  value;
		return 0;
	}
}


short WriteTo_CTOM_MSGRAM(unsigned short offset, short value)
{
	short *XMEM_pw;
	if (offset>0x3FF) {return (-1);}
	else
	{
		XMEM_pw = (short *)(0x0003F800 + offset);
		*XMEM_pw  =  value;
		return 0;
	}
}

short ReadFrom_CTOM_MSGRAM(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x3FF) {return (0);}
	else
	{
		XMEM_pw = (short *)(0x0003F800 + offset);
		result = *XMEM_pw;
		return result;
	}
}

short ReadFrom_MTOC_MSGRAM(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x3FF) {return (0);}
	else
		{
			XMEM_pw = (short *)(0x0003FC00 + offset);
			result = *XMEM_pw;
			return result;
		}
}

float ReadFrom_MTOC_MSGRAM_Float(short offset)
{
	float *XMEM_pw;
	float result;
	if (offset>0x3FF) {return (0);}
	else
		{
			XMEM_pw = (float *)(0x0003FC00 + offset);
			result = *XMEM_pw;
			return result;
		}
}


short WriteTo_SHRAM(unsigned short offset, short value)
{
	short *XMEM_pw;
	if (offset>0xFFF) {return (-1);}
	else
	{
		XMEM_pw = (short *)(0x00052000 + offset);
		*XMEM_pw  =  value;
		return 0;
	}

}

short ReadFrom_SHRAM(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0xFFF) {return (0);}
	else
	{
		XMEM_pw = (short *)(0x00052000 + offset);
		result = *XMEM_pw;
		return result;
	}
}

