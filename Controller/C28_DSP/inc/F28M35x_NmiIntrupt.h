//###########################################################################
// FILE:   F28M35x_Nmiintrupt.h
// TITLE:  F28M35x Device NmiIntrupt Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_NmiIntrupt_H
#define F28M35x_NmiIntrupt_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// NmiIntrupt Individual Register Bit Definitions:

struct NMICFG_BITS {              // bits description
        Uint16 NMIEN : 1;         // 0    NMI Enable
        Uint16 rsvd1 : 5;         // 5:1  Reserved
        Uint16 AERRE : 1;         // 6    AERR NMI Enable
        Uint16 rsvd2 : 9;         // 15:7 Reserved
};

union NMICFG_REG {
        Uint16 all;
        struct NMICFG_BITS bit;
};

struct NMIFLG_BITS {                     // bits description
        Uint16 NMIINT : 1;               // 0    NMI Interrupt Flag
        Uint16 CLOCKFAIL : 1;            // 1    CLOCKFAIL NMI Flag
        Uint16 C28RAMUNCERR : 1;         // 2    C28 RAM Uncorrectable Error NMI
                                         //      Flag
        Uint16 C28FLUNCERR : 1;          // 3    C28 Flash Uncorrectable Error
                                         //      NMI Flag
        Uint16 M3BISTERR : 1;            // 4    M3 HW BIST Error NMI Flag
        Uint16 C28BISTERR : 1;           // 5    C28 HW BIST Error NMI Flag
        Uint16 AERR : 1;                 // 6    Analog System Control Error NMI
                                         //      Flag
        Uint16 rsvd2 : 9;                // 15:7 Reserved
};

union NMIFLG_REG {
        Uint16 all;
        struct NMIFLG_BITS bit;
};

struct NMIFLGCLR_BITS {                  // bits description
        Uint16 NMIINT : 1;               // 0    NMI Interrupt Flag Clear
        Uint16 CLOCKFAIL : 1;            // 1    CLOCKFAIL NMI Flag Clear
        Uint16 C28RAMUNCERR : 1;         // 2    C28 RAM Uncorrectable Error NMI
                                         //      Flag Clear
        Uint16 C28FLUNCERR : 1;          // 3    C28 Flash Uncorrectable Error
                                         //      NMI Flag Clear
        Uint16 M3BISTERR : 1;            // 4    M3 HW BIST Error NMI Flag
        Uint16 C28BISTERR : 1;           // 5    C28 HW BIST Error NMI Flag
        Uint16 AERR : 1;                 // 6    Analog System Control Error NMI
                                         //      Flag Clear
        Uint16 rsvd2 : 9;                // 15:7 Reserved
};

union NMIFLGCLR_REG {
        Uint16 all;
        struct NMIFLGCLR_BITS bit;
};

struct NMIFLGFRC_BITS {                  // bits description
        Uint16 rsvd1 : 1;                // 0    Reserved
        Uint16 CLOCKFAIL : 1;            // 1    CLOCKFAIL NMI Flag Force
        Uint16 C28RAMUNCERR : 1;         // 2    C28 RAM Uncorrectable Error NMI
                                         //      Flag Force
        Uint16 C28FLUNCERR : 1;          // 3    C28 Flash Uncorrectable Error
                                         //      NMI Flag Force
        Uint16 rsvd2 : 2;
        Uint16 AERR : 1;                 // 6    Analog System Control Error NMI
                                         //      Flag
        Uint16 rsvd3 : 9;                // 15:7 Reserved
};

union NMIFLGFRC_REG {
        Uint16 all;
        struct NMIFLGFRC_BITS bit;
};

struct NMI_INTRUPT_REGS {
        union        NMICFG_REG NMICFG;               // C28 NMI Configuration
                                                      // Register
        union        NMIFLG_REG NMIFLG;               // C28 NMI Flag Register
        union        NMIFLGCLR_REG NMIFLGCLR;         // C28 NMI Flag Clear
                                                      // Register
        union        NMIFLGFRC_REG NMIFLGFRC;         // C28 NMI Flag Force
                                                      // Register
        Uint16 NMIWDCNT;                              // C28 NMI Watchdog
                                                      // Counter Register
        Uint16 NMIWDPRD;                              // C28 NMI Watchdog Period
                                                      // Register
};

//---------------------------------------------------------------------------
// Nmiintrupt External References & Function Declarations:
extern volatile struct NMI_INTRUPT_REGS NmiIntruptRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_NmiIntrupt_H definition


