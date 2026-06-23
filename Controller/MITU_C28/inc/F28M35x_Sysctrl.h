//###########################################################################
// FILE:   F28M35x_Sysctrl.h
// TITLE:  F28M35x Device SYSCTRL Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_SYSCTRL_H
#define F28M35x_SYSCTRL_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// SYSCTRL Individual Register Bit Definitions:

struct CLKCTL_BITS {                    // bits description
        Uint16 rsvd1 : 3;               // 2:0    Reserved
        Uint16 TMR2CLKSRCSEL : 2;       // 4:3    CPU Timer 2 Clock Source
                                        //        Select Bit
        Uint16 TMR2CLKPRESCALE : 3;     // 7:5    CPU Timer 2 Clock Pre-Scale
                                        //        Value
        Uint16 rsvd2 : 8;               // 15:8   Reserved
};

union CLKCTL_REG {
        Uint16 all;
        struct    CLKCTL_BITS bit;
};

struct PCLKCR0_BITS {                   // bits description
        Uint16 HRPWMENCLK : 1;          // 0     HRPWM Clock Enable
        Uint16 rsvd1 : 1;               // 1     Reserved
        Uint16 TBCLKSYNC : 1;           // 2     EPWM Clock Sync
        Uint16 rsvd2 : 1;               // 3     Reserved
        Uint16 I2CAENCLK : 1;           // 4     I2C-A Clock Enable
        Uint16 rsvd3 : 3;               // 7:5   Reserved
        Uint16 SPIAENCLK : 1;           // 8     SPI-A Clock Enable
        Uint16 rsvd4 : 1;               // 9     Reserved
        Uint16 SCIAENCLK : 1;           // 10    SCI-A Clock Enable
        Uint16 rsvd5 : 1;               // 11    Reserved
        Uint16 MCBSPAENCLK : 1;         // 12    McBSP-A Clock Enable
        Uint16 rsvd6 : 3;               // 15:13 Reserved
};

union PCLKCR0_REG {
        Uint16 all;
        struct PCLKCR0_BITS bit;
};

struct PCLKCR1_BITS {                  // bits description
        Uint16 EPWM1ENCLK : 1;         // 0    EPWM1 Clock Enable
        Uint16 EPWM2ENCLK : 1;         // 1    EPWM2 Clock Enable
        Uint16 EPWM3ENCLK : 1;         // 2    EPWM3 Clock Enable
        Uint16 EPWM4ENCLK : 1;         // 3    EPWM4 Clock Enable
        Uint16 EPWM5ENCLK : 1;         // 4    EPWM5 Clock Enable
        Uint16 EPWM6ENCLK : 1;         // 5    EPWM6 Clock Enable
        Uint16 EPWM7ENCLK : 1;         // 6    EPWM7 Clock Enable
        Uint16 EPWM8ENCLK : 1;         // 7    EPWM8 Clock Enable
        Uint16 ECAP1ENCLK : 1;         // 8    ECAP1 Clock Enable
        Uint16 ECAP2ENCLK : 1;         // 9    ECAP2 Clock Enable
        Uint16 ECAP3ENCLK : 1;         // 10   ECAP3 Clock Enable
        Uint16 ECAP4ENCLK : 1;         // 11   ECAP4 Clock Enable
        Uint16 ECAP5ENCLK : 1;         // 12   ECAP5 Clock Enable
        Uint16 ECAP6ENCLK : 1;         // 13   ECAP6 Clock Enable
        Uint16 EQEP1ENCLK : 1;         // 14   EQEP1 Clock Enable
        Uint16 EQEP2ENCLK : 1;         // 15   EQEP2 Clock Enable
};

union PCLKCR1_REG {
        Uint16 all;
        struct PCLKCR1_BITS bit;
};

struct PCLKCR2_BITS {                  // bits description
        Uint16 EPWM9ENCLK : 1;         // 0    EPWM9 Clock Enable
        Uint16 rsvd1 : 7;              // 7:1  Reserved
        Uint16 EQEP3ENCLK : 1;         // 8    EQEP3 Clock Enable
        Uint16 rsvd2 : 7;              // 15:9 Reserved
};

union PCLKCR2_REG {
        Uint16 all;
        struct PCLKCR2_BITS bit;
};

struct PCLKCR3_BITS {                        // bits description
        Uint16 rsvd1 : 8;                    // 7:0   Reserved
        Uint16 CPUTIMER0ENCLK : 1;           // 8     B.C28 CPU Timer0 Clock
                                             //       Enable
        Uint16 CPUTIMER1ENCLK : 1;           // 9     B.C28 CPU Timer1 Clock
                                             //       Enable
        Uint16 CPUTIMER2ENCLK : 1;           // 10    B.C28 CPU Timer2 Clock
                                             //       Enable
        Uint16 DMAENCLK : 1;                 // 11    B.C28 DMA Clock Enable
        Uint16 rsvd2 : 4;                    // 15:12 Reserved
};

union PCLKCR3_REG {
        Uint16 all;
        struct PCLKCR3_BITS bit;
};

struct CWIR_BITS {				// bits description
		Uint16 EMU0: 1;			// 0 Latched State of EMU0 pin
		Uint16 EMU1: 1;			// 1 Latched State of EMU1 pin
		Uint16 SAMPLE: 1;		// 2 Re-sample EMU0 & EMU1 pins
		Uint16 rsvd1: 13;		// 15:3 Reserved
};

union CWIR_REG{
		Uint16 all;
		struct CWIR_BITS bit;
};

struct LOSPCP_BITS {               // bits description
        Uint16 LSPCLK : 3;         // 2:0    Low Speed Clock Prescaler
        Uint16 rsvd1 : 13;         // 15:3   Reserved
};

union LOSPCP_REG {
        Uint16 all;
        struct LOSPCP_BITS bit;
};

struct LPMCR0_BITS {                    // bits description
        Uint16 LPM : 2;                 // 1:0    Low Power Mode
        Uint16 QUALSTDBY : 6;           // 7:2    Number of Qual Cycles
        Uint16 rsvd1 : 8;               // 15:8   Reserved
};

union LPMCR0_REG {
        Uint16 all;
        struct LPMCR0_BITS bit;
};

struct CRESC_BITS {
        Uint16 rsvd1 : 2;               // 1:0   Reserved
        Uint16 XRSn : 1;                // 2     External Reset
        Uint16 VMON : 1;                // 3     VMON Reset
        Uint16 C28_NMIRSN : 1;          // 4     C28 NMI Watchdog Reset
        Uint16 rsvd2 : 11;              // 15:5  Reserved
};

union CRESC_REG {
        Uint16 all;
        struct CRESC_BITS bit;
};

struct SYS_CTRL_REGS {
        Uint16 rsvd1[2];                  // Reserved
        union    CLKCTL_REG CLKCTL;       // CPU Timer 2 Clock Configuratioin
                                          // Register
        Uint16   rsvd2[6];                // Reserved
        union    PCLKCR2_REG PCLKCR2;     // Peripheral Clock Control Register2
        Uint16   rsvd3;                   // Reserved
        union    LOSPCP_REG LOSPCP;       // Low Speed Clock Prescaler
        union    PCLKCR0_REG PCLKCR0;     // Peripheral Clock Control Register0
        union    PCLKCR1_REG PCLKCR1;     // Peripheral Clock Control Register1
        union    LPMCR0_REG LPMCR0;       // C28 LPM Control Register0
        Uint16   rsvd4;                   // Reserved
        union    PCLKCR3_REG PCLKCR3;     // Peripheral Clock Control Register3
        Uint16   rsvd5[10];               // Reserved
		union    CWIR_REG CWIR;			  // C28 Wait-In-Reset Register
        union    CRESC_REG CRESC;         // C28 Reset Cause Register
};

struct SYSPLLCTL_BITS {                   // bits description
        Uint16 SPLLEN : 1;                // 0     M3 System PLL Enable
        Uint16 SPLLCLKEN : 1;             // 1     M3 System PLL Clock Enable
        Uint16 rsvd1 : 7;                 // 8:2   Reserved
        Uint16 rsvd2 : 7;                 // 15:9  Reserved
        Uint16 rsvd3 : 16;                // 31:16 Reserved
};

union SYSPLLCTL_REG {
        Uint32 all;
        struct SYSPLLCTL_BITS bit;
};

struct SYSPLLMULT_BITS {               // bits description
        Uint16 SPLLIMULT : 7;          // 6:0   M3 System PLL Integer
                                       //       Multiplier
        Uint16 rsvd1 : 1;              // 7     Reserved
        Uint16 SPLLFMULT : 2;          // 9:8   M3 System PLL Fractional
                                       //       Multiplier
        Uint16 rsvd2 : 6;              // 15:10 Reserved
        Uint16 rsvd3 : 16;             // 31:16 Reserved
};

union SYSPLLMULT_REG {
        Uint32 all;
        struct SYSPLLMULT_BITS bit;
};

struct SYSPLLSTS_BITS {                 // bits description
        Uint16 SYSPLLLOCKS : 1;         // 0     M3 System PLL Lock Status
        Uint16 SPLLSLIPS : 1;           // 1     M3 System PLL Out of Range
                                        //       Status
        Uint16 rsvd1 : 14;              // 15:2  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union SYSPLLSTS_REG {
        Uint32 all;
        struct SYSPLLSTS_BITS bit;
};

struct SYSDIVSEL_BITS {                  // bits description
        Uint16 SYSDIVSEL : 2;            // 1:0   System Clock Divider
        Uint16 rsvd1 : 4;                // 5:2   Reserved
        Uint16 SPR_FAMOUNTL : 10;        // 15:6  System PLL Modulation
                                         //       Fractional Part (Low)
        Uint16 SPR_FAMOUNTH : 5;         // 20:16 System PLL Modulation
                                         //       Fractional Part (High)
        Uint16 SPR_IAMOUNT : 11;         // 31:21 System PLL Modulation Integer
                                         //       Part
};

union SYSDIVSEL_REG {
        Uint32 all;
        struct SYSDIVSEL_BITS bit;
};

struct M3PLL_REGS {
        union    SYSPLLCTL_REG SYSPLLCTL;     // M3 System PLL Configuration
                                              // Register
        union    SYSPLLMULT_REG SYSPLLMULT;   // M3 System PLL Multiplier
        union    SYSPLLSTS_REG SYSPLLSTS;     // M3 System PLL Lock Status
        union    SYSDIVSEL_REG SYSDIVSEL;     // M3 System Clock Divider
};

struct CSMCR_BITS {                   // bits description
    Uint16    rsvd1:3;                // 2:0 Reserved
    Uint16    CSM_ALLZERO:1;          // 3 CSM loaded pswd contains all 0's
    Uint16    CSM_ALLONE:1;           // 4 CSM loaded pswd contains all 1's
    Uint16    CSM_MATCH:1;            // 5 CSM loaded pswd and key register match
    Uint16    CSM_ARMED:1;            // 6 Code security is armed
    Uint16    ECSL_ALLZERO:1;         // 7 ECSL loaded pswd contains all 0's
    Uint16    ECSL_ALLONE:1;          // 8 ECSL loaded pswd contains all 1's
    Uint16    ECSL_MATCH:1;           // 9 ECSL loaded pswd and key register match
    Uint16    ECSL_ARMED:1;           // 10 Code security is armed
    Uint16    rsvd2:4;                // 14:11    Reserved
    Uint16    FORCESEC:1;             // 15    Force secure control bit
};

union CSMCR_REG {
        Uint16 all;
        struct CSMCR_BITS bit;
};

struct EXEONLYR_BITS {            // bits description
        Uint16 EXEONLY_SECTN : 1; // 0     Flash Sector N Execute Only Enabled
        Uint16 EXEONLY_SECTM : 1; // 1     Flash Sector M Execute Only Enabled
        Uint16 EXEONLY_SECTL : 1; // 2     Flash Sector L Execute Only Enabled
        Uint16 EXEONLY_SECTK : 1; // 3     Flash Sector K Execute Only Enabled
        Uint16 EXEONLY_SECTJ : 1; // 4     Flash Sector J Execute Only Enabled
        Uint16 EXEONLY_SECTI : 1; // 5     Flash Sector I Execute Only Enabled
        Uint16 EXEONLY_SECTH : 1; // 6     Flash Sector H Execute Only Enabled
        Uint16 EXEONLY_SECTG : 1; // 7     Flash Sector G Execute Only Enabled
        Uint16 EXEONLY_SECTF : 1; // 8     Flash Sector F Execute Only Enabled
        Uint16 EXEONLY_SECTE : 1; // 9     Flash Sector E Execute Only Enabled
        Uint16 EXEONLY_SECTD : 1; // 10    Flash Sector D Execute Only Enabled
        Uint16 EXEONLY_SECTC : 1; // 11    Flash Sector C Execute Only Enabled
        Uint16 EXEONLY_SECTB : 1; // 12    Flash Sector B Execute Only Enabled
        Uint16 EXEONLY_SECTA : 1; // 13    Flash Sector A Execute Only Enabled
        Uint16 rsvd1 : 2;         // 15:14 Reserved
};

union EXEONLYR_REG {
        Uint16 all;
        struct EXEONLYR_BITS bit;
};

struct CSM_REGS {
        Uint32 CSMKEY0;                             // KEY reg bits 31-0
        Uint32 CSMKEY1;                             // KEY reg bits 63-32
        Uint32 CSMKEY2;                             // KEY reg bits 95-64
        Uint32 CSMKEY3;                             // KEY reg bits 127-96
        Uint16 rsvd1[7];                            // Reserved
        union  CSMCR_REG CSMCR;                     // C28  Status and Control
                                                    // Register
        Uint32 ECSLKEY0;                            // C28  ECSL Key 0 31-0
        Uint32 ECSLKEY1;                            // C28  ECSL Key 1 63-32
        Uint16 rsvd2;                               // Reserved
        union    EXEONLYR_REG EXEONLYR;             // C28  Execute Only
                                                    // Register (Read)
};

struct CSM_PWL {
        Uint32 PSWD0;           // PSWD bits 31-0
        Uint32 PSWD1;           // PSWD bits 63-32
        Uint32 PSWD2;           // PSWD bits 95-64
        Uint32 PSWD3;           // PSWD bits 127-96
};

struct ECSL_PWL {
        Uint32 PSWD0;           // PSWD bits 31-0
        Uint32 PSWD1;           // PSWD bits 63-32
};

struct FLASH_EXE_ONLY {
        Uint32 FLASH_EXE_ONLY_REG;       // Flash Execute Only Regsiter
};

//---------------------------------------------------------------------------
// Sysctrl External References & Function Declarations:
extern volatile struct SYS_CTRL_REGS SysCtrlRegs;
extern volatile struct M3PLL_REGS M3PllRegs;
extern volatile struct CSM_REGS CsmRegs;
extern volatile struct CSM_PWL CsmPwl;
extern volatile struct ECSL_PWL EcslPwl;
extern volatile struct FLASH_EXE_ONLY FlashExeOnly;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_SYSCTRL_H definition


