/*
 * DataProcessing.c
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include "GlobalData.h"

Void KeepAliveTask(UArg arg0, UArg arg1)
{
	unsigned int i=0;
	unsigned int j=0;

	int k;

	GPIO_write(TMDXDOCKH52C1_ERR, GPIO_TURN_ON);

	while (true)
	{
		i++;
		j++;

		if (j==40)
		{
			GPIO_write(TMDXDOCKH52C1_LED, GPIO_TURN_ON);
		}
		else if (j>80)
		{
			j=0;
			GPIO_write(TMDXDOCKH52C1_LED, GPIO_TURN_OFF);
		}


		if (i%2==0)
		{
			GPIO_write(TMDXDOCKH52C1_WD, GPIO_TURN_OFF);
		}
		else
		{
			GPIO_write(TMDXDOCKH52C1_WD, GPIO_TURN_ON);
		}

		//if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC7) == IPC_CTOMIPCSTS_IPC7)
		//{
			for (k=0;k<CTOM_DATA_SIZE;k++)
			{
				CTOM_Data[k] = ReadFrom_CTOM_MSGRAM(k);
			}
		//	HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCSTS_IPC7;
			CTOM_IO_Count++;
		//}


		//if ( (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_MTOCIPCFLG_IPC8) == 0)
		//{
			for (k=0;k<MTOC_DATA_SIZE;k++)
				{
				WriteTo_MTOC_MSGRAM(k,MTOC_Data[k]);
				}
		//	HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET)|=IPC_MTOCIPCSET_IPC8;
			MTOC_IO_Count++;
		//}

		Task_sleep(25);
	}

}


