#include "F28M35x_Device.h"
#include "F28M35x_Examples.h"
#include "F28M35x_Ipc.h"

#include <string.h>
#include <stdbool.h>

#include "error_handling.h"
#include "communication_M3.h"
#include "communication_FPGA.h"

#define C28_FREQ			100  // C28 работает на 100 мГц
#define MAIN_CYCLE_US		300  // Главный цикл С28 - 300 мкс
#define CPU_FREQ     100E6      // 100MHz

// Shared memory buffer
// telling the compiler to put this variable into the specific section
// this only takes section 0 of 0-7 sections
// look into .cmd file to see the layout
#pragma DATA_SECTION(shared_mem, "SHARERAMS0")
volatile Uint16 shared_mem[0x1000];


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

void setup_SPI(void) {
	// COPY PASTE

#define SPI_BAUDRATE 1000000     // 1000KHz
#define LSPCLK_FREQ  (CPU_FREQ/4)
#define SPI_BRR      (LSPCLK_FREQ/SPI_BAUDRATE)-1

	//GpioG1DataRegs.GPADAT.bit.GPIO19 = 1;

		// Initialize SPI FIFO registers
		// SPI reset
		// Reset the FIFO pointer and hold in reset
		//
		    SpiaRegs.SPIFFTX.bit.SPIRST = 0;
		    SpiaRegs.SPIFFTX.bit.TXFIFO = 0;

		// Set TX FIFO to 4 words
		// Enable TX FIFOs
		// Enable TX FIFO interrupts
		// Release FIFO from reset
		    SpiaRegs.SPIFFTX.bit.TXFFIL = 6; // в примере 4 у далера 6
		    SpiaRegs.SPIFFTX.bit.SPIFFENA = 1;
		    SpiaRegs.SPIFFTX.bit.TXFFIENA = 1;
		    SpiaRegs.SPIFFTX.bit.TXFIFO = 1;

		//
		// Put RX FIFO in reset
		// Set RX FIFO to 4 words
		// Enable RX FIFO interrupts
		// Release RX FIFO from reset
		    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
		    SpiaRegs.SPIFFRX.bit.RXFFIL = 6; // в примере 4 у далера 6
		    SpiaRegs.SPIFFRX.bit.RXFFINT = 1;
		    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;

		// Transmit delay is zero
		// Release SPI from reset
		    SpiaRegs.SPIFFCT.all=0x0;
		    SpiaRegs.SPIFFTX.bit.SPIRST = 1;

	    // Set reset low before configuration changes
	    // Clock polarity (0 == rising, 1 == falling)
	    // 16-bit character
	    // Enable loop-back
	    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
	    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
	    SpiaRegs.SPICCR.bit.SPICHAR = (16-1);
	    SpiaRegs.SPICCR.bit.SPILBK = 0;

	    // Enable master (0 == slave, 1 == master)
	    // Enable transmission (Talk)
	    // Clock phase (0 == normal, 1 == delayed ВЫБРАЛ 1)
	    // SPI interrupts are disabled
	    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
	    SpiaRegs.SPICTL.bit.TALK = 1;
	    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
	    SpiaRegs.SPICTL.bit.SPIINTENA = 0;

	    // Set the baud rate
	    //
	    SpiaRegs.SPIBRR = SPI_BRR;

	    // Set FREE bit
	    // Halting on a breakpoint will not halt the SPI
	    SpiaRegs.SPIPRI.bit.FREE = 1;

	    //
	    // Release the SPI from reset
	    SpiaRegs.SPICCR.bit.SPISWRESET = 1;
}

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

void setup_DMA(volatile Uint16 *DMADestFPGA, Uint16 size) {
	// COPY PASTE

	Uint16 *DMASourceFPGA = (Uint16 *)(0x340000);
	DMACH1AddrConfig(DMADestFPGA, DMASourceFPGA);

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

void main(void) {
	{
		// COPY PASTE

		//GpioG2DataRegs.GPEDAT.bit.GPIO134 = 0; //Remove system OK flag to FPGA
		GpioG1DataRegs.GPADAT.bit.GPIO0 = 0; 	 //Remove system OK flag to FPGA
	}

    // Disable all interrupts while configuring
    DINT;

    setup_flash_mem();

    InitSysCtrl();      // wakes up the CPU and the clocks
    InitGpio();

    setup_GPIO();       // setup GPIO pins
    setup_SPI();        // setup SPI pins

    // Configuring PIE (Peripheral Interrupt Expansion)
    // Инициализация системы прерываний
	InitPieCtrl(); 		// Initialize the PIE control registers to their default state.
	IER = 0x0000; 		// Disable CPU interrupts and clear all CPU interrupt flags
	IFR = 0x0000; 		// Disable CPU interrupts and clear all CPU interrupt flags
	InitPieVectTable(); // Initialize the PIE vector table with pointers to the shell ISR.

	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM

	//ErrorSet(ERR_EMERGENCY_STOP_ALG);  Исходно стартуем с прерывания, чтобы безопасно все было, когда систему ошибок поправим, вернем

	// Запуска таймера на 300 мкс для главного цикла
	setup_timers(MAIN_CYCLE_US, C28_FREQ);
	start_timers();

	// Проверка запуска М3
	CtoMIpcRegs.CTOMIPCSET.bit.IPC1 = 1; 		   // Send data ready signal
	while (CtoMIpcRegs.CTOMIPCFLG.bit.IPC1 != 0);  // Wait for M3 to read init data

	// Настройка DMA
	// DMASourceFPGA - память на FPGA, DMADestFPGA - локальная копия
	Uint16 DMABufFPGA[128] = {0};
	setup_DMA(&DMABufFPGA[0], 51);

	DataToM3 Data;
	Uint16 FreeTimeCounter = 0;
	Uint64 CycleCounter = 0;
	Uint16 LedCounter = 0;
	Uint8  WatchDog = 0;

	for(;;) {  // Итерации раз в 300 мкс
		if (DmaRegs.CH1.CONTROL.bit.TRANSFERSTS) {
			continue;  // DMA занят, ждем
		}

		// Моргаем светодиодиком
		LedCounter++;
		if (LedCounter == 500) {
			GpioG1DataRegs.GPADAT.bit.GPIO8 = 0;
		}
		else if (LedCounter > 1000) {
			GpioG1DataRegs.GPADAT.bit.GPIO8 = 1;
			LedCounter = 0;
		}

		Data.CycleCounter = CycleCounter;
		Data.FreeTimeCounter = FreeTimeCounter;

		ReadFPGAData(DMABufFPGA, &Data);
		CheckFPGAConnect(Data, &WatchDog);

		Data.C28_Errors = ErrorLatch;
		//WriteOSCI_S6S7(Data);  	  // Взаимодействие с М3

		WriteFPGAData(WatchDog);  // Запись в FPGA

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
		} else if (ErrorLatch & (1 << ERROR_CPU_OVERLOAD)) {
			ErrorReset(ERROR_CPU_OVERLOAD);
		}

		CpuTimer1Regs.TCR.bit.TIF = 1; // Перезапуск таймера

		CycleCounter++;
	}
}
