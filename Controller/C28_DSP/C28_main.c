#include "F28M35x_Device.h"
#include "F28M35x_Examples.h"

#include <string.h>
#include <stdbool.h>

#include "error_handling.h"
#include "communication_M3.h"
#include "communication_FPGA.h"
#include "flash.h"

#define C28_FREQ			100		// C28 работает на 100 мГц
#define MAIN_CYCLE_US		300		// Главный цикл С28 - 300 мкс
#define CPU_FREQ     		100E6	// 100MHz

// Currently we are using Flash memory to store the code
inline void setup_flash_mem(void) {
    #ifdef _FLASH
        // Copy the ramfuncs from Flash to RAM
        memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

        // Initialize the Flash controller
        // This should be in RAM while executing otherwise flash is fried
        InitFlash();
    #endif
}

// Original code is in InitFunctions.c
void setup_GPIO(void) {
    EALLOW;
    // Sets GPIO128 (pin 140 not connected) and GPIO19 (pin 29 = EPWM6A) to output
    GpioG2CtrlRegs.GPEDIR.bit.GPIO128 = 1;
    GpioG1CtrlRegs.GPADIR.bit.GPIO19 = 1;
    EDIS;
}

/*
 НЕ РАБОТАЕТ????
void setup_DMA_FPGA() {
	Uint16 size = sizeof(FPGA_MSGRAM_LOCAL)/sizeof(FPGA_MSGRAM_LOCAL[0]);
	DMACH1AddrConfig(FPGA_MSGRAM_LOCAL, FPGA_MSGRAM);

	DMACH1BurstConfig(0, 0, 0);
	DMACH1TransferConfig(size - 1, 1, 1);
	DMACH1WrapConfig(size - 1, 0, size - 1, 0);

	// This is a static copy use one shot mode, so only one trigger is needed
	// Use 16-bit mode
	// Enable the PIE interrupt for the DMA channel
	DMACH1ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,
					 SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,
					 CHINT_END, CHINT_ENABLE);

	StartDMACH1();
}
*/

void setup_timers(unsigned short MainCycleTimer_uS, unsigned short CPUfreq_value_MHZ) {
	// COPY PASTE

	//Set up Timer 1 for main cycle triggering
	CpuTimer1Regs.TCR.bit.TSS = 1; // Stop timer
	CpuTimer1Regs.TCR.bit.TIE = 0; // Disable timer interrupt

	CpuTimer1Regs.TPRH.bit.TDDRH = 0;
	CpuTimer1Regs.TPR.bit.TDDR = CPUfreq_value_MHZ - 1; //Prescale to 1uS tick for counter
	CpuTimer1Regs.PRD.all = MainCycleTimer_uS - 1; //MainCycleTimer_uS time for main cycle in microseconds
	CpuTimer1Regs.TCR.bit.FREE = 1; //Set timer debug mode to freerun
}

void start_timers(void) {
	// COPY PASTE

	CpuTimer1Regs.TCR.bit.TRB = 1; //Reload timer
	CpuTimer1Regs.TCR.bit.TIF = 1; //Reset interrupt flag
	CpuTimer1Regs.TCR.bit.TIE = 1; //Enable timer interrupt
	CpuTimer1Regs.TCR.bit.TSS = 0; //Ensure timer start
}

void main(void) {
	GpioG1DataRegs.GPADAT.bit.GPIO0 = 0; 	 //Remove system OK flag to FPGA

    // Disable all interrupts while configuring
    DINT;

    setup_flash_mem();

    InitSysCtrl();      // wakes up the CPU and the clocks
    InitGpio();

    setup_GPIO();       // setup GPIO pins

    // Configuring PIE (Peripheral Interrupt Expansion)
    // Инициализация системы прерываний
	InitPieCtrl(); 		// Initialize the PIE control registers to their default state.
	IER = 0x0000; 		// Disable CPU interrupts and clear all CPU interrupt flags
	IFR = 0x0000; 		// Disable CPU interrupts and clear all CPU interrupt flags
	InitPieVectTable(); // Initialize the PIE vector table with pointers to the shell ISR.

	flash_spi_setup();

	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM

	//ErrorSet(ERR_EMERGENCY_STOP_ALG);  Исходно стартуем с прерывания, чтобы безопасно все было, когда систему ошибок поправим, вернем

	// Запуска таймера на 300 мкс для главного цикла
	setup_timers(MAIN_CYCLE_US, C28_FREQ);
	start_timers();

	// Проверка запуска М3
	CtoMIpcRegs.CTOMIPCSET.bit.IPC1 = 1; 		   // Send data ready signal
	// IVAN: we set it in M3 init
	// LOOK IN README.TXT (9)
	while (CtoMIpcRegs.CTOMIPCFLG.bit.IPC1 != 0);  // Wait for M3 to read init data

	// Настройка DMA
	// FPGA_MSGRAM - память на FPGA, FPGA_MSGRAM_LOCAL - локальная копия
	//setup_DMA_FPGA();

	DataToM3 Data;
	Uint16 FreeTimeCounter = 0;
	Uint64 CycleCounter = 0;
	Uint16 LedCounter = 0;
	Uint8  WatchDog = 0;

	enum {
		FLASH_ST_RESET = 0,
		FLASH_ST_G_UNPROT_WE,
		FLASH_ST_G_UNPROT,
		FLASH_ST_READ,
		FLASH_ST_ERASE_WE,
		FLASH_ST_ERASE,
		FLASH_ST_WRITE_WE,
		FLASH_ST_WRITE,
		FLASH_ST_POLL_STATUS,
		FLASH_ST_CHECK_STATUS,
		FLASH_ST_DONE,
	} flash_st = FLASH_ST_G_UNPROT_WE;

	enum {
		DBG_ST_RESET,
		DBG_ST_UNPROT,
		DBG_ST_STATUS,
		DBG_ST_READ_1,
		DBG_ST_ERASE,
		DBG_ST_WRITE,
		DBG_ST_READ_2,
		DBG_ST_DONE,
	} dbg_st = DBG_ST_RESET;

	for(;;) {  // Итерации раз в 300 мкс
		static FlashStatusRegister flash_status_register = {0};
		switch (CTOM_DATA->FlashData.Cmd) {
			case FLASH_CMD_DONE: {
				// start doing next command if there's any
//				if (CTOM_DATA->FlashData.CmdIdx != MTOC_DATA->FlashData.CmdIdx) {
//					CTOM_DATA->FlashData.Cmd = MTOC_DATA->FlashData.Cmd;
//					CTOM_DATA->FlashData.CmdIdx = MTOC_DATA->FlashData.CmdIdx;
//				}

				// TEST
				switch (dbg_st) {
					case DBG_ST_RESET: {
						CTOM_DATA->FlashData.Cmd = FLASH_CMD_BUSY;
						flash_st = FLASH_ST_RESET;
						dbg_st++;
					} break;
					case DBG_ST_UNPROT: {
						CTOM_DATA->FlashData.Cmd = FLASH_CMD_BUSY;
						flash_st = FLASH_ST_G_UNPROT_WE;
						dbg_st++;
					} break;
					case DBG_ST_STATUS: {
						CTOM_DATA->FlashData.Cmd = FLASH_CMD_BUSY;
						flash_st = FLASH_ST_POLL_STATUS;
						dbg_st++;
					} break;
					case DBG_ST_READ_1:
					case DBG_ST_READ_2: {
						CTOM_DATA->FlashData.Cmd = FLASH_CMD_READ;
						CTOM_DATA->FlashData.Address = 0;
						CTOM_DATA->FlashData.DataSize = 100;
						memset((void *)CTOM_DATA->FlashData.Buf, 0, CTOM_DATA->FlashData.DataSize);
						dbg_st++;
					} break;
					case DBG_ST_ERASE: {
						CTOM_DATA->FlashData.Cmd = FLASH_CMD_ERASE_4K;
						CTOM_DATA->FlashData.Address = 0;
						dbg_st++;
					} break;
					case DBG_ST_WRITE: {
						CTOM_DATA->FlashData.Cmd = FLASH_CMD_WRITE;
						CTOM_DATA->FlashData.Address = 1;
						CTOM_DATA->FlashData.DataSize = 67;
						memset((void *)CTOM_DATA->FlashData.Buf, 123, CTOM_DATA->FlashData.DataSize);
						dbg_st++;
					} break;
					default: break;
				}
			} break;
			case FLASH_CMD_BUSY: {
				if (!flash_is_ready()) {
					break;
				}
				switch (flash_st) {
					case FLASH_ST_RESET: {
						flash_reset();
						flash_st = FLASH_ST_POLL_STATUS;
					} break;
					case FLASH_ST_G_UNPROT: {
						flash_write_status_fst(0, 0, 1);
						flash_st++;
					} break;
					case FLASH_ST_READ: {
						flash_read_array(
							CTOM_DATA->FlashData.Buf,
							CTOM_DATA->FlashData.DataSize,
							CTOM_DATA->FlashData.Address
						);
						flash_st = FLASH_ST_POLL_STATUS;
					} break;
					case FLASH_ST_G_UNPROT_WE:
					case FLASH_ST_ERASE_WE:
					case FLASH_ST_WRITE_WE: {
						flash_write_enable();
						flash_st++;
					} break;
					case FLASH_ST_ERASE: {
						flash_block_erase_4K(CTOM_DATA->FlashData.Address);
						flash_st = FLASH_ST_POLL_STATUS;
					} break;
					case FLASH_ST_WRITE: {
						flash_write_array(
//							MTOC_DATA->FlashData.Buf,
//							MTOC_DATA->FlashData.DataSize,
//							MTOC_DATA->FlashData.Address
							CTOM_DATA->FlashData.Buf,
							CTOM_DATA->FlashData.DataSize,
							CTOM_DATA->FlashData.Address
						);
						flash_st++;
					} break;
					case FLASH_ST_POLL_STATUS: {
						flash_read_status(&flash_status_register);
						flash_st = FLASH_ST_CHECK_STATUS;
					} break;
					case FLASH_ST_CHECK_STATUS: {
						// wait for ongoing operation to complete
						if (flash_status_register.snd.RDY_BSY == 0) {
							CTOM_DATA->FlashData.Cmd = FLASH_CMD_DONE;
							flash_st = FLASH_ST_DONE;
						}
					} break;
					default: break;
				}
			} break;
			case FLASH_CMD_READ: {
				CTOM_DATA->FlashData.Cmd = FLASH_CMD_BUSY;
				flash_st = FLASH_ST_READ;
			} break;
			case FLASH_CMD_WRITE: {
				CTOM_DATA->FlashData.Cmd = FLASH_CMD_BUSY;
				flash_st = FLASH_ST_WRITE_WE;
			} break;
			case FLASH_CMD_ERASE_4K: {
				CTOM_DATA->FlashData.Cmd = FLASH_CMD_BUSY;
				flash_st = FLASH_ST_ERASE_WE;
			} break;
			default: break;
		}

		if (DmaRegs.CH1.CONTROL.bit.TRANSFERSTS) {
			continue;  // DMA занят, ждем
		}

		// Моргаем светодиодиком
		LedCounter++;
		if (LedCounter == 1000) {
			GpioG1DataRegs.GPADAT.bit.GPIO8 = 0;
		}
		else if (LedCounter > 2000) {
			GpioG1DataRegs.GPADAT.bit.GPIO8 = 1;
			LedCounter = 0;
		}

		Data.CycleCounter = CycleCounter;
		Data.FreeTimeCounter = FreeTimeCounter;

		ReadFPGAData(&Data);
		CheckFPGAConnect(Data, &WatchDog);

		// TODO: Считывание сигналов с M3: (bool16 ?)
		Bool reset_errors = false;
		Bool converter_on = false;
		Bool mode_up = false;
		Uint16 PWM_Counter = 0; // TODO: Расчет pwm:
		WriteFPGAData(PWM_Counter, mode_up, converter_on, reset_errors);

		Data.C28_Errors = ErrorGetCurrent();
		Data.C28_Errors_Latch = ErrorGetLatch();
		WriteToM3Data(Data);  // Отправляем замер на М3

		if (reset_errors) {
			reset_errors = 0;
			// 300 мкс должно хватить на передачу сигнала до ADCHub и обратно
			ErrorResetAll();
		}

		EALLOW;
		DmaRegs.CH1.CONTROL.bit.PERINTFRC = 1;  // DMA запуск получения значений
		EDIS;

		bool   CPU_OverloadFlag = true;
		Uint16 WaitCyclesCounter = 0;
		// Ждем до 300 мкс (одна итерация 300 мкс)
		while (CpuTimer1Regs.TCR.bit.TIF == 0) {
			FreeTimeCounter = CpuTimer1Regs.TIM.all;
			CPU_OverloadFlag = false;
			WaitCyclesCounter++;
			if (WaitCyclesCounter > 30000) {
				ErrorSet(ERROR_MAIN_TIMER_DEAD);
				break;
			}
		}

		if (CPU_OverloadFlag != 0) {
			ErrorSet(ERROR_CPU_OVERLOAD);
		} else {
			ErrorReset(ERROR_CPU_OVERLOAD);
		}

		CpuTimer1Regs.TCR.bit.TIF = 1; // Перезапуск таймера

		CycleCounter++;
	}
}
