/*
 * ModbusFunctions.c
 *
 *  Created on: 7 дек. 2016 г.
 *      Author: Volkodav
 */

#include "GlobalData.h"

enum ThreadState eGetPollingThreadState( void );
enum ThreadState eGetPollingThreadState2( void );
void     eSetPollingThreadState( enum ThreadState eNewState );
void     eSetPollingThreadState2( enum ThreadState eNewState );
void* pvPollingThread( void *pvParameter );
void* pvPollingThread2( void *pvParameter2 );


Void ModbusThread(UArg arg0, UArg arg1)
{

	Task_Handle taskHandle_MBUS;
	Task_Params taskParams_MBUS;
	Error_Block eb_MBUS;

	Task_Params_init(&taskParams_MBUS);
	Error_init(&eb_MBUS);

	if( eMBTCPInit( MB_TCP_PORT_USE_DEFAULT ) != MB_ENOERR )
	     {
	       System_printf("Can't initialize modbus stack!\n");
	       System_flush();
	     }
	     else
		{
	    	System_printf("Modbus init successfull. Starting modbus thread (port 502).\n");
	    	System_flush();
			eSetPollingThreadState( STOPPED );
			taskParams_MBUS.stackSize = 1024;
			taskParams_MBUS.priority = 1;
			taskHandle_MBUS = Task_create((Task_FuncPtr)pvPollingThread, &taskParams_MBUS, &eb_MBUS);
			if (taskHandle_MBUS == NULL) {
					System_printf("Can't start protocol stack!\n");}
			System_flush();
		}

	while (true)
	{
		Task_sleep(100);
	}

}


Void ModbusThread2(UArg arg0, UArg arg1)
{

    Task_Handle taskHandle_MBUS;
    Task_Params taskParams_MBUS;
    Error_Block eb_MBUS;

    Task_Params_init(&taskParams_MBUS);
    Error_init(&eb_MBUS);

    if( eMBTCPInit( 504 ) != MB_ENOERR )
         {
           System_printf("Can't initialize modbus stack!\n");
           System_flush();
         }
         else
        {
            System_printf("Modbus init successfull. Starting modbus thread (port 504).\n");
            System_flush();
            eSetPollingThreadState2( STOPPED );
            taskParams_MBUS.stackSize = 1024;
            taskParams_MBUS.priority = 1;
            taskHandle_MBUS = Task_create((Task_FuncPtr)pvPollingThread2, &taskParams_MBUS, &eb_MBUS);
            if (taskHandle_MBUS == NULL) {
                    System_printf("Can't start protocol stack!\n");}
            System_flush();
        }

    while (true)
    {
        Task_sleep(100);
    }

}

void* pvPollingThread( void *pvParameter )
{
	short i;
    eSetPollingThreadState( RUNNING );

    if( eMBEnable(  ) == MB_ENOERR )
    {
        do
        {

        	for (i=MTOC_DATA_SIZE;i<REG_HOLDING_NREGS;i++)
			{
				if ((i - MTOC_DATA_SIZE)<CTOM_DATA_SIZE)
				{
				usRegHoldingBuf[i] = CTOM_Data[i - MTOC_DATA_SIZE];
				}
			}

            if( eMBPoll(  ) != MB_ENOERR ) {break;}

            //if (usRegHoldingBuf[5]==2015)  {ModbusControlledInput = 0;}
            //else {ModbusControlledInput = 1;}
            ModbusControlledInput = 0;


            if (ModbusControlledInput == 1)
            {
            	for (i=0;i<MTOC_DATA_SIZE;i++)
				{
					if (i<REG_HOLDING_NREGS)
					{
						MTOC_Data[i] = usRegHoldingBuf[i];
					}
				}

            }

        }
        while( eGetPollingThreadState(  ) != SHUTDOWN );
    }

    ( void )eMBDisable(  );

    eSetPollingThreadState( STOPPED );
    return 0;
}


void* pvPollingThread2( void *pvParameter )
{
    short i;
    eSetPollingThreadState2( RUNNING );

    if( eMBEnable(  ) == MB_ENOERR )
    {
        do
        {

            for (i=MTOC_DATA_SIZE;i<REG_HOLDING_NREGS;i++)
            {
                if ((i - MTOC_DATA_SIZE)<CTOM_DATA_SIZE)
                {
                usRegHoldingBuf[i] = CTOM_Data[i - MTOC_DATA_SIZE];
                }
            }

            if( eMBPoll(  ) != MB_ENOERR ) {break;}

            //if (usRegHoldingBuf[5]==2015)  {ModbusControlledInput = 0;}
            //else {ModbusControlledInput = 1;}
            ModbusControlledInput = 1;


            if (ModbusControlledInput == 1)
            {
                for (i=0;i<MTOC_DATA_SIZE;i++)
                {
                    if (i<REG_HOLDING_NREGS)
                    {
                        MTOC_Data[i] = usRegHoldingBuf[i];
                    }
                }

            }

        }
        while( eGetPollingThreadState2(  ) != SHUTDOWN );
    }

    ( void )eMBDisable(  );

    eSetPollingThreadState2( STOPPED );
    return 0;
}

enum ThreadState eGetPollingThreadState(  )
{
    enum ThreadState eCurState;

    //( void )pthread_mutex_lock( &xLock );
    eCurState = ePollThreadState;
    //( void )pthread_mutex_unlock( &xLock );

    return eCurState;
}

enum ThreadState eGetPollingThreadState2(  )
{
    enum ThreadState eCurState;

    //( void )pthread_mutex_lock( &xLock );
    eCurState = ePollThreadState2;
    //( void )pthread_mutex_unlock( &xLock );

    return eCurState;
}

void eSetPollingThreadState( enum ThreadState eNewState )
{
    //( void )pthread_mutex_lock( &xLock );
    ePollThreadState = eNewState;
    //( void )pthread_mutex_unlock( &xLock );
}

void eSetPollingThreadState2( enum ThreadState eNewState )
{
    //( void )pthread_mutex_lock( &xLock );
    ePollThreadState2 = eNewState;
    //( void )pthread_mutex_unlock( &xLock );
}

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}



