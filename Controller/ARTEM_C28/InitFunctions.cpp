/*
 * InitFunctions.c
 *
 *  Created on: 30.07.2014
 *      Author: Daler
 */

#include "InitFunctions.h"

#define CPU_FREQ     100E6      // 100MHz
#define SPI_BAUDRATE 1000000     // 1000KHz
#define LSPCLK_FREQ  (CPU_FREQ/4)
#define SPI_BRR      (LSPCLK_FREQ/SPI_BAUDRATE)-1


void Init_SPI()
{
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


void INIT_GPIO_Setup(void) {
	EALLOW;
	GpioG2CtrlRegs.GPEDIR.bit.GPIO128 = 1;
	GpioG1CtrlRegs.GPADIR.bit.GPIO19 = 1;
	EDIS;
}


void INIT_Setup_DMA(volatile Uint16 *DMADestFPGA, Uint16 size) {
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

void INIT_Setup_Timers(unsigned short MainCycleTimer_uS, unsigned short CPUfreq_value_MHZ)
{
	//Set up Timer 1 for main cycle triggering
	CpuTimer1Regs.TCR.bit.TSS = 1; // Stop timer
	CpuTimer1Regs.TCR.bit.TIE = 0; // Disable timer interrupt

	CpuTimer1Regs.TPRH.bit.TDDRH = 0;
	CpuTimer1Regs.TPR.bit.TDDR = CPUfreq_value_MHZ - 1; //Prescale to 1uS tick for counter
	CpuTimer1Regs.PRD.all = MainCycleTimer_uS-1; //MainCycleTimer_uS time for main cycle in microseconds
	CpuTimer1Regs.TCR.bit.FREE = 1; //Set timer debug mode to freerun
}

void INIT_Start_Timers(void)
{
	CpuTimer1Regs.TCR.bit.TRB = 1; //Reload timer
	CpuTimer1Regs.TCR.bit.TIF = 1; //Reset interrupt flag
	CpuTimer1Regs.TCR.bit.TIE = 1; //Enable timer interrupt
	CpuTimer1Regs.TCR.bit.TSS = 0; //Ensure timer start
}

