/*
 * NETW_SocketComms.c
 *
 *  Created on: 7 дек. 2016 г.
 *      Author: Volkodav
 */

#include "GlobalData.h"


Void DataExchangeWorker(UArg arg0, UArg arg1)
{
	int clientfd = (int)arg0;
    int nbytes;
    int bytesSent;
    tBoolean flag = true;
    char buffer[TCPPACKETSIZE];

    short DataInCheckSum;
    short ReadChecksum;
    short ReadWord;
    unsigned long int i;

    //System_printf("tcpWorker: start clientfd = 0x%x\n", clientfd);

    /* Loop while we receive data */
    while (flag) {
        nbytes = recv(clientfd, buffer, TCPPACKETSIZE, 0);
        if (nbytes == (EXT_DATA_SIZE*sizeof(short)+sizeof(short)))
        {
        	IO_Count++;
        	DataInCheckSum = 0;
        	for (i=0;i<EXT_DATA_SIZE*sizeof(short);i=i+2)
				{
        			ReadWord = (short)(((unsigned short)(buffer[i])<<8)&0xFF00) | (((unsigned short)(buffer[i+1]))&0x00FF);
					DataInCheckSum = DataInCheckSum + ReadWord;
				}
        	ReadChecksum =  (((short)(buffer[nbytes-2])<<8)&0xFF00) | (((short)(buffer[nbytes-1]))&0x00FF);
        	if (DataInCheckSum==ReadChecksum)
        		{
        		IO_Success_Count++;
        		//Process incoming data
        		if (ModbusControlledInput==0)
					{
					for (i=0;i<((nbytes-2)/2);i++)
						{
						ReadWord = (((short)(buffer[i*2])<<8)&0xFF00) | (((short)(buffer[i*2+1]))&0x00FF);
						MTOC_Data[i] = ReadWord;
						}
					}
        		}
        	//Prepare output data
        	DataInCheckSum = 0;
        	for (i=0;i<CTOM_DATA_SIZE;i++)
        	{
        		DataInCheckSum = DataInCheckSum + CTOM_Data[i];
        		buffer[i*2+1] = ((char)CTOM_Data[i]);
        		buffer[i*2] = (char)(((unsigned short)CTOM_Data[i])>>8);
        	}
        	buffer[CTOM_DATA_SIZE*2+1] = ((char)DataInCheckSum);
        	buffer[CTOM_DATA_SIZE*2] = (char)(((unsigned short)DataInCheckSum)>>8);
        	/* Echo the data back */
            send(clientfd, buffer, CTOM_DATA_SIZE*2+2, 0 );
            if (bytesSent < 0 || bytesSent != CTOM_DATA_SIZE*2+2) {
                        System_printf("Error: send failed.\n");
                        flag = false;
            }
        }
        else {
            flag = false;
        }
    }

    //System_printf("tcpWorker stop clientfd = 0x%x\n", clientfd);
    close(clientfd);
}


Void OsciConnectionHandler(UArg arg0, UArg arg1)
{
    int clientfd = (int)arg0;
    int nbytes;
    tBoolean flag = true;
    char recv_buffer[TCPPACKETSIZE];
    char send_buffer_header[10];
    char *send_buffer;

    //unsigned long long Time;
    unsigned short Time0, Time1, Time2, Time3;

    short PriorityBuff=-1;
    unsigned long long BuffersTimeMin = 0xFFFFFFFFFFFFFFFF;
    unsigned long long BuffersTimes[SHMEM_BUFFERS_COUNT];
    //unsigned short BuffersPriorities[SHMEM_BUFFERS_COUNT];
    unsigned short BuffersReady[SHMEM_BUFFERS_COUNT];
    //unsigned long long TempTime;
    //unsigned short TempPrio;
    //unsigned short BufNum;
    short DataInCheckSum;
    short ReadChecksum;
    short ReadWord;
    unsigned short Setup_ID;
    unsigned short Setup_DATA;

    char *temp_buf;
    //Error_Block eb;
    unsigned short i;
    //unsigned short j;
    char cmd;
    unsigned int buffer_offset;

    int bytesSent, curBytesSent;
    int bytesToSend = DATA_PACKS_TOTAL*TOTAL_CHANNELS*2 + HEADER_SIZE;

    System_printf("tcpWorker: start clientfd = 0x%x\n", clientfd);

    for (i = 0; i < 10; i++)
    {
    	send_buffer_header[i] = 0;
    }


    /* Loop while we receive data */
    while (flag)
    {
    	curBytesSent = 0;
    	bytesSent = 0;
    	PriorityBuff=-1;
    	nbytes = recv(clientfd, recv_buffer, TCPPACKETSIZE, 0);

        if (nbytes	> 0)
        {
        	cmd = recv_buffer[0];

			switch(cmd)
			{
			case 1:
				if (nbytes == (1+EXT_DATA_SIZE*sizeof(short)+sizeof(short)))
				        {
				        	IO_Count++;
				        	DataInCheckSum = 0;
				        	for (i=0;i<EXT_DATA_SIZE*sizeof(short);i=i+2)
								{
				        			ReadWord = (short)(((unsigned short)(recv_buffer[i+1])<<8)&0xFF00) | (((unsigned short)(recv_buffer[i+2]))&0x00FF);
									DataInCheckSum = DataInCheckSum + ReadWord;
								}
				        	ReadChecksum =  (((short)(recv_buffer[nbytes-2])<<8)&0xFF00) | (((short)(recv_buffer[nbytes-1]))&0x00FF);
				        	if (DataInCheckSum==ReadChecksum)
				        		{
				        		IO_Success_Count++;
				        		//Process incoming data
				        		if (ModbusControlledInput==0)
									{
									for (i=0;i<((nbytes-3)/2);i++)
										{
										ReadWord = (((short)(recv_buffer[i*2+1])<<8)&0xFF00) | (((short)(recv_buffer[i*2+2]))&0x00FF);
										MTOC_Data[i] = ReadWord;
										}
									}
				        		}
				        	//Prepare output data
				        	DataInCheckSum = 0;
				        	for (i=0;i<CTOM_DATA_SIZE;i++)
				        	{
				        		DataInCheckSum = DataInCheckSum + CTOM_Data[i];
				        		recv_buffer[i*2+1] = ((char)CTOM_Data[i]);
				        		recv_buffer[i*2] = (char)(((unsigned short)CTOM_Data[i])>>8);
				        	}
				        	recv_buffer[CTOM_DATA_SIZE*2+1] = ((char)DataInCheckSum);
				        	recv_buffer[CTOM_DATA_SIZE*2] = (char)(((unsigned short)DataInCheckSum)>>8);
				        	// Echo the data back
				            send(clientfd, recv_buffer, CTOM_DATA_SIZE*2+2, 0 );
				        }
			break;

			case 7:
					temp_buf = send_buffer_header;
					*temp_buf = (char)TOTAL_CHANNELS;
					temp_buf += 2;
					Time3 = (unsigned short) ReadFrom_CTOM_MSGRAM(100); *temp_buf = (char) (Time3>>8); temp_buf++; *temp_buf = (char) (Time3);  temp_buf++;
					Time2 = (unsigned short) ReadFrom_CTOM_MSGRAM(101); *temp_buf = (char) (Time2>>8); temp_buf++; *temp_buf = (char) (Time2);  temp_buf++;
					Time1 = (unsigned short) ReadFrom_CTOM_MSGRAM(102); *temp_buf = (char) (Time1>>8); temp_buf++; *temp_buf = (char) (Time1);  temp_buf++;
					Time0 = (unsigned short) ReadFrom_CTOM_MSGRAM(103); *temp_buf = (char) (Time0>>8); temp_buf++; *temp_buf = (char) (Time0);  temp_buf++;
					send(clientfd, send_buffer_header, 10, 0);
			break;

			case 11: //Write Uint16 setup data
				ReadWord = (short)(((unsigned short)(recv_buffer[3])<<8)&0xFF00) | (((unsigned short)(recv_buffer[2]))&0x00FF);
				Setup_ID = ReadWord;
				ReadWord = (short)(((unsigned short)(recv_buffer[5])<<8)&0xFF00) | (((unsigned short)(recv_buffer[4]))&0x00FF);
				Setup_DATA = ReadWord;
				WriteTo_MTOC_MSGRAM(SETUP_DATA_START_ADDR, Setup_ID);
				WriteTo_MTOC_MSGRAM(SETUP_DATA_START_ADDR+1, Setup_DATA);
				HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET)|=IPC_MTOCIPCSET_IPC8;
					bytesSent = 0;
					while (bytesSent < 2)
					{
						curBytesSent = send(clientfd, &Setup_ID + bytesSent, 2 - bytesSent, 0 );
						bytesSent += curBytesSent;
					}
			break;

			case 12: //Read Uint16 setup data
				ReadWord = (short)(((unsigned short)(recv_buffer[3])<<8)&0xFF00) | (((unsigned short)(recv_buffer[2]))&0x00FF);
				Setup_ID = ReadWord;
				if (Setup_ID < SETUP_UINT_DATA_COUNT)
				{
					Uint16 SendVal = (Uint16)ReadFrom_CTOM_MSGRAM(SETUP_DATA_START_ADDR+Setup_ID);
					bytesSent = 0;
					while (bytesSent < 2)
					{
						curBytesSent = send(clientfd, &SendVal, 2 - bytesSent, 0 );
						bytesSent += curBytesSent;
					}
				}
				else
				{
					char errbyte2 = 0xF;
					bytesSent = 0;
					while (bytesSent < 1)
					{
						curBytesSent = send(clientfd, &errbyte2 + bytesSent, 1 - bytesSent, 0 );
						bytesSent += curBytesSent;
					}
				}
			break;


			case 14: //Write float setup data
				ReadWord = (short)(((unsigned short)(recv_buffer[3])<<8)&0xFF00) | (((unsigned short)(recv_buffer[2]))&0x00FF);
				Setup_ID = ReadWord;
				WriteTo_MTOC_MSGRAM(SETUP_DATA_START_ADDR, Setup_ID);
				float *floatbuf = (float *)(recv_buffer+4);
				float RecvData = *floatbuf;
				WriteTo_MTOC_MSGRAM_Float(SETUP_DATA_START_ADDR + 2, RecvData);
				HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET)|=IPC_MTOCIPCSET_IPC9;
					bytesSent = 0;
					while (bytesSent < 2)
					{
						curBytesSent = send(clientfd, &Setup_ID + bytesSent, 2 - bytesSent, 0 );
						bytesSent += curBytesSent;
					}
			break;


			case 15: //Read float setup data
				ReadWord = (short)(((unsigned short)(recv_buffer[3])<<8)&0xFF00) | (((unsigned short)(recv_buffer[2]))&0x00FF);
				Setup_ID = ReadWord;
				if (Setup_ID < SETUP_FLOAT_DATA_COUNT)
				{
					float SendAr = ReadFrom_CTOM_MSGRAM_Float(SETUP_DATA_START_ADDR+SETUP_UINT_DATA_COUNT+Setup_ID*2);
					bytesSent = 0;
					while (bytesSent < 4)
					{
						curBytesSent = send(clientfd, &SendAr + bytesSent, 4 - bytesSent, 0 );
						bytesSent += curBytesSent;
					}
				}
				else
				{
					char errbyte = 0xF;
					bytesSent = 0;
					while (bytesSent < 1)
					{
						curBytesSent = send(clientfd, &errbyte + bytesSent, 1 - bytesSent, 0 );
						bytesSent += curBytesSent;
					}
				}
			break;


			case 17: //Read osci channel gain/offset
				ReadWord = (short)(((unsigned short)(recv_buffer[3])<<8)&0xFF00) | (((unsigned short)(recv_buffer[2]))&0x00FF);
				Setup_ID = ReadWord;
				WriteTo_MTOC_MSGRAM(OSCI_SETUP_DATA_START_ADDR, Setup_ID);
				HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET)|=IPC_MTOCIPCSET_IPC6;
				Task_sleep(1);
				float DataArr[2];
				DataArr[0]=0; DataArr[1]=0;
				Uint16 TimeOutCounter=0;
				while (((HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_MTOCIPCFLG_IPC8) != 0) && TimeOutCounter<2) {TimeOutCounter++;Task_sleep(1);}
				if ((HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_MTOCIPCFLG_IPC8) == 0)
				{
					DataArr[0] = ReadFrom_CTOM_MSGRAM_Float(OSCI_SETUP_DATA_START_ADDR);
					DataArr[1] = ReadFrom_CTOM_MSGRAM_Float(OSCI_SETUP_DATA_START_ADDR+4);
				}
				bytesSent = 0;
				while (bytesSent < 8)
				{
					curBytesSent = send(clientfd, DataArr + bytesSent, 4 - bytesSent, 0 );
					bytesSent += curBytesSent;
				}
			break;

			case 18: //Set service osci channel to send
				ReadWord = (short)(((unsigned short)(recv_buffer[3])<<8)&0xFF00) | (((unsigned short)(recv_buffer[2]))&0x00FF);
				Setup_ID = ReadWord;
				ReadWord = (short)(((unsigned short)(recv_buffer[5])<<8)&0xFF00) | (((unsigned short)(recv_buffer[4]))&0x00FF);
				Setup_DATA = ReadWord;
				WriteTo_MTOC_MSGRAM(OSCI_SETUP_DATA_START_ADDR, Setup_ID);
				WriteTo_MTOC_MSGRAM(OSCI_SETUP_DATA_START_ADDR+2, Setup_DATA);
				HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET)|=IPC_MTOCIPCSET_IPC7;
					bytesSent = 0;
					while (bytesSent < 2)
					{
						curBytesSent = send(clientfd, &Setup_ID + bytesSent, 2 - bytesSent, 0 );
						bytesSent += curBytesSent;
					}
			break;


			case 50: //Send FLASH Init command
				bytesSent = 0;
				char errbyte3 = 0xF;
				if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC2) == IPC_CTOMIPCSTS_IPC2)
					{HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET)|=IPC_MTOCIPCSET_IPC2;errbyte3 = 0x0;}

				while (bytesSent < 1)
				{
					curBytesSent = send(clientfd, &errbyte3 + bytesSent, 1 - bytesSent, 0 );
					bytesSent += curBytesSent;
				}

			break;

			case 20:
				while (PriorityBuff==-1)
				{
				PriorityBuff=-1;
				BuffersTimeMin = 0xFFFFFFFFFFFFFFFF;
				for (i=0;i<SHMEM_BUFFERS_COUNT;i++)
				{
				BuffersReady[i]=0;
				Time3 = (unsigned short) ReadFrom_S6SHRAM(SHMEM_BUFFER_SIZE*i+1);
				Time2 = (unsigned short) ReadFrom_S6SHRAM(SHMEM_BUFFER_SIZE*i+2);
				Time1 = (unsigned short) ReadFrom_S6SHRAM(SHMEM_BUFFER_SIZE*i+3);
				Time0 = (unsigned short) ReadFrom_S6SHRAM(SHMEM_BUFFER_SIZE*i+4);
				BuffersTimes[i] = ((unsigned long long) (Time0) & 0x000000000000FFFF) |
									((((unsigned long long) (Time1))<<16) & 0x00000000FFFF0000) |
									((((unsigned long long) (Time2))<<32) & 0x0000FFFF00000000) |
									((((unsigned long long) (Time3))<<48) & 0xFFFF000000000000);
					switch (i)
					{
					case 0: if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC10) == IPC_CTOMIPCSTS_IPC10) {BuffersReady[i]=1;}; break;
					case 1: if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC11) == IPC_CTOMIPCSTS_IPC11) {BuffersReady[i]=1;}; break;
					case 2: if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC12) == IPC_CTOMIPCSTS_IPC12) {BuffersReady[i]=1;}; break;
					case 3: if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC13) == IPC_CTOMIPCSTS_IPC13) {BuffersReady[i]=1;}; break;
					case 4: if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC14) == IPC_CTOMIPCSTS_IPC14) {BuffersReady[i]=1;}; break;
					case 5: if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC15) == IPC_CTOMIPCSTS_IPC15) {BuffersReady[i]=1;}; break;
					case 6: if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC16) == IPC_CTOMIPCSTS_IPC16) {BuffersReady[i]=1;}; break;
					}

					if (BuffersReady[i]==1 && BuffersTimes[i]<BuffersTimeMin) {BuffersTimeMin = BuffersTimes[i]; PriorityBuff=i;}
				}

					switch (PriorityBuff)
					{
					case 0:
					if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC10) == IPC_CTOMIPCSTS_IPC10)
									{
										buffer_offset = 0*SHMEM_BUFFER_SIZE;
										send_buffer = (char *)(0x20014000 + buffer_offset*2);
										bytesSent = 0;
										while (bytesSent < bytesToSend)
										{
											curBytesSent = send(clientfd, send_buffer + bytesSent, bytesToSend - bytesSent, 0 );
											bytesSent += curBytesSent;
										}
										HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCACK_IPC10;
									}
					break;

					case 1:
									if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC11) == IPC_CTOMIPCSTS_IPC11)
													{
														buffer_offset = 1*SHMEM_BUFFER_SIZE;
														send_buffer = (char *)(0x20014000 + buffer_offset*2);
														bytesSent = 0;
														while (bytesSent < bytesToSend)
														{
															curBytesSent = send(clientfd, send_buffer + bytesSent, bytesToSend - bytesSent, 0 );
															bytesSent += curBytesSent;
														}
														HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCACK_IPC11;
													}
					break;

					case 2:
									if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC12) == IPC_CTOMIPCSTS_IPC12)
									{
										buffer_offset = 2*SHMEM_BUFFER_SIZE;
										send_buffer = (char *)(0x20014000 + buffer_offset*2);
										bytesSent = 0;
										while (bytesSent < bytesToSend)
											{
											curBytesSent = send(clientfd, send_buffer + bytesSent, bytesToSend - bytesSent, 0 );
											bytesSent += curBytesSent;
											}
										HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCACK_IPC12;
									}
					break;


					case 3:
									if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC13) == IPC_CTOMIPCSTS_IPC13)
									{
										buffer_offset = 3*SHMEM_BUFFER_SIZE;
										send_buffer = (char *)(0x20014000 + buffer_offset*2);
										bytesSent = 0;
										while (bytesSent < bytesToSend)
											{
											curBytesSent = send(clientfd, send_buffer + bytesSent, bytesToSend - bytesSent, 0 );
											bytesSent += curBytesSent;
											}
										HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCACK_IPC13;
									}
					break;

					case 4:
									if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC14) == IPC_CTOMIPCSTS_IPC14)
									{
											buffer_offset = 4*SHMEM_BUFFER_SIZE;
											send_buffer = (char *)(0x20014000 + buffer_offset*2);
											bytesSent = 0;
											while (bytesSent < bytesToSend)
												{
												curBytesSent = send(clientfd, send_buffer + bytesSent, bytesToSend - bytesSent, 0 );
												bytesSent += curBytesSent;
												}
										HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCACK_IPC14;
									}
					break;

					case 5:
									if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC15) == IPC_CTOMIPCSTS_IPC15)
									{
											buffer_offset = 5*SHMEM_BUFFER_SIZE;
											send_buffer = (char *)(0x20014000 + buffer_offset*2);
											bytesSent = 0;
											while (bytesSent < bytesToSend)
												{
												curBytesSent = send(clientfd, send_buffer + bytesSent, bytesToSend - bytesSent, 0 );
												bytesSent += curBytesSent;
												}
										HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCACK_IPC15;
									}
					break;

					case 6:
									if ( (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_CTOMIPCSTS_IPC16) == IPC_CTOMIPCSTS_IPC16)
									{
											buffer_offset = 6*SHMEM_BUFFER_SIZE;
											send_buffer = (char *)(0x20014000 + buffer_offset*2);
											bytesSent = 0;
											while (bytesSent < bytesToSend)
												{
												curBytesSent = send(clientfd, send_buffer + bytesSent, bytesToSend - bytesSent, 0 );
												bytesSent += curBytesSent;
												}
										HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK)|=IPC_CTOMIPCACK_IPC16;
									}
					break;


					}
				}
				break;

			default:
				flag = false;
			break;

			}


        }
        else
        {
            flag = false;
        }
    }

    close(clientfd);
}


Void DataExchangeListener(UArg arg0, UArg arg1)
{
	int status;
	int lSocket;
    int clientfd;
    struct sockaddr_in sLocalAddr;
    struct sockaddr_in client_addr;
    int optval;
    int optlen = sizeof(optval);
    socklen_t addrlen=sizeof(client_addr);
    Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;

    lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (lSocket < 0) {
        System_printf("tcpHandler: socket failed\n");
        goto shutdown;
    }

    memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
    sLocalAddr.sin_family = AF_INET;
    sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sLocalAddr.sin_port = htons(arg0);

    status = bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr));
    if (status < 0) {
        System_printf("tcpHandler: bind failed\n");
        goto shutdown;
    }


    status = listen(lSocket, NUMTCPWORKERS);
    	if (status == -1) {
    		System_printf("Error: listen failed.\n");
    		goto shutdown;
    	}

    optval = 1;
    if (setsockopt(lSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        System_printf("tcpHandler: setsockopt failed\n");
        goto shutdown;
    }

    optval = 1;
    if (setsockopt(lSocket, IPPROTO_TCP, TCP_NODELAY, &optval, optlen) < 0) {
            System_printf("tcpHandler: setsockopt TCP_NODELAY failed\n");
            goto shutdown;
        }
    System_flush();
    // Wait for incoming request
    while ((clientfd =
    		accept(lSocket, (struct sockaddr*)&client_addr, &addrlen)) != -1) {

        System_printf("tcpHandler: Creating thread clientfd = %d\n", clientfd);

        // Init the Error_Block
        Error_init(&eb);

        // Initialize the defaults and set the parameters.
        Task_Params_init(&taskParams);
        taskParams.arg0 = (UArg)clientfd;
        taskParams.stackSize = 1280;
        taskHandle = Task_create((Task_FuncPtr)DataExchangeWorker, &taskParams, &eb);
        if (taskHandle == NULL) {
            System_printf("tcpHandler: Failed to create new Task\n");
            close(clientfd);
            }

        /* addrlen is a value-result param, must reset for next accept call */
        addrlen = sizeof(client_addr);
    }

    System_printf("Error: accept failed.\n");

    shutdown:
    	if (lSocket > 0) {
    		close(lSocket);
    	}
}


Void OscillogrammsListener(UArg arg0, UArg arg1)
{
    int lSocket;
    struct sockaddr_in sLocalAddr;
    int clientfd;
    struct sockaddr_in client_addr;
    int addrlen=sizeof(client_addr);
    int optval;
    int optlen = sizeof(optval);
    int status;
    Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;

    lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (lSocket < 0) {
        System_printf("tcpHandler: socket failed\n");
        goto shutdown;
    }

    memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
    sLocalAddr.sin_family = AF_INET;
    sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sLocalAddr.sin_port = htons(arg0);

    status = bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr));
    if (status < 0) {
        System_printf("tcpHandler: bind failed\n");
        goto shutdown;
    }

    status = listen(lSocket, NUMTCPWORKERS);
	if (status == -1) {
		System_printf("Error: listen failed.\n");
		goto shutdown;
	}

	optval = 1;
    if (setsockopt(lSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        System_printf("tcpHandler: setsockopt failed\n");
        goto shutdown;
    }

    optval = 1;
    if (setsockopt(lSocket, IPPROTO_TCP, TCP_NODELAY, &optval, optlen) < 0) {
		System_printf("tcpHandler: setsockopt TCP_NODELAY failed\n");
		goto shutdown;
	}


    /* Wait for incoming request */
    while ((clientfd =
       		accept(lSocket, (struct sockaddr*)&client_addr, &addrlen)) != -1) {
        System_printf("tcpHandler: Creating thread clientfd = %d\n", clientfd);

        /* Init the Error_Block */
        Error_init(&eb);

        /* Initialize the defaults and set the parameters. */
        Task_Params_init(&taskParams);
        taskParams.arg0 = (UArg)clientfd;
        taskParams.stackSize = 1024;
        taskHandle = Task_create((Task_FuncPtr)OsciConnectionHandler, &taskParams, &eb);
        if (taskHandle == NULL) {
            System_printf("tcpHandler: Failed to create new Task\n");
            close(clientfd);
		}

		/* addrlen is a value-result param, must reset for next accept call */
		addrlen = sizeof(client_addr);
    }

    shutdown:
	if (lSocket > 0) {
		close(lSocket);
	}
}
