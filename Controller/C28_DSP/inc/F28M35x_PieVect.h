//###########################################################################
// FILE:   F28M35x_PieVect.h
// TITLE:  F28M35x Devices PIE Vector Table Definitions.
//###########################################################################
// $TI Release:
// $Release Date:
//###########################################################################

#ifndef F28M35x_PIE_VECT_H
#define F28M35x_PIE_VECT_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// PIE Interrupt Vector Table Definition:
// Create a user type called PINT (pointer to interrupt):

typedef __interrupt void (*PINT)(void);

// Define Vector Table:
struct PIE_VECT_TABLE {

// Reset is never fetched from this table.
// It will always be fetched from 0x3FFFC0 in
// boot ROM

        PINT PIE1_RESERVED;
        PINT PIE2_RESERVED;
        PINT PIE3_RESERVED;
        PINT PIE4_RESERVED;
        PINT PIE5_RESERVED;
        PINT PIE6_RESERVED;
        PINT PIE7_RESERVED;
        PINT PIE8_RESERVED;
        PINT PIE9_RESERVED;
        PINT PIE10_RESERVED;
        PINT PIE11_RESERVED;
        PINT PIE12_RESERVED;
        PINT PIE13_RESERVED;

// Non-Peripheral Interrupts:
        PINT TINT1;       // CPU-Timer1
        PINT TINT2;       // CPU-Timer2
        PINT DATALOG;     // Datalogging interrupt
        PINT RTOSINT;     // RTOS interrupt
        PINT EMUINT;      // Emulation interrupt
        PINT NMI;         // Non-maskable interrupt
        PINT ILLEGAL;     // Illegal operation TRAP
        PINT USER1;       // User Defined trap 1
        PINT USER2;       // User Defined trap 2
        PINT USER3;       // User Defined trap 3
        PINT USER4;       // User Defined trap 4
        PINT USER5;       // User Defined trap 5
        PINT USER6;       // User Defined trap 6
        PINT USER7;       // User Defined trap 7
        PINT USER8;       // User Defined trap 8
        PINT USER9;       // User Defined trap 9
        PINT USER10;      // User Defined trap 10
        PINT USER11;      // User Defined trap 11
        PINT USER12;      // User Defined trap 12

// Group 1 PIE Peripheral Vectors:
        PINT ADCINT1;     // ADC - make rsvd1_1 if ADCINT1 is wanted in Group 10
                          // instead.
        PINT ADCINT2;     // ADC - make rsvd1_2 if ADCINT2 is wanted in Group 10
                          // instead.
        PINT rsvd1_3;
        PINT XINT1;
        PINT XINT2;
        PINT rsvd1_6;
        PINT TINT0;         // Timer 0
        PINT WAKEINT;       // WD

// Group 2 PIE Peripheral Vectors:
        PINT EPWM1_TZINT;   // EPWM Trip Zone-1
        PINT EPWM2_TZINT;   // EPWM Trip Zone-2
        PINT EPWM3_TZINT;   // EPWM Trip Zone-3
        PINT EPWM4_TZINT;   // EPWM Trip Zone-4
        PINT EPWM5_TZINT;   // EPWM Trip Zone-5
        PINT EPWM6_TZINT;   // EPWM Trip Zone-6
        PINT EPWM7_TZINT;   // EPWM Trip Zone-7
        PINT EPWM8_TZINT;   // EPWM Trip Zone-8

// Group 3 PIE Peripheral Vectors:
        PINT EPWM1_INT;     // EPWM-1
        PINT EPWM2_INT;     // EPWM-2
        PINT EPWM3_INT;     // EPWM-3
        PINT EPWM4_INT;     // EPWM-4
        PINT EPWM5_INT;     // EPWM-5
        PINT EPWM6_INT;     // EPWM-6
        PINT EPWM7_INT;     // EPWM-7
        PINT EPWM8_INT;     // EPWM-8

// Group 4 PIE Peripheral Vectors:
        PINT ECAP1_INT;     // ECAP-1
        PINT ECAP2_INT;     // ECAP-2
        PINT ECAP3_INT;     // ECAP-3
        PINT ECAP4_INT;     // ECAP-4
        PINT ECAP5_INT;     // ECAP-5
        PINT ECAP6_INT;     // ECAP-6
        PINT rsvd4_7;
        PINT EPWM9_TZINT;  // EPWM-9

// Group 5 PIE Peripheral Vectors:
        PINT EQEP1_INT;   // EQEP-1
        PINT EQEP2_INT;   // EQEP-2
        PINT EQEP3_INT;   // EQEP-3
        PINT rsvd5_4;
        PINT rsvd5_5;
        PINT rsvd5_6;
        PINT rsvd5_7;
        PINT EPWM9_INT;   // EPWM-9

// Group 6 PIE Peripheral Vectors:
        PINT SPIRXINTA;   // SPI-A
        PINT SPITXINTA;   // SPI-A
        PINT rsvd6_3;
        PINT rsvd6_4;
        PINT MRINTA;      // McBSP-A
        PINT MXINTA;      // McBSP-A
        PINT rsvd6_7;
        PINT rsvd6_8;

// Group 7 PIE Peripheral Vectors:
        PINT DINTCH1;     // DMA Channel 1
        PINT DINTCH2;     // DMA Channel 2
        PINT DINTCH3;     // DMA Channel 3
        PINT DINTCH4;     // DMA Channel 4
        PINT DINTCH5;     // DMA Channel 5
        PINT DINTCH6;     // DMA Channel 6
        PINT rsvd7_7;
        PINT rsvd7_8;

// Group 8 PIE Peripheral Vectors:
        PINT I2CINT1A;    // I2C-A
        PINT I2CINT2A;    // I2C-A
        PINT rsvd8_3;
        PINT rsvd8_4;
        PINT rsvd8_5;
        PINT rsvd8_6;
        PINT rsvd8_7;
        PINT rsvd8_8;

// Group 9 PIE Peripheral Vectors:

        PINT SCIRXINTA;    // SCI-A
        PINT SCITXINTA;    // SCI-A
        PINT rsvd9_3;
        PINT rsvd9_4;
        PINT rsvd9_5;
        PINT rsvd9_6;
        PINT rsvd9_7;
        PINT rsvd9_8;

// Group 10 PIE Peripheral Vectors:
        PINT rsvd10_1;   // Can be ADCINT1, but must make ADCINT1 in Group 1
                         // space "reserved".
        PINT rsvd10_2;   // Can be ADCINT2, but must make ADCINT2 in Group 1
                         // space "reserved".
        PINT ADCINT3;    // ADC Interrupt 3
        PINT ADCINT4;    // ADC Interrupt 4
        PINT ADCINT5;    // ADC Interrupt 5
        PINT ADCINT6;    // ADC Interrupt 6
        PINT ADCINT7;    // ADC Interrupt 7
        PINT ADCINT8;    // ADC Interrupt 8

// Group 11 PIE Peripheral Vectors:

        PINT MTOCIPC_INT1;    // Master to Control Processor IPC Interrupt 1
        PINT MTOCIPC_INT2;    // Master to Control Processor IPC Interrupt 2
        PINT MTOCIPC_INT3;    // Master to Control Processor IPC Interrupt 3
        PINT MTOCIPC_INT4;    // Master to Control Processor IPC Interrupt 4
        PINT rsvd11_5;
        PINT rsvd11_6;
        PINT rsvd11_7;
        PINT rsvd11_8;

// Group 12 PIE Peripheral Vectors:
        PINT XINT3;           // XINT
        PINT CFLSINGERR;      // Flash Single Error
        PINT CFLFSM;          // CFLFSM
        PINT CRAMSINGERR;     // RAM Single Error
        PINT CRAMACCVIOL;     // RAM Access Violation
        PINT rsvd12_6;
        PINT LVF;             // Latched overflow
        PINT LUF;             // Latched underflow
};

//---------------------------------------------------------------------------
// PIE Interrupt Vector Table External References & Function Declarations:
extern struct PIE_VECT_TABLE PieVectTable;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    // end of F28M35x_PIE_VECT_H definition




