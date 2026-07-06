/*
 * SPIFlashFunctions.h
 *
 *  Created on: 25.05.2015
 *      Author: GPS
 *      Company: PMCB
 */



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																						 					//
//											ОПИСАНИЕ									 					//
//																											//
//				Класс реализует чтение и запись данных из/в FLASH память Atmel AT25DF321A.		 			//
//			При первом вызове класс необходимо проинициировать с помощью процедуры				 			//
//			SPIFlashReaderInit, необходимым параметром, CycleTime, которой является тактовый	 			//
//			период в микросекундах.																 			//
//																											//
//				Порядок работы:																	 			//
//			Необходимая операция инициируется с помощью соответствующей процедуры. Её			 			//
//			выполнение происходит "пошагово" путём вызова функции SPIFlashStep, которая			 			//
//			возвращает значение в зависимости от состояния выполнения текущей операции:			 			//
//			0 - операция не закончена; 1 - операция закончена; -1 - возникла ошибка.			 			//
//																						 					//
//				Поддерживаются следующие операции:												 			//
//																											//
//	Запись одиночного слова по указанному адресу - InitWriteWordToFlash(..)				 					//
//																											//
//		void InitWriteWordToFlash(Uint32 WordAdress, Uint16 Data)											//
//		WordAdress	- адрес флеш памяти для записи слова													//
// 		Data		- слово																					//
//																											//
//	Запись массива слов в диапазон адресов памяти  	 									 					//
//	из диапазона адресов микроконтроллера 			 - InitWriteBlockToFlash(..)						 	//
//																					 						//
//		void InitWriteBlockToFlash(short BlockNumber, Uint32 LengthInWords, long StartSourceAddress )		//
//		BlockNumber 		-	адрес 4КБ-блока с которого начинать запись			 						//
//		LengthInWords		-	длина записывемого массива в словах					 						//
//		StartSourceAddress 	-	начальный адрес записываемых данных					 						//
//																											//
//	Чтение одиночного слова из указанного адреса - InitReadWordFromFlash(..)		 						//
//		void InitReadWordFromFlash(Uint32 WordAdress)														//
//		WordAdress	-	адрес для чтения																	//
//																					 						//
//	Чтение массива слов	из диапазона адресов	 - InitReadBlockFromFlash(..)		 						//
//																					 						//
//		void InitReadBlockFromFlash(Uint32 WordAdress,Uint32 LengthInWords,Uint16 *StartDestinationAddress) //
//		WordAdress					-		адрес начала массива для чтения									//
//		LengthInWords				-		длина массива в словах											//
//		*StartDestinationAddress	-		адрес начала массива для записи прочитанных значений			//
//																					 						//
//																					 						//
//																					 						//
//			Получить код ошибки 						 - Uint16 GetErrorCode()			 				//
//			Получить результат чтения единичного слова	 - Uint16 GetReadValue()	 		 				//
//																					 						//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef SPIFLASHFUNCTIONS_H_
#define SPIFLASHFUNCTIONS_H_

/////////////////// PIN/REG DEFS //////////////////////////
#define nCS_SPI GpioG1DataRegs.GPADAT.bit.GPIO19
#define RX_FIFO_RESET SpiaRegs.SPIFFRX.bit.RXFIFORESET
#define RX_FIFO_STATUS SpiaRegs.SPIFFRX.bit.RXFFST
#define TX_FIFO_STATUS SpiaRegs.SPIFFTX.bit.TXFFST
#define TX_SPI_BUFFER SpiaRegs.SPITXBUF
#define RX_SPI_BUFFER SpiaRegs.SPIRXBUF
/////////////////// PIN/REG DEFS //////////////////////////

//////////////// FLASH COMMANDS  /////////////////
////////////////  READ COMMANDS  /////////////////
#define Com_ReadArray 0x03
#define Com_DualReadArray 0x3B
/////////  Program and Erase COMMANDS  ///////////
#define Com_BlockErase4K 0x20
#define Com_BlockErase32K 0x52
#define Com_BlockErase64K 0xD8
#define Com_ChipErase 0xC7
#define Com_BytePageProgram 0x02
#define Com_DualBytePageProgram 0xA2
#define Com_ProgramEraseSuspend 0xB0
#define Com_ProgramEraseResume 0xD0
//////////////// PROTECT COMMANDS  ///////////////
#define Com_WriteEnable 0x06
#define Com_WriteDisable 0x04
#define Com_ProtectSector 0x36
#define Com_UnProtectSector 0x39
//////////////// GLOBAL COMMANDS  ////////////////
#define Com_ReadSectorProtectionRegisters 0x3C
//////////////// SECURITY COMMANDS  //////////////
#define Com_SectorLockdown 0x33
#define Com_FreezeSectorLockdownState 0x34
#define Com_ReadSectorLockdownRegisters 0x35
#define Com_ProgramOTPSecurityRegister 0x9B
#define Com_ReadOTPSecurityRegister 0x77
//////////////// STATUS REG COMMANDS  ////////////
#define Com_ReadStatusRegister 0x05
#define Com_WriteStatusRegisterByte1 0x01
#define Com_WriteStatusRegisterByte2 0x31
//////////////// MISC COMMANDS  //////////////////
#define Com_Reset 0xF0
#define Com_ReadManufacturerAndDeviceID 0x9F
#define Com_DeepPowerDown 0xB9
#define Com_ResumeFromDeepPowerDown 0xAB
//////////////// FLASH COMMANDS  /////////////////
//////////////////////////////////////////////////

//////////////// FSM STATES ///////////////////////
#define ReadWord 0x01
#define ReadBlock 0x02
#define WriteWord 0x03
#define WriteBlock 0x04
#define FlashReaderInit 0x00
#define OperationFinished 0xFF
#define ErrorOccured 0x77
//////////////// FSM STATES ///////////////////////

/////////////// RESTRICTIONS //////////////////////
#define OP_MAXTIMEOUT 1200/StepTime
#define RETRY_MAXTIMEOUT 2
/////////////// RESTRICTIONS //////////////////////

/////////////	ERROR CODES 	///////////
#define OP_TIMEOUTERROR 0x50
#define RETRY_TIMEOUTERROR 0x51
#define FLASHOVERLOADERROR 0x60
#define ZEROWRITEBLOCKERROR 0x61
#define CYCLETIMEUNDEFINED 0x70
////////////	ERROR CODES 	///////////

//////// INTERNAL REFACTORING //////////
#define isSingleData 1
#define notSingleData 0
//////// INTERNAL REFACTORING //////////

//////////////// PARAMS /////////////////
#define PageSizeInWords 128
#define PagesInBlock 16
#define Blocks4InBlocks32 8
#define BytesIn32KB 32768
#define BytesIn64KB 65536
//////////////// PARAMS /////////////////

//////////// ERASE FSM //////////////////
#define BLOCK_4K 1
#define BLOCK_32K 2
#define BLOCK_64K 3
//////////// ERASE FSM //////////////////

#include "DSP28x_Project.h"



class SPIFlashReader
{
private:
	short DEBUGVAR;
	short CurrentOperation;
	short NextStep;
	short appearance;
	Uint32 WriteWordAdress;
	Uint32 ReadWordAdress;
	Uint32 ReadBlockAdress;
	Uint16 *DestinationAddress;
	Uint16 WriteWordData;
	Uint32 OP_TIMEOUT;
	Uint16 RETRY_TIMEOUT;
	Uint16 StatusResult;
	short WriteBlockNumber;
	Uint32 LengthOfBlock;
	long SourceAddress;
	Uint32 LengthOfCurrentPage;
	Uint32 AmountOfPages;
	Uint32 RemainingPagesInBlock;
	short AmountOf4KBBlocks;
	short AmountOf64KBBlocks;
	short AmountOf32KBBlocks;
	short SkipErase;
	bool LastDataFlag;
	bool SingleWordFlag;
	short StepTime;
	Uint16 DummyWord;
	Uint16 ERRORCODE;
	Uint16 ReadResult;
	Int32 temp2;
	Uint16 Block4KErased[64];



	void SetBlockErased(Uint32 WordAddr)
	{
		WordAddr = WordAddr*2;
		if (WordAddr <= 0x3FFFFF)
			{
			Uint16 Block16num = (Uint16)((WordAddr&0x00FFF000)>>12);
			Uint16 Block64num = ((Block16num)>>4)&0x003F;
			Uint16 NumInBlock64 = Block16num&0x000F;
			Uint16 Mask = 0x0001<<NumInBlock64;
			Block4KErased[Block64num] = Block4KErased[Block64num]|Mask;
			}
	}

	Uint16 GetIfBlockErased(Uint32 WordAddr)
	{
		WordAddr = WordAddr*2;
		if (WordAddr <= 0x3FFFFF)
		{
			Uint16 Block16num = (Uint16)((WordAddr&0x00FFF000)>>12);
			Uint16 Block64num = ((Block16num)>>4)&0x003F;
			Uint16 NumInBlock64 = Block16num&0x000F;
			Uint16 Mask = 0x0001<<NumInBlock64;
			if ((Block4KErased[Block64num]&Mask) != 0)
			{
				return(1);
			}
			else
			{
				return(0);
			}

		}
		else
		{
			return(0);
		}

	}


	// Преобразует байт-код комманды (перемещает в старший байт)
	Uint16 SetCommand(Uint16 Command)
	{
		return(((Command)<<8)&0xFF00);
	}


	// Шлёт команду WE на флешку. Возвращает 1 = DONE. 0 = BUSY.
		bool WriteEnable()
		{
			Uint16 Command = SetCommand(Com_WriteEnable);

			if (appearance==1)
				{
					if (RX_FIFO_STATUS == 1)
						{
							nCS_SPI = 1;
							RX_FIFO_RESET=0;
							RX_FIFO_RESET=1;
							appearance=0;
							return 1;
						}
					else
					{
						OP_TIMEOUT++;
						return 0;
					}
				}
			else if (appearance==0)
				{
					nCS_SPI = 0;
					TX_SPI_BUFFER=Command;
					appearance++;
					return 0;
				}
			else
			{
				OP_TIMEOUT++;
				return 0;

			}
		}

		// Проверяет, разрешена ли запись, 2 = разрешена, 0 = BUSY, 1 = запрещена.
		short CheckWriteEnable()
		{
			Uint16 rdata;
			// Должен послать команду чтения, потом ещё дамми байт, чтобы получить статус
			if (appearance == 1) // если второй раз сюда зашли, читаем два байта
			{
				if (RX_FIFO_STATUS == 1) // если в FIFO упало значение
					{
						nCS_SPI = 1;
						rdata = RX_SPI_BUFFER;
						StatusResult = (rdata&0x00FF)<<8;
						if (StatusResult&0x200) // Смотрим что ответила флешка
							{
								appearance=0;
								return 2;
							}
							else
							{
								appearance=0;
								return 1;
							}
					}
				else // условие тайм-аута
				{
					OP_TIMEOUT++;
					return 0; // если буфер не заполнился, значит передача не завершилась - ждём
				}
			}
			else if (appearance == 0) // если первый раз сюда вошли
			{
				Uint16 Command = SetCommand(Com_ReadStatusRegister)|0x00FF;
				nCS_SPI = 0;
				TX_SPI_BUFFER=Command;
				appearance++;
				return 0;
			}
			else // на всякий случай - висяк
			{
				OP_TIMEOUT++;
				return 0;
			}
		}


// Делает анпротект сектора, в который попадает указанный адрес. Возвращает 1 = DONE. 0 = BUSY.
	bool Unprotect(Uint32 AddrOfBlock)
	{
		Uint32 BlockAddr = AddrOfBlock*2;
		Uint16 Command = SetCommand(Com_UnProtectSector);
		Uint16 Address1 = ((Uint16)((BlockAddr&0x00FF0000)>>16))&0x00FF|Command;
		Uint16 Address2 = (Uint16)((BlockAddr&0x0000FFFF));
		if (appearance==1)
			{
				if (RX_FIFO_STATUS == 2)
					{
						nCS_SPI = 1;
						RX_FIFO_RESET=0;
						RX_FIFO_RESET=1;
						appearance=0;
						return 1;
					}
				else // условие тайм-аута
					{
						OP_TIMEOUT++;
						return 0;
					}
			}
		else if (appearance==0)
			{
				nCS_SPI = 0;
				TX_SPI_BUFFER=Address1;
				TX_SPI_BUFFER=Address2;
				appearance++;
				return 0;
			}
		else
			{
				OP_TIMEOUT++;
				return 0;
			}

	}







// Проверяет, сделан ли анпротект, если да, то 2 = UNPROTECTED, 0 = BUSY, PROTECTED = 1.
	short CheckProtect(Uint32 AddrOfBlock)
	{
		Uint32 BlockAddr = AddrOfBlock*2;
		Uint16 Command = SetCommand(Com_ReadSectorProtectionRegisters);
		Uint16 rdata;
		Uint16 Address1 = ((Uint16)((BlockAddr&0x00FF0000)>>16))&0x00FF|Command;
		Uint16 Address2 = (Uint16)((BlockAddr&0x0000FFFF));

		if (appearance == 1) // если второй, считали буфер и потом послали дамми
		{
			if (RX_FIFO_STATUS == 4) // если в FIFO упало значение ТАЙМ АУТ НУЖЕН
				{
					rdata = RX_SPI_BUFFER;
					rdata = RX_SPI_BUFFER;
					rdata = RX_SPI_BUFFER;
					rdata = RX_SPI_BUFFER;
					nCS_SPI = 1;

					if (rdata!=0)
					{
						appearance=0;
						return 1;
					}
					else
					{
						appearance=0;
						return 2;
					}
				}
			else // условие тайм-аута
				{
					OP_TIMEOUT++;
					return 0;
				}
		}

		else if (appearance == 0) // если первый раз сюда вошли
		{

			nCS_SPI = 0;

			TX_SPI_BUFFER=Address1;
			TX_SPI_BUFFER=Address2;
			TX_SPI_BUFFER=DummyWord;
			TX_SPI_BUFFER=DummyWord;
			appearance++;
			return 0;
		}
		else
		{
			OP_TIMEOUT++;
			return 0;
		}

	}


// Удаляет блок данных. Продумать расчёт объёма. Возвращает 1 = DONE. 0 = BUSY.
	bool BlockErase(Uint32 AddrOfBlock, short SectorSize)
	{
		Uint16 Command;
		Uint32 BlockAddr=2*AddrOfBlock;
		switch (SectorSize)
				{
				case BLOCK_4K  : Command = SetCommand(Com_BlockErase4K); break;
				case BLOCK_32K : Command = SetCommand(Com_BlockErase32K); break;
				case BLOCK_64K : Command = SetCommand(Com_BlockErase64K); break;
				default: break;
				}

		Uint16 Address1 = ((Uint16)((BlockAddr&0x00FF0000)>>16))&0x00FF|Command;
		Uint16 Address2 = (Uint16)((BlockAddr&0x0000FFFF));


		if (appearance==1)
			{
				if (RX_FIFO_STATUS == 2)
					{
						nCS_SPI = 1;
						RX_FIFO_RESET=0;
						RX_FIFO_RESET=1;
						appearance=0;
						return 1;
					}
				else // условие тайм-аута
					{
						OP_TIMEOUT++;
						return 0;
					}
			}
		else if (appearance==0)
			{
				nCS_SPI = 0;
				TX_SPI_BUFFER=Address1;
				TX_SPI_BUFFER=Address2;
				SetBlockErased(AddrOfBlock);
				appearance++;
				return 0;
			}
		else
			{
				OP_TIMEOUT++;
				return 0;
			}

	}

// Проверяет успешность последней операции стирания и занятость флешки. Возвращает 2 = ERASED 1 = ERROR. 0 = BUSY.
	short CheckErrorReady()
	{
		Uint16 rdata;

		Uint16 test,test2;
		// Должен послать команду чтения, потом ещё дамми байт, чтобы получить статус

		if (appearance == 1) // если второй раз сюда зашли, читаем два байта
		{
			if (RX_FIFO_STATUS == 2) // если в FIFO упало значение
				{
					nCS_SPI = 1;
					rdata = RX_SPI_BUFFER;
					StatusResult = (rdata&0x00FF)<<8;
					rdata = RX_SPI_BUFFER;
					StatusResult = (StatusResult&0xFF00)|((rdata&0xFF00)>>8);
					// Смотрим что ответила флешка
					test=StatusResult&0x2000; // если занято, то перепроверить занято 0100
					test2=StatusResult&0x0100;
					if (test==0)
						{
							if (test2!=0) // если ещё не стёрло
							{
								appearance=0;
								return 0;
							}
							else //if (test2==0)
							{
								appearance=0;
								return 2;
							}
						}
						else // if (test!=0)
						{
							appearance=0;
							return 1;
						}
				}
			else // условие тайм-аута
				{
					OP_TIMEOUT++;
					return 0; // если буфер не заполнился, значит передача не завершилась - ждём
				}
		}
		else if (appearance == 0) // если первый раз сюда вошли
		{
			Uint16 Command = SetCommand(Com_ReadStatusRegister)|0x00FF;
			nCS_SPI = 0;
			RX_FIFO_RESET=0;
			RX_FIFO_RESET=1;
			TX_SPI_BUFFER=Command;
			TX_SPI_BUFFER=DummyWord;
			appearance++;
			return 0;
		}
		else // на всякий случай - висяк
			{
				OP_TIMEOUT++;
				return 0;
			}
	}


	//	Возвращает 1 = DONE, BUSY = 0.
		bool WriteWordOperation(Uint32 WordAdress, Uint16 Data, bool SingleDataFlag)
		{

			Uint32 Adress = WordAdress*2;
			Uint16 Command = SetCommand(Com_BytePageProgram);
			Uint16 Address1 = ((Uint16)((Adress&0x00FF0000)>>16))&0x00FF|Command;
			Uint16 Address2 = (Uint16)((Adress&0x0000FFFF));

			if (appearance==0)
			{
				if (TX_FIFO_STATUS!=3) //  not TX_FIFO_FULL на всякий случай проверяем очередь отправки. Мало ли глюк!
				{
					nCS_SPI = 0;
					TX_SPI_BUFFER=Address1;
					TX_SPI_BUFFER=Address2;
					TX_SPI_BUFFER=Data;
					appearance++;
					return 0;
				}
				else
				{
					OP_TIMEOUT++;
					return 0;
				}

			}
			else if (appearance==1)
			{
				if (RX_FIFO_STATUS == 3)
				{
					RX_FIFO_RESET=0;
					RX_FIFO_RESET=1;
					appearance=0;
					if (SingleDataFlag) // если это единственное слово, то вырубаем nCS и возвращаем DONE
					{
						nCS_SPI = 1;
					} // иначе просто пропускаем этот шаг
					return 1; // закончили
				}
				else // условие тайм-аута
					{
						OP_TIMEOUT++;
						return 0; 	// если буфер ещё не наполнился, то операция не завершена – ждём!
					}
			}
			else
				{
					OP_TIMEOUT++;
					return 0; // висяк на всякий случай
				}

		}

		//	Возвращает 1 = DONE, BUSY = 0.
		bool WriteBlockOperation(Uint16 Data, bool LastData)
		{

			if (not LastData) // если слово не последнее
			{
				RX_FIFO_RESET=0;
				RX_FIFO_RESET=1;
				if (TX_FIFO_STATUS!=3) // проверяем, не забито ли выходное фифо, если нет, то пишем и выдаём DONE
				{
					TX_SPI_BUFFER=Data;
					return 1;
				}
				else // если забито, то выдаём BUSY (декрементить длину тогда не надо)
				{
					OP_TIMEOUT++;
					return 0;
				}
			}
			else //if (LastDataFlag) // если надо записать последнее слово и закрыть
			{
				// дождаться пока опустошиться выходное фифо
				// опустошить входное фифо
				// кинуть на выход последнее слово
				// ждать, пока во входном фифо будет 1 слово
				// после этого вырубать ЦС
				if (appearance==1) // входим не первый раз
				{
					if (RX_FIFO_STATUS==1)
					{
						nCS_SPI=1;
						RX_FIFO_RESET=0;
						RX_FIFO_RESET=1;
						appearance=0;
						return 1;
					}
					else
					{
						OP_TIMEOUT++;
						return 0;
					}
				}
				else //if (appearance!=1) входим первый раз
				{
					if (TX_FIFO_STATUS==0) // TX FIFO EMPTY
					{
						RX_FIFO_RESET=0;
						RX_FIFO_RESET=1;
						TX_SPI_BUFFER=Data;
						appearance=1;
						return 0;
					}
					else
					{
						OP_TIMEOUT++;
						return 0;
					}
				}

			}
		}

// Читает слово по адресу указанному через InitReadWordFromFlash в ReadWordAdress. Возвращает прочитанное значение. Если "занята", то возвращает -1.
		Int32 ReadWordOperation(Uint32 WordAdress, bool LastData)
		{
			Uint16 rdata;
			Uint32 Adress = WordAdress*2;
			Int32 result = 0;


			Uint16 Command = SetCommand(Com_ReadArray);
			Uint16 Address1 = ((Uint16)((Adress&0x00FF0000)>>16))&0x00FF|Command;
			Uint16 Address2 =  (Uint16)((Adress&0x0000FFFF));

			if (appearance==1) // второй заход
			{
				if (RX_FIFO_STATUS == 3) // если в фифо упали 3 значения
				{
					if (LastData)
					{
						nCS_SPI = 1;
					}
					rdata = RX_SPI_BUFFER;
					rdata = RX_SPI_BUFFER;
					rdata = RX_SPI_BUFFER;
					appearance = 0;
					result = 0;
					return (result|(0x0000FFFF&(Uint32)rdata));
				}
				else
				{
					OP_TIMEOUT++;
					return (-1);
				}
			}
			else if (appearance==0) // первый заход
			{
				nCS_SPI = 0;
				TX_SPI_BUFFER=Address1;
				TX_SPI_BUFFER=Address2;
				TX_SPI_BUFFER=DummyWord;
				appearance++;
				return (-1);
			}
			else
			{
				OP_TIMEOUT++;
				return (-1);
			}


		}

// Читает следующее слово в потоковом режиме. Возвращает прочитанное значение. Если поднят флаг - поднимает CS. Если "занята", то возвращает -1.
		Int32 ReadBlockOperation(bool LastData)
		{
			Uint16 rdata;
			Int32 result = 0;
			if (appearance==1)
			{
				if (RX_FIFO_STATUS == 1)
				{
					if (LastData)
					{
						nCS_SPI = 0;
					}
					rdata = RX_SPI_BUFFER;
					appearance = 0;
					result = 0;
					return (result|(0x0000FFFF&(Uint32)rdata));
				}
				else
				{
					OP_TIMEOUT++;
					return (-1);
				}
			}
			else if (appearance==0)
			{
				TX_SPI_BUFFER=DummyWord;
				appearance++;
				return (-1);
			}
			else
			{
				OP_TIMEOUT++;
				return (-1);
			}
		}


//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//											READ WORD										//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

	void ReadWordStep()
	{
		short temp;
		//Int32 temp2;
		switch(NextStep)
				{
				case 0: 	temp=CheckErrorReady(); 																		// STEP 1. Проверка состояния флешки.
							if (temp==2)
							{
								NextStep++;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
							else if (temp==1)
							{
								OP_TIMEOUT=0;
								if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
								{
									ERRORCODE=RETRY_TIMEOUTERROR;
								}
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;
				case 1: 	temp2=ReadWordOperation(ReadWordAdress, isSingleData);
							if (temp2!=-1)
							{
								ReadResult=temp2;
								NextStep++;
							}
				break; 											// STEP 2. Считываем значение
				case 2: 	CurrentOperation=OperationFinished; break;														// STEP 3. FINISH
				}
	}

// Записывает значение Value во внутреннюю память по адресу DestinationAddress
	void WriteToPCON(Uint16 Value)
	{
		//short *XMEM_pw;
		//XMEM_pw = (short *)(DestinationAddress);
		//*XMEM_pw  =  Value;
		*DestinationAddress = Value;
	}

//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//											READ BLOCK										//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////


	void ReadBlockStep()
		{
			short temp;
			//Int32 temp2;
			switch(NextStep)
					{
					case 0: 	temp=CheckErrorReady(); 	// STEP 1. Проверка состояния флешки.
								if (temp==2)
								{
									NextStep++;
									appearance=0;
									OP_TIMEOUT=0;
									RETRY_TIMEOUT=0;
								}
								else if (temp==1)
								{
									OP_TIMEOUT=0;
									if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
									{
										ERRORCODE=RETRY_TIMEOUTERROR;
									}
								}
								else if(OP_TIMEOUT>OP_MAXTIMEOUT)
								{
									ERRORCODE=OP_TIMEOUTERROR;
								}
					break;

					case 1: 	while (not DEBUGVAR){}
								temp2=ReadWordOperation(ReadBlockAdress, notSingleData);	// STEP 2. Считываем значение
								if(temp2!=-1)
								{
									WriteToPCON(temp2);
									LengthOfBlock--;
									DestinationAddress++;
									LastDataFlag=0;
									NextStep++;
								}
					break;

					case 2:		while (not DEBUGVAR){}
								if (LengthOfBlock==1) // если осталось считать одно значение
								{
									LastDataFlag=1;
								}
								else if (LengthOfBlock==0) // если все значения считаны
								{
									NextStep++;
									break;
								}
								temp2=ReadBlockOperation(LastDataFlag);

								if (temp2!=-1)
								{
									WriteToPCON(temp2);
									LengthOfBlock--;
									DestinationAddress++;
								}

					break;

					case 3: 	CurrentOperation=OperationFinished; break;														// STEP 3. FINISH
					}
		}


//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//											WRITE WORD										//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

	void WriteWordStep()
	{
		short temp;

		switch(NextStep) // ВЕЗДЕ ГДЕ ПЕРЕХОД ШАГА – сбрасывать appearance
		{
		case 0: 	if (WriteEnable())						// STEP 1. Write enable
					{
						NextStep++;
						appearance=0;
						OP_TIMEOUT=0;
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 1: 	temp=CheckWriteEnable(); 				// STEP 2. Check write enable

					if (temp==2)
					{
						NextStep++;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if (temp==1)
					{
						NextStep--;
						appearance=0;
						OP_TIMEOUT=0;
						if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
						{
							ERRORCODE=RETRY_TIMEOUTERROR;
						}
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;


		case 2: 	if (Unprotect(WriteWordAdress)) 		// STEP 3. Unprotect sector
					{
						NextStep++;
						appearance=0;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 3: 	temp=CheckProtect(WriteWordAdress);		// STEP 4. Check protect ERROR
					if (temp==2)
					{
						NextStep++;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if (temp==1)
					{
						NextStep--;
						OP_TIMEOUT=0;
						if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
						{
							ERRORCODE=RETRY_TIMEOUTERROR;
						}
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 4: 	if (WriteEnable())						// STEP 5. Write enable
					{
						NextStep++;
						appearance=0;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 5: 	temp=CheckWriteEnable(); 				// STEP 6. Check write enable
					if (temp==2)
					{
						NextStep++;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if (temp==1)
					{
						NextStep--;
						OP_TIMEOUT=0;
						if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
						{
							ERRORCODE=RETRY_TIMEOUTERROR;
						}
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 6:
			        if (GetIfBlockErased(WriteWordAdress)==0)
			        {
						if (BlockErase(WriteWordAdress, BLOCK_4K))		// STEP 7. Block Erase
						{
							NextStep++;
							appearance=0;
							OP_TIMEOUT=0;
							RETRY_TIMEOUT=0;
						}
						else if(OP_TIMEOUT>OP_MAXTIMEOUT)
						{
							ERRORCODE=OP_TIMEOUTERROR;
						}
			        }
			        else
			        	{
			        		NextStep++;
							appearance=0;
							OP_TIMEOUT=0;
							RETRY_TIMEOUT=0;
			        	}
		break;

		case 7: 	temp=CheckErrorReady(); 				// STEP 8. Check Block Erase
					if (temp==2)
					{
						NextStep++;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if (temp==1)
					{
						NextStep--;
						OP_TIMEOUT=0;
						if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
						{
							ERRORCODE=RETRY_TIMEOUTERROR;
						}
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 8: 	if (WriteEnable())						// STEP 9. Write enable
					{
						NextStep++;
						appearance=0;
						OP_TIMEOUT=0;
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 9: 	temp=CheckWriteEnable(); 				// STEP 10. Check write enable
					if (temp==2)
					{
						NextStep++;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if (temp==1)
					{
						NextStep--;
						OP_TIMEOUT=0;
						if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
						{
							ERRORCODE=RETRY_TIMEOUTERROR;
						}
					}
					else if (OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 10: 	if ( WriteWordOperation(WriteWordAdress, WriteWordData, isSingleData) ) 		// STEP 11. Write the word
					{
						NextStep++;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if (OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 11: 	temp=CheckErrorReady(); 				// STEP 12. Check Program Error/Finished
					if (temp==2)
					{
						NextStep++;
						OP_TIMEOUT=0;
						RETRY_TIMEOUT=0;
					}
					else if (temp==1)
					{
						NextStep--;
						OP_TIMEOUT=0;
						if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
						{
							ERRORCODE=RETRY_TIMEOUTERROR;
						}
					}
					else if(OP_TIMEOUT>OP_MAXTIMEOUT)
					{
						ERRORCODE=OP_TIMEOUTERROR;
					}
		break;

		case 12:	CurrentOperation=OperationFinished;							// STEP 11. FINISH
		break;

		default : break;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//											WRITE BLOCK										//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////


	void WriteBlockStep()
	{

		short temp;
		short BlockToErase;

				switch(NextStep) // ВЕЗДЕ ГДЕ ПЕРЕХОД ШАГА – сбрасывать appearance
				{
				case 0: 	if (WriteEnable())						// STEP 1. Write enable
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 1: 	temp=CheckWriteEnable(); 				// STEP 2. Check write enable

							if (temp==2)
							{
								NextStep++;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
							else if (temp==1)
							{
								NextStep--;
								appearance=0;
								OP_TIMEOUT=0;
								if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
								{
									ERRORCODE=RETRY_TIMEOUTERROR;
								}
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;


				case 2: 	if (Unprotect(WriteWordAdress)) 		// STEP 3. Unprotect sector
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 3: 	temp=CheckProtect(WriteWordAdress);		// STEP 4. Check protect ERROR
							if (temp==2)
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
							else if (temp==1)
							{
								NextStep--;
								appearance=0;
								OP_TIMEOUT=0;
								if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
								{
									ERRORCODE=RETRY_TIMEOUTERROR;
								}
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 4: 	if (WriteEnable())						// STEP 5. Write enable
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 5: 	temp=CheckWriteEnable(); 				// STEP 6. Check write enable
							if (temp==2)
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
							else if (temp==1)
							{
								NextStep--;
								appearance=0;
								OP_TIMEOUT=0;
								if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
								{
									ERRORCODE=RETRY_TIMEOUTERROR;
								}
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

																																		// STEP 7. Block Erase
				case 6:

							if (appearance==0)
							{
								if (RemainingPagesInBlock==0) // если записали весь стёртый блок
								{
									if ((AmountOf64KBBlocks>0)&&(WriteWordAdress%32768==0))
									{
										BlockToErase=BLOCK_64K;
										AmountOf64KBBlocks--;
										RemainingPagesInBlock=256;
									}
									else if ((AmountOf32KBBlocks>0)&&(WriteWordAdress%16384==0))
									{
										BlockToErase=BLOCK_32K;
										AmountOf32KBBlocks--;
										RemainingPagesInBlock=128;
									}
									else if (AmountOf4KBBlocks>0)
									{
										BlockToErase=BLOCK_4K;
										AmountOf4KBBlocks--;
										RemainingPagesInBlock=16;
									}
									else
									{
										BlockToErase=BLOCK_4K;
									}

									SkipErase=0;

								}
								else
								{
									SkipErase=1;
								}

							}

							if (SkipErase==0)
							{

								if (BlockErase(WriteWordAdress, BlockToErase))
								{
									NextStep++;
									appearance=0;
									OP_TIMEOUT=0;
									RETRY_TIMEOUT=0;
								}
								else if(OP_TIMEOUT>OP_MAXTIMEOUT)
								{
									ERRORCODE=OP_TIMEOUTERROR;
								}
							}
							else
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
				break;

				case 7: 	temp=CheckErrorReady(); 				// STEP 8. Check Block Erase
							if (temp==2)
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;
							}
							else if (temp==1)
							{
								NextStep--;
								OP_TIMEOUT=0;
								if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
								{
									ERRORCODE=RETRY_TIMEOUTERROR;
								}
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 8: 	if (WriteEnable())						// STEP 9. Write enable
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 9: 	temp=CheckWriteEnable(); 				// STEP 10. Check write enable
							if (temp==2)
							{
								NextStep++;
								OP_TIMEOUT=0;
								appearance=0;
								RETRY_TIMEOUT=0;
							}
							else if (temp==1)
							{
								NextStep--;
								OP_TIMEOUT=0;
								if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
								{
									ERRORCODE=RETRY_TIMEOUTERROR;
								}
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 10:
							if (WriteWordOperation(WriteWordAdress,WriteWordData, SingleWordFlag)) 		// STEP 11. Write the FIRST word
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;

								if (LengthOfBlock>127)
								{
									LengthOfCurrentPage=128;
									LengthOfBlock -= 128; // было 127
								}
								else
								{
									LengthOfCurrentPage=LengthOfBlock;
									LengthOfBlock=0;
								}
								LengthOfCurrentPage--;


							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}
				break;

				case 11:
							if (LengthOfCurrentPage==1) // если осталась одна запись, то переходим к закрытию блока
							{
								LastDataFlag=1;

							}
							else if (LengthOfCurrentPage==0) // если страницу записали и закрыли
							{
								if (LengthOfBlock>1) // если в следующей странице только одна запись
								{
									SingleWordFlag=notSingleData;
								}
								else
								{
									SingleWordFlag=isSingleData;
								}

								NextStep++;
								appearance=0;
								AmountOfPages--;
								RemainingPagesInBlock--;
								break;
							}
								GetNewWord();





							if (WriteBlockOperation(WriteWordData, LastDataFlag))
							{
								LengthOfCurrentPage--;
							}

				break;

				case 12: 	temp=CheckErrorReady(); 				// STEP 13. Check Program Error/Finished
							if (temp==2)
							{
								NextStep++;
								appearance=0;
								OP_TIMEOUT=0;
								RETRY_TIMEOUT=0;

							}
							else if (temp==1)
							{

								OP_TIMEOUT=0;
								if (RETRY_TIMEOUT++>RETRY_MAXTIMEOUT)
								{
									ERRORCODE=RETRY_TIMEOUTERROR;
								}
							}
							else if(OP_TIMEOUT>OP_MAXTIMEOUT)
							{
								ERRORCODE=OP_TIMEOUTERROR;
							}


				break;

				case 13:
							if (AmountOfPages>0) // если остались ещё страницы к записи
							{
								WriteWordAdress += 128;
								NextStep=4;
								appearance=0;
								LastDataFlag=0;
							}
							else
							{
								NextStep++;
							}



				break;


				case 14:	CurrentOperation=OperationFinished;							// STEP 15. FINISH
				break;

				default : break;
				}
	}

// Чтение слова из SRAM
	void GetNewWord()
	{
	//	WriteWordData=ReadWordFrom_SRAM(SourceAddress);
	//	SourceAddress++;
				WriteWordData=WriteWordData+7;
	}

public:


	SPIFlashReader()
			{}


	void SetRewriteFlash(void)
	{
		int i=0;
		for (i=0;i<64;i++)
		{
			Block4KErased[i]=0;
		}
	}


// Инициализации класса
	void SPIFlashReaderInit(short CycleTime_uS)
	{
		nCS_SPI=1;
		ReadResult = 0;
		CurrentOperation = OperationFinished;
		appearance=0;
		NextStep=0;
		OP_TIMEOUT=0;
		RETRY_TIMEOUT=0;
		ERRORCODE=0;
		StepTime = CycleTime_uS;
		DummyWord = 0xFFFF;
		SkipErase=0;
		LastDataFlag=0;
		RX_FIFO_RESET=0;
		RX_FIFO_RESET=1;
	}

// Функция шага. Возвращает 1, когда инициированная операция закончена, иначе возвращает 0. ERROR = -1;
	short SPIFlashStep()
	{
		if (ERRORCODE!=0) // если возникла ошибка
		{
			CurrentOperation = ErrorOccured;
		}

		if (StepTime==0)
		{
			CurrentOperation = ErrorOccured;
			ERRORCODE = CYCLETIMEUNDEFINED;
		}

		switch(CurrentOperation) // Переход к выбранной операции ( сначала инит потом всё. а по дефолту и по нулю - ничего не делать)
		{

			case ReadWord : ReadWordStep() ; return 0; break;
			case ReadBlock : ReadBlockStep() ; return 0; break;

			case WriteWord : WriteWordStep() ; return 0; break;
			case WriteBlock : WriteBlockStep() ; return 0; break;

			case OperationFinished : return 1;  break; // подумать чем отличить его и надо ли

			case ErrorOccured : return (-1); break;

			default : return 1; break;

		}
	}



// Инициализация записи слова. Параметры: адрес для записи, данные.
	void InitWriteWordToFlash(Uint32 WordAdress, Uint16 Data)
	{
		if (StepTime!=0)
		{
			SPIFlashReaderInit(StepTime);
		}
		else
		{
			ERRORCODE=CYCLETIMEUNDEFINED;
		}
		CurrentOperation=WriteWord;
		WriteWordAdress=WordAdress;
		WriteWordData=Data;
	}

	// Инициализация записи блока. Параметры: порядковый номер 4Кб блока, длина записываемого блока, начальный адрес записываемых данных.
		void InitWriteBlockToFlash(short BlockNumber, Uint32 LengthInWords, long StartSourceAddress )
		{
			if (StepTime!=0)
			{
				SPIFlashReaderInit(StepTime);
			}
			else
			{
				ERRORCODE=CYCLETIMEUNDEFINED;
			}
			CurrentOperation=WriteBlock;
			WriteBlockNumber=BlockNumber;
			LengthOfBlock=LengthInWords+1;
			SourceAddress=StartSourceAddress;

			AmountOfPages=0;
			RemainingPagesInBlock=0;
			AmountOf4KBBlocks=0;
			AmountOf64KBBlocks=0;

			WriteWordAdress=BlockNumber*128*16; // пока для частичного тест а

			if (WriteWordAdress+LengthInWords>0x3FFFFF/2)
			{
				CurrentOperation = ErrorOccured; // ОШИБКА - ЗАДАННЫЙ ДИАПАЗОН ВЫХОДИТ ЗА ПРЕДЕЛЫ ПАМЯТИ
				ERRORCODE = FLASHOVERLOADERROR;
			}


			if (LengthInWords==0)
			{
				CurrentOperation = ErrorOccured; // ОШИБКА - ЗАДАНА НУЛЕВАЯ ДЛИНА БЛОКА
				ERRORCODE = ZEROWRITEBLOCKERROR;
			}

				AmountOfPages = LengthOfBlock / PageSizeInWords + (int)((LengthOfBlock % PageSizeInWords)>0);


				AmountOf4KBBlocks = AmountOfPages / PagesInBlock + (int)((AmountOfPages % PagesInBlock)>0);

				if (AmountOf4KBBlocks>Blocks4InBlocks32) // 32/4=8
				{
					AmountOf32KBBlocks = AmountOf4KBBlocks / Blocks4InBlocks32 + (int)((AmountOf4KBBlocks % Blocks4InBlocks32)>0);

					if (BlockNumber % BytesIn32KB != 0) // попадает ли 4КБ Блок в начало 32КБ блока
					{
						AmountOf32KBBlocks--; // если нет, то значит на 1 32КБ блок у нас меньше
					}// иначе всё в порядке


					AmountOf4KBBlocks -= AmountOf32KBBlocks*8;
					AmountOf4KBBlocks = (AmountOf4KBBlocks>0) ? AmountOf4KBBlocks : 0;


					if (AmountOf32KBBlocks>2) // 64/32=2
					{
						AmountOf64KBBlocks = AmountOf32KBBlocks / 2 + (int)((AmountOf32KBBlocks % 2)>0);


						if (BlockNumber % BytesIn64KB != 0) // попадает ли 4КБ Блок в начало 64КБ блока при наличии расчётных 64КБ блоков
						{
							AmountOf32KBBlocks--; // если нет, то значит на 1 64КБ блок у нас меньше
						}

						AmountOf32KBBlocks -= AmountOf64KBBlocks*2;
						AmountOf32KBBlocks = (AmountOf32KBBlocks>0) ? AmountOf32KBBlocks : 0;
					}
				}
		}



// Инициализация чтения слова - результат во внутреннюю переменную, которую можно считать через GetReadValue().
		void InitReadWordFromFlash(Uint32 WordAdress)
		{
			if (StepTime!=0)
			{
				SPIFlashReaderInit(StepTime);
			}
			else
			{
				ERRORCODE=CYCLETIMEUNDEFINED;
			}
			CurrentOperation=ReadWord;
			ReadWordAdress=WordAdress;
		}


// Инициализация чтения блока - результат в указанный диапазон адресов (StartDestinationAddress)
		void InitReadBlockFromFlash(Uint32 WordAdress, Uint32 LengthInWords, Uint16 *StartDestinationAddress)
		{
			if (StepTime!=0)
			{
				SPIFlashReaderInit(StepTime);
			}
			else
			{
				ERRORCODE=CYCLETIMEUNDEFINED;
			}
			CurrentOperation=ReadBlock;
			ReadBlockAdress=WordAdress;
			DestinationAddress=StartDestinationAddress;
			LengthOfBlock=LengthInWords;
		}

// Возвращает прочтённое до этого значение.
		Uint16 GetReadValue()
		{
			return ReadResult;
		}

// Возвращает код ошибки. 0 - отсутствие оной.
		Uint16 GetErrorCode()
		{
			return ERRORCODE;
		}


};
#endif /* SPIFLASHFUNCTIONS_H_ */
