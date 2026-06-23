//###########################################################################
// FILE:   F28M35x_Xintrupt.h
// TITLE:  F28M35x Device XINTRUPT Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_XINTRUPT_H
#define F28M35x_XINTRUPT_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// XINTRUPT Individual Register Bit Definitions:

struct XINTCR_BITS {                 // bits description
        Uint16 ENABLE : 1;           // 0    XINT1 Enable
        Uint16 rsvd1 : 1;            // 1    Reserved
        Uint16 POLARITY : 2;         // 3:2  XINT1 Polarity
        Uint16 rsvd2 : 12;           // 15:4 Reserved
};

union XINTCR_REG {
        Uint16 all;
        struct XINTCR_BITS bit;
};

struct XINTRUPT_REGS {
        union    XINTCR_REG XINT1CR;   // XINT1 Configuration Register
        union    XINTCR_REG XINT2CR;   // XINT2 Configuration Register
        union    XINTCR_REG XINT3CR;   // XINT3 Configuration Register
        Uint16 rsvd1[5];               // Reserved
        Uint16 XINT1CTR;               // XINT1 Counter Register
        Uint16 XINT2CTR;               // XINT2 Counter Register
        Uint16 XINT3CTR;               // XINT3 Counter Register
        Uint16 rsvd[5];                // Reserved
};

//---------------------------------------------------------------------------
// Xintrupt External References & Function Declarations:
extern volatile struct XINTRUPT_REGS XIntruptRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_XINTRUPT_H definition


