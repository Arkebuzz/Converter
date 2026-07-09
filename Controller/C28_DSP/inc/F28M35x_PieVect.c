//###########################################################################
// FILE:    F28M35x_PieVect.c
// TITLE:   F28M35x Devices PIE Vector Table Initialization Functions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#include "F28M35x_Device.h"     // F28M35x Headerfile Include File
#include "F28M35x_Examples.h"   // F28M35x Examples Include File

const struct PIE_VECT_TABLE PieVectTableInit = {

    PIE_RESERVED,    // 1  Reserved space
    PIE_RESERVED,    // 2  Reserved space
    PIE_RESERVED,    // 3  Reserved space
    PIE_RESERVED,    // 4  Reserved space
    PIE_RESERVED,    // 5  Reserved space
    PIE_RESERVED,    // 6  Reserved space
    PIE_RESERVED,    // 7  Reserved space
    PIE_RESERVED,    // 8  Reserved space
    PIE_RESERVED,    // 9  Reserved space
    PIE_RESERVED,    // 10 Reserved space
    PIE_RESERVED,    // 11 Reserved space
    PIE_RESERVED,    // 12 Reserved space
    PIE_RESERVED,    // 13 Reserved space

// Non-Peripheral Interrupts
    INT13_ISR,       // CPU-Timer1
    INT14_ISR,       // CPU-Timer2
    DATALOG_ISR,     // Datalogging interrupt
    RTOSINT_ISR,     // RTOS interrupt
    EMUINT_ISR,      // Emulation interrupt
    NMI_ISR,         // Non-maskable interrupt
    ILLEGAL_ISR,     // Illegal operation TRAP
    USER1_ISR,       // User Defined trap 1
    USER2_ISR,       // User Defined trap 2
    USER3_ISR,       // User Defined trap 3
    USER4_ISR,       // User Defined trap 4
    USER5_ISR,       // User Defined trap 5
    USER6_ISR,       // User Defined trap 6
    USER7_ISR,       // User Defined trap 7
    USER8_ISR,       // User Defined trap 8
    USER9_ISR,       // User Defined trap 9
    USER10_ISR,      // User Defined trap 10
    USER11_ISR,      // User Defined trap 11
    USER12_ISR,      // User Defined trap 12

// Group 1 PIE Vectors
    ADCINT1_ISR,       // 1.1 ADC  ADC - make rsvd1_1 if ADCINT1 is wanted in
                       // Group 10 instead.
    ADCINT2_ISR,       // 1.2 ADC  ADC - make rsvd1_2 if ADCINT2 is wanted in
                       // Group 10 instead.
    rsvd_ISR,          // 1.3
    XINT1_ISR,         // 1.4 External Interrupt
    XINT2_ISR,         // 1.5 External Interrupt
    rsvd_ISR,          // 1.6
    TINT0_ISR,         // 1.7 Timer 0
    WAKEINT_ISR,       // 1.8 WD, Low Power

// Group 2 PIE Vectors
    EPWM1_TZINT_ISR,   // 2.1 EPWM-1 Trip Zone
    EPWM2_TZINT_ISR,   // 2.2 EPWM-2 Trip Zone
    EPWM3_TZINT_ISR,   // 2.3 EPWM-3 Trip Zone
    EPWM4_TZINT_ISR,   // 2.4 EPWM-4 Trip Zone
    EPWM5_TZINT_ISR,   // 2.5 EPWM-5 Trip Zone
    EPWM6_TZINT_ISR,   // 2.6 EPWM-6 Trip Zone
    EPWM7_TZINT_ISR,   // 2.7 EPWM-7 Trip Zone
    EPWM8_TZINT_ISR,   // 2.8 EPWM-8 Trip Zone

// Group 3 PIE Vectors
    EPWM1_INT_ISR,     // 3.1 EPWM-1 Interrupt
    EPWM2_INT_ISR,     // 3.2 EPWM-2 Interrupt
    EPWM3_INT_ISR,     // 3.3 EPWM-3 Interrupt
    EPWM4_INT_ISR,     // 3.4 EPWM-4 Interrupt
    EPWM5_INT_ISR,     // 3.5 EPWM-5 Interrupt
    EPWM6_INT_ISR,     // 3.6 EPWM-6 Interrupt
    EPWM7_INT_ISR,     // 3.7 EPWM-7 Interrupt
    EPWM8_INT_ISR,     // 3.8 EPWM-8 Interrupt

// Group 4 PIE Vectors
    ECAP1_INT_ISR,     // 4.1 ECAP-1
    ECAP2_INT_ISR,     // 4.2 ECAP-2
    ECAP3_INT_ISR,     // 4.3 ECAP-3
    ECAP4_INT_ISR,     // 4.4 ECAP-4
    ECAP5_INT_ISR,     // 4.5 ECAP-5
    ECAP6_INT_ISR,     // 4.6 ECAP-6
    rsvd_ISR,          // 4.7
    EPWM9_TZINT_ISR,   // 4.8 EPWM9 Trip Zone-9

// Group 5 PIE Vectors

    EQEP1_INT_ISR,     // 5.1 EQEP-1
    EQEP2_INT_ISR,     // 5.2 EQEP-2
    EQEP3_INT_ISR,     // 5.3 EQEP-3
    rsvd_ISR,          // 5.4
    rsvd_ISR,          // 5.5
    rsvd_ISR,          // 5.6
    rsvd_ISR,          // 5.7
    EPWM9_INT_ISR,     // 5.8 EPWM-9

// Group 6 PIE Vectors
    SPIRXINTA_ISR,     // 6.1 SPI-A
    SPITXINTA_ISR,     // 6.2 SPI-A
    rsvd_ISR,          // 6.3
    rsvd_ISR,          // 6.4
    MRINTA_ISR,        // 6.5 McBSP-A
    MXINTA_ISR,        // 6.6 McBSP-A
    rsvd_ISR,          // 6.7
    rsvd_ISR,          // 6.8

// Group 7 PIE Vectors
    DINTCH1_ISR,       // 7.1 DMA Channel 1
    DINTCH2_ISR,       // 7.2 DMA Channel 2
    DINTCH3_ISR,       // 7.3 DMA Channel 3
    DINTCH4_ISR,       // 7.4 DMA Channel 4
    DINTCH5_ISR,       // 7.5 DMA Channel 5
    DINTCH6_ISR,       // 7.6 DMA Channel 6
    rsvd_ISR,          // 7.7
    rsvd_ISR,          // 7.8

// Group 8 PIE Vectors
    I2CINT1A_ISR,      // 8.1 I2C-A
    I2CINT2A_ISR,      // 8.2 I2C-A
    rsvd_ISR,          // 8.3
    rsvd_ISR,          // 8.4
    rsvd_ISR,          // 8.5
    rsvd_ISR,          // 8.6
    rsvd_ISR,          // 8.7
    rsvd_ISR,          // 8.8

// Group 9 PIE Vectors
    SCIRXINTA_ISR,     // 9.1 SCI-A
    SCITXINTA_ISR,     // 9.2 SCI-A
    rsvd_ISR,          // 9.3
    rsvd_ISR,          // 9.4
    rsvd_ISR,          // 9.5
    rsvd_ISR,          // 9.6
    rsvd_ISR,          // 9.7
    rsvd_ISR,          // 9.8

// Group 10 PIE Vectors
    rsvd_ISR,          // 10.1 Can be ADCINT1, but must make ADCINT1 in Group 1
                       // space "reserved".
    rsvd_ISR,          // 10.2 Can be ADCINT2, but must make ADCINT2 in Group 1
                       // space "reserved".
    ADCINT3_ISR,       // 10.3 ADC Interrupt 3
    ADCINT4_ISR,       // 10.4 ADC Interrupt 4
    ADCINT5_ISR,       // 10.5 ADC Interrupt 5
    ADCINT6_ISR,       // 10.6 ADC Interrupt 6
    ADCINT7_ISR,       // 10.7 ADC Interrupt 7
    ADCINT8_ISR,       // 10.8 ADC Interrupt 8

// Group 11 PIE Vectors
    MTOCIPC_INT1_ISR,     // 11.1 Master to Control Processor IPC Interrupt
    MTOCIPC_INT2_ISR,     // 11.2 Master to Control Processor IPC Interrupt
    MTOCIPC_INT3_ISR,     // 11.3 Master to Control Processor IPC Interrupt
    MTOCIPC_INT4_ISR,     // 11.4 Master to Control Processor IPC Interrupt
    rsvd_ISR,             // 11.5 
    rsvd_ISR,             // 11.6
    rsvd_ISR,             // 11.7 
    rsvd_ISR,             // 11.8 

// Group 12 PIE Vectors
    XINT3_ISR,         // 12.1 External Interrupt
    CFLSINGERR_ISR,    // 12.2 Flash Single Error
    CFLFSM_ISR,        // 12.3
    CRAMSINGERR_ISR,   // 12.4 RAM Single Error
    CRAMACCVIOL_ISR,   // 12.5 RAM Access Violation
    rsvd_ISR,          // 12.6
    LVF_ISR,           // 12.7 Latched Overflow
    LUF_ISR            // 12.8 Latched Underflow
};

//---------------------------------------------------------------------------
// InitPieVectTable:
//---------------------------------------------------------------------------
// This function initializes the PIE vector table to a known state.
// This function must be executed after boot time.

void InitPieVectTable(void)
{
    int16 i;
    Uint32 *Source = (void *) &PieVectTableInit;
    Uint32 *Dest = (void *) &PieVectTable;

    EALLOW;
    for(i=0; i < 128; i++)
        *Dest++ = *Source++;
    EDIS;

    // Enable the PIE Vector Table
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}

