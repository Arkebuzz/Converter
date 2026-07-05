//###########################################################################
// FILE:   F28M35x_Devemu.h
// TITLE:  F28M35x Device DEVEMU Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_DEVEMU_H
#define F28M35x_DEVEMU_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// DEVEMU Individual Register Bit Definitions:

struct DEVICECNF_BITS {          // bits  description
        Uint16 rsvd1 : 3;        // 2:0   reserved
        Uint16 VMAPS : 1;        // 3     VMAP Status
        Uint16 rsvd2 : 1;        // 4     reserved
        Uint16 XRSn : 1;         // 5     XRSn Signal Status
        Uint16 rsvd3 : 10;       // 15:6  reserved
        Uint16 rsvd4 : 3;        // 18:16 reserved
        Uint16 ENPROT : 1;       // 19    Enable/Disable pipeline protection
        Uint16 MONPRIV : 1;      // 20    MONPRIV Enable Bit
        Uint16 rsvd5 : 6;        // 26:21 reserved
        Uint16 TRSTn : 1;        // 27    Status of TRSTn signal
        Uint16 rsvd6 : 2;        // 29:28 PINOUTSEL pin pull-up disable
        Uint16 ANALOGRESETn : 1; // 30    Status of ANALOGRESETn Bit
        Uint16 rsvd7 : 1;        // 31    reserved
};

union DEVICECNF_REG {
        Uint32 all;
        struct DEVICECNF_BITS bit;
};

struct PARTID_BITS {                 // bits description
        Uint16 PARTNO : 8;           // 7:0    Part Number Register
        Uint16 PARTTYPE : 8;         // 15:8   Part Type Register
};

union PARTID_REG {
        Uint16 all;
        struct PARTID_BITS bit;
};

struct CCNF0_BITS {                 // bits description
        Uint16 HRPWM : 1;           // 0     HRPWM Module Configuration
        Uint16 rsvd1 : 3;           // 3:1   Reserved
        Uint16 I2C : 1;             // 4     I2C Module Configuration
        Uint16 rsvd2 : 3;           // 7:5   Reserved
        Uint16 SPI : 1;             // 8     SPI Module Configuration
        Uint16 rsvd3 : 1;           // 9     Reserved
        Uint16 SCI : 1;             // 10    SCI Module Configuration
        Uint16 rsvd4 : 1;           // 11    Reserved
        Uint16 MCBSP : 1;           // 12    McBSP Module Configuration
        Uint16 rsvd5 : 3;           // 15:13 Reserved
        Uint16 rsvd6 : 16;          // 31:16 Reserved
};

union CCNF0_REG {
        Uint32 all;
        struct CCNF0_BITS bit;
};

struct CCNF1_BITS {                // bits description
        Uint16 EPWM1 : 1;          // 0     EPWM1 Module Configuration
        Uint16 EPWM2 : 1;          // 1     EPWM2 Module Configuration
        Uint16 EPWM3 : 1;          // 2     EPWM3 Module Configuration
        Uint16 EPWM4 : 1;          // 3     EPWM4 Module Configuration
        Uint16 EPWM5 : 1;          // 4     EPWM5 Module Configuration
        Uint16 EPWM6 : 1;          // 5     EPWM6 Module Configuration
        Uint16 EPWM7 : 1;          // 6     EPWM7 Module Configuration
        Uint16 EPWM8 : 1;          // 7     EPWM8 Module Configuration
        Uint16 ECAP1 : 1;          // 8     ECAP1 Module Configuration
        Uint16 ECAP2 : 1;          // 9     ECAP2 Module Configuration
        Uint16 ECAP3 : 1;          // 10    ECAP3 Module Configuration
        Uint16 ECAP4 : 1;          // 11    ECAP4 Module Configuration
        Uint16 ECAP5 : 1;          // 12    ECAP5 Module Configuration
        Uint16 ECAP6 : 1;          // 13    ECAP6 Module Configuration
        Uint16 EQEP1 : 1;          // 14    EQEP1 Module Configuration
        Uint16 EQEP2 : 1;          // 15    EQEP2 Module Configuration
        Uint16 rsvd1 : 16;         // 31:16 Reserved
};

union CCNF1_REG {
        Uint32 all;
        struct CCNF1_BITS bit;
};

struct CCNF2_BITS {                // bits description
        Uint16 EPWM9 : 1;          // 0     EPWM9 Module Configuration
        Uint16 rsvd1 : 7;          // 7:1   Reserved
        Uint16 EQEP3 : 1;          // 8     EQEP3 Module Configuration
        Uint16 rsvd2 : 7;          // 15:9  Reserved
        Uint16 rsvd3 : 16;         // 31:16 Reserved
};

union CCNF2_REG {
        Uint32 all;
        struct CCNF2_BITS bit;
};

struct CCNF3_BITS {                // bits description
        Uint16 rsvd1 : 11;         // 10:0  Reserved
        Uint16 C28DMA : 1;         // 11    C28 DMA Module Configuration
        Uint16 rsvd2 : 4;          // 15:12 Reserved
        Uint16 rsvd3 : 16;         // 31:16 Reserved
};

union CCNF3_REG {
        Uint32 all;
        struct CCNF3_BITS bit;
};

struct CCNF4_BITS {                  // bits description
        Uint16 FLASH : 3;            // 2:0   C28 Flash Size Configuration
        Uint16 rsvd1 : 5;            // 7:3   Reserved
        Uint16 rsvd2 : 1;            // 8     Reserved
        Uint16 rsvd3 : 1;            // 9     Reserved
        Uint16 rsvd4 : 6;            // 15:10 Reserved
        Uint16 C28HWBIST : 1;        // 16    C28 HW BIST Configuration
        Uint16 rsvd5 : 15;           // 31:17 Reserved
};

union CCNF4_REG {
        Uint32 all;
        struct CCNF4_BITS bit;
};

struct MEMCNF_BITS {       // bits description
        Uint16 S0 : 1;     // 0     S0 Shared Memory Configuration
        Uint16 S1 : 1;     // 1     S1 Shared Memory Configuration
        Uint16 S2 : 1;     // 2     S2 Shared Memory Configuration
        Uint16 S3 : 1;     // 3     S3 Shared Memory Configuration
        Uint16 S4 : 1;     // 4     S4 Shared Memory Configuration
        Uint16 S5 : 1;     // 5     S5 Shared Memory Configuration
        Uint16 S6 : 1;     // 6     S6 Shared Memory Configuration
        Uint16 S7 : 1;     // 7     S7 Shared Memory Configuration
        Uint16 rsvd1 : 8;  // 15:8  Reserved
        Uint16 rsvd2 : 16; // 31:16 Reserved
};

union MEMCNF_REG {
        Uint32 all;
        struct MEMCNF_BITS bit;
};

struct DEV_EMU_REGS {
        union    DEVICECNF_REG DEVICECNF;     // Device Configuration
        union    PARTID_REG PARTID;           // Device ID Register
        Uint16 REVID;                         // Revision Register
        Uint16 rsvd1[2];                      // Reserved
        union    CCNF0_REG CCNF0;             // C28 Peripheral Configuration 0
        union    CCNF1_REG CCNF1;             // C28 Peripheral Configuration 1
        union    CCNF2_REG CCNF2;             // C28 Peripheral Configuration 2
        union    CCNF3_REG CCNF3;             // C28 Peripheral Configuration 3
        union    CCNF4_REG CCNF4;             // C28 Flash and HWBIST
                                              // Configuration Register
        Uint16 rsvd2[48];                     // Reserved
        union    MEMCNF_REG MEMCNF;           // M3 Memory Configuration
                                              // Register
};

//---------------------------------------------------------------------------
// Devemu External References & Function Declarations:
extern volatile struct DEV_EMU_REGS DevEmuRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_DEVEMU_H definition


