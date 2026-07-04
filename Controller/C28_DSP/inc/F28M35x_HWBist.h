//###########################################################################
// FILE:   F28M35x_HWBist.h
// TITLE:  F28M35x Device HW Bist Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_HWBIST_H
#define F28M35x_HWBIST_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// HWBIST Individual Register Bit Definitions:

struct CSTCGCR0_BITS {                  // bits description
        Uint16 BISTDISABLE : 4;         // 3:0   HW BIST Disable
        Uint16 rsvd1 : 12;              // 15:4  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union CSTCGCR0_REG {
        Uint32 all;
        struct CSTCGCR0_BITS bit;
};

struct CSTCGCR2_BITS {                 // bits description
        Uint16 MACROSFG : 16;          // 15:0  HW BIST Disable
        Uint16 rsvd1 : 16;             // 31:16 Reserved
};

union CSTCGCR2_REG {
        Uint32 all;
        struct CSTCGCR2_BITS bit;
};

struct CSTCGCR3_BITS {                  // bits description
        Uint16 ISL : 4;                 // 3:0   Interrupt Logging Start
        Uint16 rsvd1 : 12;              // 15:4  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union CSTCGCR3_REG {
        Uint32 all;
        struct CSTCGCR3_BITS bit;
};

struct CSTCGCR4_BITS {                 // bits description
        Uint16 BISTGO : 4;             // 3:0   BIST Start
        Uint16 rsvd1 : 12;             // 15:4  Reserved
        Uint16 rsvd2 : 16;             // 31:16 Reserved
};

union CSTCGCR4_REG {
        Uint32 all;
        struct CSTCGCR4_BITS bit;
};

struct CSTCGCR5_BITS {                  // bits description
        Uint16 RESTART : 4;             // 3:0   Restart Enable
        Uint16 rsvd1 : 12;              // 15:4  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union CSTCGCR5_REG {
        Uint32 all;
        struct CSTCGCR5_BITS bit;
};

struct CSTCGCR6_BITS {                  // bits description
        Uint16 COV : 2;                 // 1:0   Coverage
        Uint16 rsvd1 : 14;              // 15:2  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union CSTCGCR6_REG {
        Uint32 all;
        struct CSTCGCR6_BITS bit;
};

struct CSTCGCR7_BITS {                  // bits description
        Uint16 MCL : 8;                 // 7:0   MAX Chain Length
        Uint16 DC : 4;                  // 11:8  Dead Cycles
        Uint16 NP : 4;                  // 15:12 Number of Pipeline Stages
        Uint16 PST : 2;                 // 17:16 Pattern Set Type
        Uint16 SCO : 2;                 // 19:18 Shift Clock Division
        Uint16 rsvd1 : 12;              // 31:20 Reserved
};

union CSTCGCR7_REG {
        Uint32 all;
        struct CSTCGCR7_BITS bit;
};

struct CSTCGCR8_BITS {                 // bits description
        Uint16 MCPC : 16;              // 15:0  MISR Compare Pattern Count
        Uint16 rsvd1 : 16;             // 31:16 Reserved
};

union CSTCGCR8_REG {
        Uint32 all;
        struct CSTCGCR8_BITS bit;
};

struct CSTCPCNT_BITS {                  // bits description
        Uint16 PCNT95 : 16;             // 15:0  Patterns for 95% Coverage
        Uint16 PCNT99 : 16;             // 31:16 Patterns for 99% Coverage
};

union CSTCPCNT_REG {
        Uint32 all;
        struct CSTCPCNT_BITS bit;
};

struct CSTCCONFIG_BITS {                // bits description
        Uint16 CFGDONE : 4;             // 3:0   Configuration Done
        Uint16 rsvd1 : 12;              // 15:4  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union CSTCCONFIG_REG {
        Uint32 all;
        struct CSTCCONFIG_BITS bit;
};

struct CSTCSADDR_BITS {                 // bits description
        Uint16 SAPAT : 16;              // 15:0  Pattern ROM Start Address
        Uint16 SAMISR : 16;             // 31:16 MISR ROM Start Address
};

union CSTCSADDR_REG {
        Uint32 all;
        struct CSTCSADDR_BITS bit;
};

struct CSTCTEST_BITS {                   // bits description
        Uint16 TESTTO : 4;               // 3:0   Test Timeout
        Uint16 TESTACMPFAIL : 4;         // 7:4   Test Analog System MISR Compare
                                         //       Fail
        Uint16 TESTNMI : 4;              // 11:8  Test NMI
        Uint16 TEST1 : 4;                // 15:12 Test Bits
        Uint16 TEST2 : 16;               // 31:16 Test Bits
};

union CSTCTEST_REG {
        Uint32 all;
        struct CSTCTEST_BITS bit;
};

struct CSTCCRD_BITS {                   // bits description
        Uint16 RESTOREDONE : 5;         // 4:0   Context Store Done
        Uint16 rsvd1 : 11;              // 15:5  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union CSTCCRD_REG {
        Uint32 all;
        struct CSTCCRD_BITS bit;
};

struct CSTCGSTAT_BITS {                  // bits description
        Uint16 BISTDONE : 1;             // 0     HW BIST Complete
        Uint16 MACRODONE : 1;            // 1     Macro Test Slot Complete
        Uint16 NMI : 1;                  // 2     Exit due to NMI
        Uint16 BISTFAIL : 1;             // 3     HW BIST Failure
        Uint16 INTCMPF : 1;              // 4     Intermediate Comparison Failure
        Uint16 TOFAIL : 1;               // 5     Timeout Failure
        Uint16 rsvd1 : 10;               // 15:6  Reserved
        Uint16 rsvd2 : 16;               // 31:16 Reserved
};

union CSTCGSTAT_REG {
        Uint32 all;
        struct CSTCGSTAT_BITS bit;
};

struct CSTCCPCR_BITS {                 // bits description
        Uint16 PATNT : 16;             // 15:0  Current Pattern Count
        Uint16 rsvd1 : 16;             // 31:16 Reserved
};

union CSTCCPCR_REG {
        Uint32 all;
        struct CSTCCPCR_BITS bit;
};

struct CSTCCADDR_BITS {                  // bits description
        Uint16 PATADDR : 16;             // 15:0  Current Pattern ROM Address
        Uint16 MISRADDR : 16;            // 31:16 Current MISR ROM Address
};

union CSTCCADDR_REG {
        Uint32 all;
        struct CSTCCADDR_BITS bit;
};

struct HWBIST_REGS {
        union    CSTCGCR0_REG CSTCGCR0;       // STC Global Control Register 0
        Uint32 CSTCGCR1;                      // STC Global Control Register 1
        union    CSTCGCR2_REG CSTCGCR2;       // STC Global Control Register 2
        union    CSTCGCR3_REG CSTCGCR3;       // STC Global Control Register 3
        union    CSTCGCR4_REG CSTCGCR4;       // STC Global Control Register 4
        union    CSTCGCR5_REG CSTCGCR5;       // STC Global Control Register 5
        union    CSTCGCR6_REG CSTCGCR6;       // STC Global Control Register 6
        union    CSTCGCR7_REG CSTCGCR7;       // STC Global Control Register 7
        union    CSTCGCR8_REG CSTCGCR8;       // STC Global Control Register 8
        union    CSTCPCNT_REG CSTCPCNT;       // STC Pattern Count Register
        union    CSTCCONFIG_REG CSTCCONFIG;   // STC Registers Configuration
                                              // Status
        union    CSTCSADDR_REG CSTCSADDR;     // STC ROM Start Address
        union    CSTCTEST_REG CSTCTEST;       // STC HW BIST Test Register
        Uint32 CSTCRET;                       // STC Return PC Address
        union    CSTCCRD_REG CSTCCRD;         // STC Context Restore Done
                                              // Register
        Uint32 rsvd1;                         // Reserved
        union    CSTCGSTAT_REG CSTCGSTAT;     // STC Global Status Register
        Uint32 rsvd2;                         // Reserved
        union    CSTCCPCR_REG CSTCCPCR;       // STC Current Pattern Count
                                              // Register
        union    CSTCCADDR_REG CSTCCADDR;     // STC Current ROM Address
                                              // Register
        Uint32 CSTCMISR3;                     // MISR3 Result Register
        Uint32 CSTCMISR2;                     // MISR2 Result Register
        Uint32 CSTCMISR1;                     // MISR1 Result Register
        Uint32 CSTCMISR0;                     // MISR0 Result Register
};

//---------------------------------------------------------------------------
// HWBIST External References & Function Declarations:
extern volatile struct HWBIST_REGS HWBistRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_HWBIST_H definition


