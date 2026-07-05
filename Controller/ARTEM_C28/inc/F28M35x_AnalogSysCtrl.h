//###########################################################################
//
// FILE:   F28M35x_AnalogSysCtrl.h
//
// TITLE:  F28M35x Device AnalogSysCtrl Register Definitions.
//
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_ANALOGSYSCTRL_H
#define F28M35x_ANALOGSYSCTRL_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// ANALOGSYSCTRL Individual Register Bit Definitions:

struct INTOVF_BITS {                // bits description
        Uint16 ADCINT1 : 1;         // 0    ADCINT1 Overflow Flag Status
        Uint16 ADCINT2 : 1;         // 1    ADCINT2 Overflow Flag Status
        Uint16 ADCINT3 : 1;         // 2    ADCINT3 Overflow Flag Status
        Uint16 ADCINT4 : 1;         // 3    ADCINT4 Overflow Flag Status
        Uint16 ADCINT5 : 1;         // 4    ADCINT5 Overflow Flag Status
        Uint16 ADCINT6 : 1;         // 5    ADCINT6 Overflow Flag Status
        Uint16 ADCINT7 : 1;         // 6    ADCINT7 Overflow Flag Status
        Uint16 ADCINT8 : 1;         // 7    ADCINT8 Overflow Flag Status
        Uint16 rsvd1 : 1;           // 8    Reserved
        Uint16 rsvd2 : 1;           // 9    Reserved
        Uint16 rsvd3 : 1;           // 10   Reserved
        Uint16 rsvd4 : 1;           // 11   Reserved
        Uint16 rsvd5 : 1;           // 12   Reserved
        Uint16 rsvd6 : 1;           // 13   Reserved
        Uint16 rsvd7 : 1;           // 14   Reserved
        Uint16 rsvd8 : 1;           // 15   Reserved
};

union INTOVF_REG {
        Uint16 all;
        struct INTOVF_BITS bit;
};

struct INTOVFCLR_BITS {             // bits description
        Uint16 ADCINT1 : 1;         // 0    ADCINT1 Overflow Flag Clear
        Uint16 ADCINT2 : 1;         // 1    ADCINT2 Overflow Flag Clear
        Uint16 ADCINT3 : 1;         // 2    ADCINT3 Overflow Flag Clear
        Uint16 ADCINT4 : 1;         // 3    ADCINT4 Overflow Flag Clear
        Uint16 ADCINT5 : 1;         // 4    ADCINT5 Overflow Flag Clear
        Uint16 ADCINT6 : 1;         // 5    ADCINT6 Overflow Flag Clear
        Uint16 ADCINT7 : 1;         // 6    ADCINT7 Overflow Flag Clear
        Uint16 ADCINT8 : 1;         // 7    ADCINT8 Overflow Flag Clear
        Uint16 rsvd1 : 1;           // 8    Reserved
        Uint16 rsvd2 : 1;           // 9    Reserved
        Uint16 rsvd3 : 1;           // 10   Reserved
        Uint16 rsvd4 : 1;           // 11   Reserved
        Uint16 rsvd5 : 1;           // 12   Reserved
        Uint16 rsvd6 : 1;           // 13   Reserved
        Uint16 rsvd7 : 1;           // 14   Reserved
        Uint16 rsvd8 : 1;           // 15   Reserved
};

union INTOVFCLR_REG {
        Uint16 all;
        struct INTOVFCLR_BITS bit;
};

struct CLOCK_BITS {                 // bits description
        Uint16 rsvd1 : 1;           // 0     Reserved
        Uint16 CCLKCTL : 1;         // 1     Control System CLKCTL Reg Write
                                    //       Disable
        Uint16 rsvd2 : 6;           // 7:02  Reserved
        Uint16 PSWD : 8;            // 15:08 Password
};

union CLOCK_REG {
        Uint16 all;
        struct CLOCK_BITS bit;
};

struct CCIBSTATUS_BITS {               // bits description
        Uint16 APGOODSTS : 1;          // 0    Analog System Power Good Status
        Uint16 READY : 1;              // 1    READY Signal State
        Uint16 INT : 1;                // 2    INT Signal State
        Uint16 rsvd1 : 5;              // 7:3  Reserved
        Uint16 ASYSCLKCNT : 8;         // 15:8 8-Bit ASYSCLK Counter
};

union CCIBSTATUS_REG {
        Uint16 all;
        struct CCIBSTATUS_BITS bit;
};

struct CCLKCTL_BITS {              // bits description
        Uint16 CLKDIV : 3;         // 2:0    Analog System Source Clock Divisor
                                   //        Selection
        Uint16 rsvd1 : 13;         // 15:3   Reserved
};

union CCLKCTL_REG {
        Uint16 all;
        struct CCLKCTL_BITS bit;
};

struct TRIGOVF_BITS {             // bits description
        Uint16 TRIG1 : 1;         // 0    TRIG1 Overflow Flag Status
        Uint16 TRIG2 : 1;         // 1    TRIG2 Overflow Flag Status
        Uint16 TRIG3 : 1;         // 2    TRIG3 Overflow Flag Status
        Uint16 TRIG4 : 1;         // 3    TRIG4 Overflow Flag Status
        Uint16 TRIG5 : 1;         // 4    TRIG5 Overflow Flag Status
        Uint16 TRIG6 : 1;         // 5    TRIG6 Overflow Flag Status
        Uint16 TRIG7 : 1;         // 6    TRIG7 Overflow Flag Status
        Uint16 TRIG8 : 1;         // 7    TRIG8 Overflow Flag Status
        Uint16 rsvd1 : 8;         // 15:8 Reserved
};

union TRIGOVF_REG {
        Uint16 all;
        struct TRIGOVF_BITS bit;
};

struct TRIGOVFCLR_BITS {          // bits description
        Uint16 TRIG1 : 1;         // 0    TRIG1 Overflow Flag Clear
        Uint16 TRIG2 : 1;         // 1    TRIG2 Overflow Flag Clear
        Uint16 TRIG3 : 1;         // 2    TRIG3 Overflow Flag Clear
        Uint16 TRIG4 : 1;         // 3    TRIG4 Overflow Flag Clear
        Uint16 TRIG5 : 1;         // 4    TRIG5 Overflow Flag Clear
        Uint16 TRIG6 : 1;         // 5    TRIG6 Overflow Flag Clear
        Uint16 TRIG7 : 1;         // 6    TRIG7 Overflow Flag Clear
        Uint16 TRIG8 : 1;         // 7    TRIG8 Overflow Flag Clear
        Uint16 rsvd1 : 8;         // 15:8 Reserved
};

union TRIGOVFCLR_REG {
        Uint16 all;
        struct TRIGOVFCLR_BITS bit;
};

struct TRIG1SEL_BITS {               // bits description
        Uint16 TRIG1SEL : 5;         // 4:0    ADC Trigger 1 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG1SEL_REG {
        Uint16 all;
        struct TRIG1SEL_BITS bit;
};

struct TRIG2SEL_BITS {               // bits description
        Uint16 TRIG2SEL : 5;         // 4:0    ADC Trigger 2 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG2SEL_REG {
        Uint16 all;
        struct TRIG2SEL_BITS bit;
};

struct TRIG3SEL_BITS {               // bits description
        Uint16 TRIG3SEL : 5;         // 4:0    ADC Trigger 3 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG3SEL_REG {
        Uint16 all;
        struct TRIG3SEL_BITS bit;
};

struct TRIG4SEL_BITS {               // bits description
        Uint16 TRIG4SEL : 5;         // 4:0    ADC Trigger 4 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG4SEL_REG {
        Uint16 all;
        struct TRIG4SEL_BITS bit;
};

struct TRIG5SEL_BITS {               // bits description
        Uint16 TRIG5SEL : 5;         // 4:0    ADC Trigger 5 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG5SEL_REG {
        Uint16 all;
        struct TRIG5SEL_BITS bit;
};

struct TRIG6SEL_BITS {               // bits description
        Uint16 TRIG6SEL : 5;         // 4:0    ADC Trigger 6 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG6SEL_REG {
        Uint16 all;
        struct TRIG6SEL_BITS bit;
};

struct TRIG7SEL_BITS {               // bits description
        Uint16 TRIG7SEL : 5;         // 4:0    ADC Trigger 7 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG7SEL_REG {
        Uint16 all;
        struct TRIG7SEL_BITS bit;
};

struct TRIG8SEL_BITS {               // bits description
        Uint16 TRIG8SEL : 5;         // 4:0    ADC Trigger 8 Input Select
        Uint16 rsvd1 : 11;           // 15:5   Reserved
};

union TRIG8SEL_REG {
        Uint16 all;
        struct TRIG8SEL_BITS bit;
};

struct ANALOG_SYSCTRL_REGS {
        Uint16 rsvd1[16];                     // Reserved
        union    INTOVF_REG INTOVF;           // ADC Interrupt Overflow Detect
        union    INTOVFCLR_REG INTOVFCLR;     // ADC Interrupt Overflow Clear
        Uint16 rsvd2[46];                     // Reserved
        union    CLOCK_REG CLOCK;             // Control System: Lock Register
        union    CCIBSTATUS_REG CCIBSTATUS;   // Control System:  ACIB Status
                                              // Register
        union    CCLKCTL_REG CCLKCTL;         // Control System: Clock Control
                                              // Register
        Uint16 rsvd3[13];                     // Reserved
        union    TRIGOVF_REG TRIGOVF;         // ADC Trigger Overflow Detect
        union    TRIGOVFCLR_REG TRIGOVFCLR;   // ADC Trigger Overflow Clear
        Uint16 rsvd4[30];                     // Reserved
        union    TRIG1SEL_REG TRIG1SEL;       // ADC Trigger 1 Input Select
        union    TRIG2SEL_REG TRIG2SEL;       // ADC Trigger 2 Input Select
        union    TRIG3SEL_REG TRIG3SEL;       // ADC Trigger 3 Input Select
        union    TRIG4SEL_REG TRIG4SEL;       // ADC Trigger 4 Input Select
        union    TRIG5SEL_REG TRIG5SEL;       // ADC Trigger 5 Input Select
        union    TRIG6SEL_REG TRIG6SEL;       // ADC Trigger 6 Input Select
        union    TRIG7SEL_REG TRIG7SEL;       // ADC Trigger 7 Input Select
        union    TRIG8SEL_REG TRIG8SEL;       // ADC Trigger 8 Input Select
};

//---------------------------------------------------------------------------
// Analog System Control External References & Function Declarations:
extern volatile struct ANALOG_SYSCTRL_REGS AnalogSysctrlRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_ANALOGSYSCTRL_H definition


