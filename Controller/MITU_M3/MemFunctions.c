/*
 * MemFunctions.c
 *
 *  Created on: 07 мая 2015 г.
 *      Author: MATLAB_user
 */


#include "MemFunctions.h"


short WriteTo_FPGA_TEST(long offset, long value) // ONLY for pins integrity test
{
	long *XMEM_pw;
	if (offset>0x80000) {return (-1);}
	else
	{
		XMEM_pw = (long *)(0x80000000 + offset*2); // 14 бит = 0x3FFF
		*XMEM_pw  =  value;
		return 0;
	}

}

short WriteTo_FPGA(short offset, short value)
{
	short *XMEM_pw;
	if (offset>0x2000) {return (-1);}
	else
	{
		XMEM_pw = (short *)(0x80000000 + offset*2); // 14 бит = 0x3FFF
		*XMEM_pw  =  value;
		return 0;
	}
}

short WriteTo_SRAM(long offset, short value)
{
	short *XMEM_pw;
	if (offset>0xFFFFF) {return (-1);} // 19 бит = 0xFFFFF
	else
	{
		XMEM_pw = (short *)(0x60000000 + offset*2);
		*XMEM_pw  =  value;
		return 0;
	}

}


short ReadFrom_FPGA(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x2000) {return (0);} // 14 бит = 0x3FFF
	else
		{
			XMEM_pw = (short *)(0x80000000 + offset*2);
			result = *XMEM_pw;
			return result;
		}
}



short ReadFrom_SRAM(long offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0xFFFFF) {return (0);} // 19 бит = 0xFFFFF
	else
		{
			XMEM_pw = (short *)(0x60000000 + offset*2);
			result = *XMEM_pw;
			return result;
		}
}


short ReadFrom_S6SHRAM(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x4000) {return (0);}
	else
		{
			XMEM_pw = (short *)(0x20014000   + offset*2);
			result = *XMEM_pw;
			return result;
		}
}


short ReadFrom_CTOM_MSGRAM(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x3FF) {return (0);}
	else
		{
			XMEM_pw = (short *)(0x2007F000 + offset*2);
			result = *XMEM_pw;
			return result;
		}
}

float ReadFrom_CTOM_MSGRAM_Float(short offset)
{
	float *XMEM_pw;
	float result;
	if (offset>0x3FF) {return (0);}
	else
		{
			XMEM_pw = (float *)(0x2007F000 + offset*2);
			result = *XMEM_pw;
			return result;
		}
}


short WriteTo_MTOC_MSGRAM(unsigned short offset, short value)
{
	short *XMEM_pw;
	if (offset>0x3FF) {return (-1);}
	else
	{
		XMEM_pw = (short *)(0x2007F800 + offset*2);
		*XMEM_pw  =  value;
		return 0;
	}

}

short WriteTo_MTOC_MSGRAM_Float(unsigned short offset, float value)
{
	float *XMEM_pw;
	if (offset>0x3FF) {return (-1);}
	else
	{
		XMEM_pw = (float *)(0x2007F800 + offset*2);
		*XMEM_pw  =  value;
		return 0;
	}

}

short ReadFrom_MTOC_MSGRAM(short offset)
{
	short *XMEM_pw;
	short result;
	if (offset>0x3FF) {return (0);}
	else
	{
		XMEM_pw = (short *)(0x2007F800 + offset*2);
		result = *XMEM_pw;
		return result;
	}
}



short WriteTo_S6SHRAM(unsigned short offset, short value)
{
	short *XMEM_pw;
	if (offset>0x3FF) {return (-1);}
	else
	{
		XMEM_pw = (short *)(0x20014000  + offset*2);
		*XMEM_pw  =  value;
		return 0;
	}

}

