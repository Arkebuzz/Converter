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


Uint32 INIT_GPIO_Setup(void)
{
	EALLOW;
	GpioG2CtrlRegs.GPEDIR.bit.GPIO128 = 1;
	GpioG1CtrlRegs.GPADIR.bit.GPIO19 = 1;
	EDIS;
	//GpioG2DataRegs.GPEDAT.bit.GPIO128 = 0;// turn off LED
	return(1);
}

/*
void Setup_DMA(Uint16 MeasBufSizeHalf)
{
	Uint16 TransfersCount = MeasBufSizeHalf/32 - 1;
	Uint16 BusrstCount = 32 - 1;

    DMACH1BurstConfig(BusrstCount,2,2); 				// Set up to use 32-bit data size
    DMACH1TransferConfig(TransfersCount,2,2); 			// Pointers are based on 16-bit words
    DMACH1WrapConfig(0xFFFF,0,0xFFFF,0);				// Increment by 2 (2 16-bit words)
    DMACH1ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT, CHINT_END, CHINT_DISABLE);

    DMACH2BurstConfig(BusrstCount,2,2); 				// Set up to use 32-bit data size
    DMACH2TransferConfig(TransfersCount,2,2); 			// Pointers are based on 16-bit words
    DMACH2WrapConfig(0xFFFF,0,0xFFFF,0);				// Increment by 2 (2 16-bit words)
    DMACH2ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT, CHINT_END, CHINT_DISABLE);

    DMACH3BurstConfig(BusrstCount,2,2); 				// Set up to use 32-bit data size
    DMACH3TransferConfig(TransfersCount,2,2); 			// Pointers are based on 16-bit words
    DMACH3WrapConfig(0xFFFF,0,0xFFFF,0);				// Increment by 2 (2 16-bit words)
    DMACH3ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT, CHINT_END, CHINT_DISABLE);

    DMACH4BurstConfig(BusrstCount,2,2); 				// Set up to use 32-bit data size
    DMACH4TransferConfig(TransfersCount,2,2); 			// Pointers are based on 16-bit words
    DMACH4WrapConfig(0xFFFF,0,0xFFFF,0);				// Increment by 2 (2 16-bit words)
    DMACH4ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT, CHINT_END, CHINT_DISABLE);

    DMACH5BurstConfig(BusrstCount,2,2); 				// Set up to use 32-bit data size
    DMACH5TransferConfig(TransfersCount,2,2); 			// Pointers are based on 16-bit words
    DMACH5WrapConfig(0xFFFF,0,0xFFFF,0);				// Increment by 2 (2 16-bit words)
    DMACH5ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT, CHINT_END, CHINT_DISABLE);

    DMACH6BurstConfig(BusrstCount,2,2); 				// Set up to use 32-bit data size
    DMACH6TransferConfig(TransfersCount,2,2); 			// Pointers are based on 16-bit words
    DMACH6WrapConfig(0xFFFF,0,0xFFFF,0);				// Increment by 2 (2 16-bit words)
    DMACH6ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT, CHINT_END, CHINT_DISABLE);

    StartDMACH1();
    StartDMACH2();
    StartDMACH3();
    StartDMACH4();
    StartDMACH5();
    StartDMACH6();

}
*/

Uint32 INIT_AssignInterrupts(void)
{
	return(1);
}

Uint32 INIT_Peripherals(void)
{
	InitAdc1(); // init the ADC1
	InitAdc2(); // init the ADC2
	//Adc1OffsetSelfCal();
	//Adc2OffsetSelfCal();
	InitAdc1Aio(); /* Configure ADC1 pins using AIO regs*/
	InitAdc2Aio(); /* Configure ADC2 pins using AIO regs*/

	return(1);
}

Uint32 INIT_Setup_ADC(void)
{
	// Configure ADC
    EALLOW;
	Adc1Regs.ADCCTL2.bit.ADCNONOVERLAP = 1;     // Enable non-overlap mode i.e. conversion and future sampling events dont overlap
	Adc2Regs.ADCCTL2.bit.ADCNONOVERLAP = 1;

	Adc1Regs.ADCCTL1.bit.INTPULSEPOS   = 1;     // ADCINT trips after AdcResults latch
	Adc2Regs.ADCCTL1.bit.INTPULSEPOS   = 1;

	Adc1Regs.INTSEL1N2.bit.INT2E       = 1;     // Enabled ADCINT2 of ADC1
	Adc2Regs.INTSEL1N2.bit.INT1E       = 1;     // Enabled ADCINT1 of ADC2

	Adc1Regs.INTSEL1N2.bit.INT2CONT    = 0;     // Disable ADCINT2 Continuous mode
	Adc2Regs.INTSEL1N2.bit.INT1CONT    = 0;     // Disable ADCINT1 Continuous mode

	Adc1Regs.INTSEL1N2.bit.INT2SEL     = 0x09;     // setup EOC9 of ADC1 to trigger ADCINT2 to fire
	Adc2Regs.INTSEL1N2.bit.INT1SEL     = 0x09;     // setup EOC9 of ADC2 to trigger ADCINT1 to fire

	//Adc1Regs.SOCPRICTL.bit.SOCPRIORITY = 0x10;   //All SOCs are in high priority mode
	//Adc2Regs.SOCPRICTL.bit.SOCPRIORITY = 0x10;   //All SOCs are in high priority mode

	Adc1Regs.ADCSOC0CTL.bit.CHSEL      = 0x0;     // set SOC0 channel select to ADC1A0 of ADC1
	Adc1Regs.ADCSOC1CTL.bit.CHSEL      = 0x2;     // set SOC1 channel select to ADC1A2 of ADC1
	Adc1Regs.ADCSOC2CTL.bit.CHSEL      = 0x3;     // set SOC2 channel select to ADC1A3 of ADC1
	Adc1Regs.ADCSOC3CTL.bit.CHSEL      = 0x4;     // set SOC3 channel select to ADC1A4 of ADC1
	Adc1Regs.ADCSOC4CTL.bit.CHSEL      = 0x6;     // set SOC4 channel select to ADC1A6 of ADC1
	Adc1Regs.ADCSOC5CTL.bit.CHSEL      = 0x7;     // set SOC5 channel select to ADC1A7 of ADC1
	Adc1Regs.ADCSOC6CTL.bit.CHSEL      = 0x8;     // set SOC6 channel select to ADC1B0 of ADC1
	Adc1Regs.ADCSOC7CTL.bit.CHSEL      = 0xB;     // set SOC7 channel select to ADC1B3 of ADC1
	Adc1Regs.ADCSOC8CTL.bit.CHSEL      = 0xC;     // set SOC8 channel select to ADC1B4 of ADC1
	Adc1Regs.ADCSOC9CTL.bit.CHSEL      = 0xF;     // set SOC9 channel select to ADC1B7 of ADC1

	Adc2Regs.ADCSOC0CTL.bit.CHSEL      = 0x0;     // set SOC0 channel select to ADC2A0 of ADC2
	Adc2Regs.ADCSOC1CTL.bit.CHSEL      = 0x2;     // set SOC1 channel select to ADC2A2 of ADC2
	Adc2Regs.ADCSOC2CTL.bit.CHSEL      = 0x3;     // set SOC2 channel select to ADC2A3 of ADC2
	Adc2Regs.ADCSOC3CTL.bit.CHSEL      = 0x4;     // set SOC3 channel select to ADC2A4 of ADC2
	Adc2Regs.ADCSOC4CTL.bit.CHSEL      = 0x6;     // set SOC4 channel select to ADC2A6 of ADC2
	Adc2Regs.ADCSOC5CTL.bit.CHSEL      = 0x7;     // set SOC5 channel select to ADC2A7 of ADC2
	Adc2Regs.ADCSOC6CTL.bit.CHSEL      = 0x8;     // set SOC6 channel select to ADC2B0 of ADC2
	Adc2Regs.ADCSOC7CTL.bit.CHSEL      = 0xB;     // set SOC7 channel select to ADC2B3 of ADC2
	Adc2Regs.ADCSOC8CTL.bit.CHSEL      = 0xC;     // set SOC8 channel select to ADC2B4 of ADC2
	Adc2Regs.ADCSOC9CTL.bit.CHSEL      = 0xF;     // set SOC9 channel select to ADC2B7 of ADC2

	AnalogSysctrlRegs.TRIG1SEL.all     = 0x3;     // Assigning CPU Timer 2 to ADC TRIGGER 1 of the analog system module

	Adc1Regs.ADCSOC0CTL.bit.TRIGSEL    = 0x5;     // Set SOC0/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC1CTL.bit.TRIGSEL    = 0x5;     // set SOC1/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC2CTL.bit.TRIGSEL    = 0x5;     // set SOC2/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC3CTL.bit.TRIGSEL    = 0x5;     // set SOC3/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC4CTL.bit.TRIGSEL    = 0x5;     // set SOC4/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC5CTL.bit.TRIGSEL    = 0x5;     // set SOC5/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC6CTL.bit.TRIGSEL    = 0x5;     // set SOC6/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC7CTL.bit.TRIGSEL    = 0x5;     // set SOC7/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC8CTL.bit.TRIGSEL    = 0x5;     // set SOC8/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc1Regs.ADCSOC9CTL.bit.TRIGSEL    = 0x5;     // set SOC9/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)


	Adc2Regs.ADCSOC0CTL.bit.TRIGSEL    = 0x5;     // Set SOC0/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC1CTL.bit.TRIGSEL    = 0x5;     // set SOC1/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC2CTL.bit.TRIGSEL    = 0x5;     // set SOC2/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC3CTL.bit.TRIGSEL    = 0x5;     // set SOC3/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC4CTL.bit.TRIGSEL    = 0x5;     // set SOC4/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC5CTL.bit.TRIGSEL    = 0x5;     // set SOC5/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC6CTL.bit.TRIGSEL    = 0x5;     // set SOC6/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC7CTL.bit.TRIGSEL    = 0x5;     // set SOC7/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC8CTL.bit.TRIGSEL    = 0x5;     // set SOC8/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)
	Adc2Regs.ADCSOC9CTL.bit.TRIGSEL    = 0x5;     // set SOC9/ADC1 start trigger to ADC Trigger 1(CPU Timer 2 INT)

	/*
	Adc2Regs.ADCINTSOCSEL1.bit.SOC0    = 0x2;     // set SOC0/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL1.bit.SOC1    = 0x2;     // set SOC1/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL1.bit.SOC2    = 0x2;     // set SOC2/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL1.bit.SOC3    = 0x2;     // set SOC3/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL1.bit.SOC4    = 0x2;     // set SOC4/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL1.bit.SOC5    = 0x2;     // set SOC5/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL1.bit.SOC6    = 0x2;     // set SOC6/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL1.bit.SOC7    = 0x2;     // set SOC7/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL2.bit.SOC8    = 0x2;     // set SOC8/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	Adc2Regs.ADCINTSOCSEL2.bit.SOC9    = 0x2;     // set SOC9/ADC2 start trigger to ADCINT2. TRIGSEL field is ignored
	*/

	Uint16 ADCAquisitionPuses = 22;

	Adc1Regs.ADCSOC0CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC0/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC1CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC1/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC2CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC2/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC3CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC3/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC4CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC4/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC5CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC5/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC6CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC6/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC7CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC7/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC8CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC8/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc1Regs.ADCSOC9CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC9/ADC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	Adc2Regs.ADCSOC0CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC0/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC1CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC1/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC2CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC2/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC3CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC3/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC4CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC4/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC5CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC5/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC6CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC6/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC7CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC7/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC8CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC8/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	Adc2Regs.ADCSOC9CTL.bit.ACQPS      = ADCAquisitionPuses;     // set SOC9/ADC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	EDIS;

	return (0);
}

Uint32 INIT_Setup_Timers(unsigned short ADCSampleCycle_uS, unsigned short MainCycleTimer_uS, unsigned short CPUfreq_value_MHZ )
{
	//Set up Timer 2 for ADC triggering
	CpuTimer2Regs.TCR.bit.TSS = 1; //Stop timer
	CpuTimer2Regs.TCR.bit.TIE = 0; //Disable timer interrupt

	CpuTimer2Regs.TPRH.bit.TDDRH = 0;
	CpuTimer2Regs.TPR.bit.TDDR = CPUfreq_value_MHZ - 1; //Prescale to 1uS tick for counter
	CpuTimer2Regs.PRD.all = ADCSampleCycle_uS-1; //ADCSampleCycle_uS time for data gathering in microseconds
	CpuTimer2Regs.TCR.bit.FREE = 1; //Set timer debug mode to freerun

	//Set up Timer 1 for main cycle triggering
	CpuTimer1Regs.TCR.bit.TSS = 1; //Stop timer
	CpuTimer1Regs.TCR.bit.TIE = 0; //Disable timer interrupt

	CpuTimer1Regs.TPRH.bit.TDDRH = 0;
	CpuTimer1Regs.TPR.bit.TDDR = CPUfreq_value_MHZ - 1; //Prescale to 1uS tick for counter
	CpuTimer1Regs.PRD.all = MainCycleTimer_uS-1; //MainCycleTimer_uS time for main cycle in microseconds
	CpuTimer1Regs.TCR.bit.FREE = 1; //Set timer debug mode to freerun

	return (0);
}

void INIT_StartTimers(void)
{
	CpuTimer2Regs.TCR.bit.TRB = 1; //Reload timer
	CpuTimer2Regs.TCR.bit.TIF = 1; //Reset interrupt flag
	CpuTimer2Regs.TCR.bit.TIE = 1; //Enable timer interrupt, start ADC conversions
	CpuTimer2Regs.TCR.bit.TSS = 0; //Ensure timer start
	CpuTimer1Regs.TCR.bit.TRB = 1; //Reload timer
	CpuTimer1Regs.TCR.bit.TIF = 1; //Reset interrupt flag
	CpuTimer1Regs.TCR.bit.TIE = 1; //Enable timer interrupt
	CpuTimer1Regs.TCR.bit.TSS = 0; //Ensure timer start
}

