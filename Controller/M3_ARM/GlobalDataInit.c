/*
 * GlobalDataInit.c
 *
 *  Created on: 7 дек. 2016 г.
 *      Author: Volkodav
 */
#include "GlobalData.h"


char * IPAddr_cfg = "10.1.3.12";
char * SubnetMask_cfg = "255.0.0.0";
char * DomainName_cfg = "PMCB";

short ModbusControlledInput = 0;

short FirstThreadRun = 1;
short SecondThreadRun = 0;

short* osciBuf;
int osciBufSize = 0;  // размер массива osciBuf
int osciBufCnt = 0;	// текущее количество элементов в массиве osciBuf
int lastSent = 0;	// индекс последнего отправленного элемента в массиве osciBuf
int lastWritten = 0;	// индекс последнего записанного элемента в массиве osciBuf
char bOverload = 0;

unsigned int PWMAmpData=0;

short* CTOM_Data;
short* ExtControl_Data;
short* MTOC_Data;

unsigned short IO_Count;
unsigned short IO_Success_Count;

unsigned short MTOC_IO_Count=0;
unsigned short CTOM_IO_Count=0;

/* ----------------------- MBUS variables ---------------------------------*/
USHORT   usRegInputStart = REG_INPUT_START;
USHORT   usRegInputBuf[REG_INPUT_NREGS];
USHORT   usRegHoldingStart = REG_HOLDING_START;
USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];


void Buffers_Init(void)
{
	/* Make sure Error_Block is initialized */
	Error_Block eb;
	Error_init(&eb);

	CTOM_Data = Memory_alloc(NULL, CTOM_DATA_SIZE*sizeof(short), 0, &eb);
	    if (CTOM_Data == NULL) {
	        System_printf("failed to alloc memory!! \n");
	    }
	memset(CTOM_Data,0,CTOM_DATA_SIZE*sizeof(short));

	MTOC_Data = Memory_alloc(NULL, MTOC_DATA_SIZE*sizeof(short), 0, &eb);
		    if (MTOC_Data == NULL) {
		        System_printf("failed to alloc memory!! \n");
		    }
	memset(MTOC_Data,0,MTOC_DATA_SIZE*sizeof(short));

	ExtControl_Data = Memory_alloc(NULL, EXT_DATA_SIZE*sizeof(short), 0, &eb);
		if (ExtControl_Data == NULL) {
		 System_printf("failed to alloc memory!! \n");
		}
	memset(ExtControl_Data,0,EXT_DATA_SIZE*sizeof(short));

}
