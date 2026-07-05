//###########################################################################
// FILE:   F28M35x_Ram.h
// TITLE:  F28M35x Device RAM Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_RAM_H
#define F28M35x_RAM_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// RAM Individual Register Bit Definitions:

struct CLxDRCR1_BITS {                  // bits description
        Uint16 FETCHPROTL0 : 1;         // 0     Fetch Protection for C28
                                        //       Dedicated RAM L0
        Uint16 rsvd1 : 1;               // 1     Reserved
        Uint16 CPUWRPROTL0 : 1;         // 2     CPU WR Protection for C28
                                        //       Dedicated RAM L0
        Uint16 rsvd2 : 5;               // 7:3   Reserved
        Uint16 FETCHPROTL1 : 1;         // 8     Fetch Protection for C28
                                        //       Dedicated RAM L1
        Uint16 rsvd3 : 1;               // 9     Reserved
        Uint16 CPUWRPROTL1 : 1;         // 10    CPU WR Protection for C28
                                        //       Dedicated RAM L1
        Uint16 rsvd4 : 5;               // 15:11 Reserved
        Uint16 rsvd5 : 16;              // 31:16 Reserved
};

union CLxDRCR1_REG {
        Uint32 all;
        struct CLxDRCR1_BITS bit;
};

struct CLxSRCR1_BITS {                  // bits description
        Uint16 rsvd1 : 16;              // 15:0  Reserved
        Uint16 FETCHPROTL2 : 1;         // 16    Fetch Protection for C28 Shared
                                        //       (CPU and DMA) RAM L2
        Uint16 DMAWRPROTL2 : 1;         // 17    DMA WR Protection for C28
                                        //       Shared (CPU and DMA) RAM L2
        Uint16 CPUWRPROTL2 : 1;         // 18    CPU WR Protection for C28
                                        //       Shared (CPU and DMA) RAM L2
        Uint16 rsvd2 : 5;               // 23:19 Reserved
        Uint16 FETCHPROTL3 : 1;         // 24    Fetch Protection for C28 Shared
                                        //       (CPU and DMA) RAM L3
        Uint16 DMAWRPROTL3 : 1;         // 25    DMA WR Protection for C28
                                        //       Shared (CPU and DMA) RAM L3
        Uint16 CPUWRPROTL3 : 1;         // 26    CPU WR Protection for C28
                                        //       Shared (CPU and DMA) RAM L3
        Uint16 rsvd3 : 5;               // 31:27 Reserved
};

union CLxSRCR1_REG {
        Uint32 all;
        struct CLxSRCR1_BITS bit;
};

struct CSxMSEL_BITS {              // bits description
        Uint16 S0MSEL : 1;         // 0     Master Select Status for Shared RAM
                                   //       S0
        Uint16 S1MSEL : 1;         // 1     Master Select Status for Shared RAM
                                   //       S1
        Uint16 S2MSEL : 1;         // 2     Master Select Status for Shared RAM
                                   //       S2
        Uint16 S3MSEL : 1;         // 3     Master Select Status for Shared RAM
                                   //       S3
        Uint16 S4MSEL : 1;         // 4     Master Select Status for Shared RAM
                                   //       S4
        Uint16 S5MSEL : 1;         // 5     Master Select Status for Shared RAM
                                   //       S5
        Uint16 S6MSEL : 1;         // 6     Master Select Status for Shared RAM
                                   //       S6
        Uint16 S7MSEL : 1;         // 7     Master Select Status for Shared RAM
                                   //       S7
        Uint16 rsvd1 : 8;          // 15:8  Reserved
        Uint16 rsvd2 : 16;         // 31:16 Reserved
};

union CSxMSEL_REG {
        Uint32 all;
        struct CSxMSEL_BITS bit;
};

struct CSxSRCR1_BITS {                  // bits description
        Uint16 FETCHPROTS0 : 1;         // 0     Fetch Protection for Shared RAM
                                        //       S0
        Uint16 DMAWRPROTS0 : 1;         // 1     DMA WR Protection for Shared RAM
                                        //       S0
        Uint16 CPUWRPROTS0 : 1;         // 2     CPU WR Protection for Shared RAM
                                        //       S0
        Uint16 rsvd1 : 5;               // 7:3   Reserved
        Uint16 FETCHPROTS1 : 1;         // 8     Fetch Protection for Shared RAM
                                        //       S1
        Uint16 DMAWRPROTS1 : 1;         // 9     DMA WR Protection for Shared RAM
                                        //       S1
        Uint16 CPUWRPROTS1 : 1;         // 10    CPU WR Protection for Shared
                                        //       RAM S1
        Uint16 rsvd2 : 5;               // 15:11 Reserved
        Uint16 FETCHPROTS2 : 1;         // 16    Fetch Protection for Shared RAM
                                        //       S2
        Uint16 DMAWRPROTS2 : 1;         // 17    DMA WR Protection for Shared
                                        //       RAM S2
        Uint16 CPUWRPROTS2 : 1;         // 18    CPU WR Protection for Shared
                                        //       RAM S2
        Uint16 rsvd3 : 5;               // 23:19 Reserved
        Uint16 FETCHPROTS3 : 1;         // 24    Fetch Protection for Shared RAM
                                        //       S3
        Uint16 DMAWRPROTS3 : 1;         // 25    DMA WR Protection for Shared
                                        //       RAM S3
        Uint16 CPUWRPROTS3 : 1;         // 26    CPU WR Protection for Shared
                                        //       RAM S3
        Uint16 rsvd4 : 5;               // 31:27 Reserved
};

union CSxSRCR1_REG {
        Uint32 all;
        struct CSxSRCR1_BITS bit;
};

struct CSxSRCR2_BITS {                  // bits description
        Uint16 FETCHPROTS4 : 1;         // 0     Fetch Protection for Shared RAM
                                        //       S4
        Uint16 DMAWRPROTS4 : 1;         // 1     DMA WR Protection for Shared RAM
                                        //       S4
        Uint16 CPUWRPROTS4 : 1;         // 2     CPU WR Protection for Shared RAM
                                        //       S4
        Uint16 rsvd1 : 5;               // 7:3   Reserved
        Uint16 FETCHPROTS5 : 1;         // 8     Featch Protection for Shared RAM
                                        //       S5
        Uint16 DMAWRPROTS5 : 1;         // 9     DMA WR Protection for Shared RAM
                                        //       S5
        Uint16 CPUWRPROTS5 : 1;         // 10    CPU WR Protection for Shared
                                        //       RAM S5
        Uint16 rsvd2 : 5;               // 15:11 Reserved
        Uint16 FETCHPROTS6 : 1;         // 16    Fetch Protection for Shared RAM
                                        //       S6
        Uint16 DMAWRPROTS6 : 1;         // 17    DMA WR Protection for Shared
                                        //       RAM S6
        Uint16 CPUWRPROTS6 : 1;         // 18    CPU WR Protection for Shared
                                        //       RAM S6
        Uint16 rsvd3 : 5;               // 23:19 Reserved
        Uint16 FETCHPROTS7 : 1;         // 24    Fetch Protection for Shared RAM
                                        //       S7
        Uint16 DMAWRPROTS7 : 1;         // 25    DMA WR Protection for Shared
                                        //       RAM S7
        Uint16 CPUWRPROTS7 : 1;         // 26    CPU WR Protection for Shared
                                        //       RAM S7
        Uint16 rsvd4 : 5;               // 31:27 Reserved
};

union CSxSRCR2_REG {
        Uint32 all;
        struct CSxSRCR2_BITS bit;
};

struct CTOMMSGRCR_BITS {           // bits description
        Uint16 rsvd1 : 1;          // 0     Reserved
        Uint16 DMAWRPROT : 1;      // 1     DMA WR Protection for MSG RAM.
        Uint16 rsvd2 : 14;         // 15:2  Reserved
        Uint16 rsvd3 : 16;         // 31:16 Reserved
};

union CTOMMSGRCR_REG {
        Uint32 all;
        struct CTOMMSGRCR_BITS bit;
};

struct CRTESTINIT_BITS {                   // bits description
        Uint16 RAMINIT_M0 : 1;             // 0     RAM Init for M0
        Uint16 ECCPARTEST_M0 : 1;          // 1     RAM Test for M0
        Uint16 RAMINIT_M1 : 1;             // 2     RAM Init for M1
        Uint16 ECCPARTEST_M1 : 1;          // 3     RAM Test for M1
        Uint16 RAMINIT_CTOMMSGRAM : 1;     // 4     RAM Init for C28 to M3
                                           //       Message RAM
        Uint16 ECCPARTEST_CTOMMSGRAM : 1;  // 5     RAM Test for C28 to M3
                                           //       Message RAM
        Uint16 rsvd1 : 10;                 // 15:6  Reserved
        Uint16 rsvd2 : 16;                 // 31:16 Reserved
};

union CRTESTINIT_REG {
        Uint32 all;
        struct CRTESTINIT_BITS bit;
};

struct CLxRTESTINIT1_BITS {        // bits description
        Uint16 RAMINIT_L0 : 1;     // 0     RAM Init for L0 RAM
        Uint16 ECCPARTEST_L0 : 1;  // 1     RAM Test for L0 RAM
        Uint16 RAMINIT_L1 : 1;     // 2     RAM Init for L1 RAM
        Uint16 ECCPARTEST_L1 : 1;  // 3     RAM Test for L1 RAM
        Uint16 RAMINIT_L2 : 1;     // 4     RAM Init for L2 RAM
        Uint16 ECCPARTEST_L2 : 1;  // 5     RAM Test for L2 RAM
        Uint16 RAMINIT_L3 : 1;     // 6     RAM Init for L3 RAM
        Uint16 ECCPARTEST_L3 : 1;  // 7     RAM Test for L3 RAM
        Uint16 rsvd1 : 8;          // 15:8  Reserved
        Uint16 rsvd2 : 16;         // 31:16 Reserved
};

union CLxRTESTINIT1_REG {
        Uint32 all;
        struct CLxRTESTINIT1_BITS bit;
};

struct CSxRTESTINIT1_BITS {          // bits description
        Uint16 RAMINIT_S0 : 1;       // 0     RAM Init for Shared RAM S0
        Uint16 ECCPARTEST_S0 : 1;    // 1     RAM Test for Shared RAM S0
        Uint16 RAMINIT_S1 : 1;       // 2     RAM Init for Shared RAM S1
        Uint16 ECCPARTEST_S1 : 1;    // 3     RAM Test for Shared RAM S1
        Uint16 RAMINIT_S2 : 1;       // 4     RAM Init for Shared RAM S2
        Uint16 ECCPARTEST_S2 : 1;    // 5     RAM Test for Shared RAM S2
        Uint16 RAMINIT_S3 : 1;       // 6     RAM Init for Shared RAM S3
        Uint16 ECCPARTEST_S3 : 1;    // 7     RAM Test for Shared RAM S3
        Uint16 RAMINIT_S4 : 1;       // 8     RAM Init for Shared RAM S4
        Uint16 ECCPARTEST_4 : 1;     // 9     RAM Test for Shared RAM S4
        Uint16 RAMINIT_S5 : 1;       // 10    RAM Init for Shared RAM S5
        Uint16 ECCPARTEST_S5 : 1;    // 11    RAM Test for Shared RAM S5
        Uint16 RAMINIT_S6 : 1;       // 12    RAM Init for Shared RAM S6
        Uint16 ECCPARTEST_S6 : 1;    // 13    RAM Test for Shared RAM S6
        Uint16 RAMINIT_S7 : 1;       // 14    RAM Init for Shared RAM S7
        Uint16 ECCPARTEST_S7 : 1;    // 15    RAM Test for Shared RAM S7
        Uint16 rsvd1 : 16;           // 31:16 Reserved
};

union CSxRTESTINIT1_REG {
        Uint32 all;
        struct CSxRTESTINIT1_BITS bit;
};

struct CRINITDONE_BITS {                   // bits description
        Uint16 RAMINITDONE_M0 : 1;         // 0     RAM Test Complete for M0
        Uint16 rsvd1 : 1;                  // 1     Reserved
        Uint16 RAMINITDONE_M1 : 1;         // 2     RAM Test Complete for M1
        Uint16 rsvd2 : 1;                  // 3     Reserved
        Uint16 RAMINITDONE_CTOMMSGRAM : 1; // 4     RAM Test Complete for C28 to
                                           //       M3 Message RAM
        Uint16 rsvd3 : 11;                 // 15:5  Reserved
        Uint16 rsvd4 : 16;                 // 31:16 Reserved
};

union CRINITDONE_REG {
        Uint32 all;
        struct CRINITDONE_BITS bit;
};

struct CLxRINITDONE_BITS {         // bits description
        Uint16 RAMINITDONE_L0 : 1; // 0     RAM Test Complete for L0 RAM
        Uint16 rsvd1 : 1;          // 1     Reserved
        Uint16 RAMINITDONE_L1 : 1; // 2     RAM Test Complete for L1 RAM
        Uint16 rsvd2 : 1;          // 3     Reserved
        Uint16 RAMINITDONE_L2 : 1; // 4     RAM Test Complete for L2 RAM
        Uint16 rsvd3 : 1;          // 5     Reserved
        Uint16 RAMINITDONE_L3 : 1; // 6     RAM Test Complete for L3 RAM
        Uint16 rsvd4 : 9;          // 15:7  Reserved
        Uint16 rsvd5 : 16;         // 31:16 Reserved
};

union CLxRINITDONE_REG {
        Uint32 all;
        struct CLxRINITDONE_BITS bit;
};

struct CSxRINITDONE_BITS {         // bits description
        Uint16 RAMINITDONE_S0 : 1; // 0     RAM Test Complete For Shared RAM S0
        Uint16 rsvd1 : 1;          // 1     Reserved
        Uint16 RAMINITDONE_S1 : 1; // 2     RAM Test Complete For Shared RAM S1
        Uint16 rsvd2 : 1;          // 3     Reserved
        Uint16 RAMINITDONE_S2 : 1; // 4     RAM Test Complete For Shared RAM S2
        Uint16 rsvd3 : 1;          // 5     Reserved
        Uint16 RAMINITDONE_S3 : 1; // 6     RAM Test Complete For Shared RAM S3
        Uint16 rsvd4 : 1;          // 7     Reserved
        Uint16 RAMINITDONE_S4 : 1; // 8     RAM Test Complete For Shared RAM S4
        Uint16 rsvd5 : 1;          // 9     Reserved
        Uint16 RAMINITDONE_S5 : 1; // 10    RAM Test Complete For Shared RAM S5
        Uint16 rsvd6 : 1;          // 11    Reserved
        Uint16 RAMINITDONE_S6 : 1; // 12    RAM Test Complete For Shared RAM S6
        Uint16 rsvd7 : 1;          // 13    Reserved
        Uint16 RAMINITDONE_S7 : 1; // 14    RAM Test Complete For Shared RAM S7
        Uint16 rsvd8 : 1;          // 15    Reserved
        Uint16 rsvd9 : 16;         // 31:16 Reserved
};

union CSxRINITDONE_REG {
        Uint32 all;
        struct CSxRINITDONE_BITS bit;
};

struct CUEFLG_BITS {                 // bits description
        Uint16 C28CPURE : 1;         // 0     C28 CPU Uncorrectable Read Error
                                     //       Flag
        Uint16 C28DMARE : 1;         // 1     C28 DMA Uncorrectable Read Error
                                     //       Flag
        Uint16 rsvd1 : 14;           // 15:2  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CUEFLG_REG {
        Uint32 all;
        struct CUEFLG_BITS bit;
};

struct CUEFRC_BITS {                 // bits description
        Uint16 C28CPURE : 1;         // 0     C28 CPU Uncorrectable Read Error
                                     //       Force
        Uint16 C28DMARE : 1;         // 1     C28 DMA Uncorrectable Read Error
                                     //       Force
        Uint16 rsvd1 : 14;           // 15:2  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CUEFRC_REG {
        Uint32 all;
        struct CUEFRC_BITS bit;
};

struct CUECLR_BITS {                 // bits description
        Uint16 C28CPURE : 1;         // 0     C28 CPU Uncorrectable Read Error
                                     //       Clear
        Uint16 C28DMARE : 1;         // 1     C28 DMA Uncorrectable Read Error
                                     //       Clear
        Uint16 rsvd1 : 14;           // 15:2  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CUECLR_REG {
        Uint32 all;
        struct CUECLR_BITS bit;
};

struct CCEFLG_BITS {               // bits description
        Uint16 C28CEFLAG : 1;      // 0     C28 CPU corrected error count
                                   //       exceeded threshold flag.
        Uint16 rsvd1 : 15;         // 15:1  Reserved
        Uint16 rsvd2 : 16;         // 31:16 Reserved
};

union CCEFLG_REG {
        Uint32 all;
        struct CCEFLG_BITS bit;
};

struct CCEFRC_BITS {                 // bits description
        Uint16 C28CEFRC : 1;         // 0     C28 CPU corrected error count
                                     //       exceeded force flag.
        Uint16 rsvd1 : 15;           // 15:1  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CCEFRC_REG {
        Uint32 all;
        struct CCEFRC_BITS bit;
};

struct CCECLR_BITS {                 // bits description
        Uint16 C28CECLR : 1;         // 0     C28 CPU corrected error threshold
                                     //       exceeded error clear.
        Uint16 rsvd1 : 15;           // 15:1  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CCECLR_REG {
        Uint32 all;
        struct CCECLR_BITS bit;
};

struct CCEIE_BITS {                 // bits description
        Uint16 C28CEIE : 1;         // 0     C28 CPU Correctable Error
                                    //       Interrupt Enable.
        Uint16 rsvd1 : 15;          // 15:1  Reserved
        Uint16 rsvd2 : 16;          // 31:16 Reserved
};

union CCEIE_REG {
        Uint32 all;
        struct CCEIE_BITS bit;
};

struct CNMAVFLG_BITS {               // bits description
        Uint16 CPUFETCH : 1;         // 0     Non-master CPU Fetch Access
                                     //       Violation Flag
        Uint16 DMAWRITE : 1;         // 1     Non-master DMA Write Access
                                     //       Violation Flag
        Uint16 CPUWRITE : 1;         // 2     Non-master CPU Write Access
                                     //       Violation Flag
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CNMAVFLG_REG {
        Uint32 all;
        struct CNMAVFLG_BITS bit;
};

struct CNMAVFRC_BITS {               // bits description
        Uint16 CPUFETCH : 1;         // 0     Non-master CPU Fetch Access
                                     //       Violation Flag Force
        Uint16 DMAWRITE : 1;         // 1     Non-master DMA Write Access
                                     //       Violation Flag Force
        Uint16 CPUWRITE : 1;         // 2     Non-master CPU Write Access
                                     //       Violation Flag Force
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CNMAVFRC_REG {
        Uint32 all;
        struct CNMAVFRC_BITS bit;
};

struct CNMAVCLR_BITS {               // bits description
        Uint16 CPUFETCH : 1;         // 0     Non-master CPU Fetch Access
                                     //       Violation Flag Clear
        Uint16 DMAWRITE : 1;         // 1     Non-master DMA Write Access
                                     //       Violation Flag Clear
        Uint16 CPUWRITE : 1;         // 2     Non-master CPU Write Access
                                     //       Violation Flag Clear
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CNMAVCLR_REG {
        Uint32 all;
        struct CNMAVCLR_BITS bit;
};

struct CNMAVIE_BITS {                // bits description
        Uint16 CPUFETCH : 1;         // 0     Non-master CPU Fetch Access
                                     //       Violation Interrupt Enable
        Uint16 DMAWRITE : 1;         // 1     Non-master DMA Write Access
                                     //       Violation Interrupt Enable
        Uint16 CPUWRITE : 1;         // 2     Non-master CPU Write Access
                                     //       Violation Interrupt Enable
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CNMAVIE_REG {
        Uint32 all;
        struct CNMAVIE_BITS bit;
};

struct CMAVFLG_BITS {                // bits description
        Uint16 CPUFETCH : 1;         // 0     Master CPU Fetch Access Violation
                                     //       Flag Clear
        Uint16 DMAWRITE : 1;         // 1     Master DMA Write Access Violation
                                     //       Flag Clear
        Uint16 CPUWRITE : 1;         // 2     Master CPU Write Access Violation
                                     //       Flag Clear
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CMAVFLG_REG {
        Uint32 all;
        struct CMAVFLG_BITS bit;
};

struct CMAVFRC_BITS {                // bits description
        Uint16 CPUFETCH : 1;         // 0     Master CPU Fetch Access Violation
                                     //       Flag Force
        Uint16 DMAWRITE : 1;         // 1     Master DMA Write Access Violation
                                     //       Flag Force
        Uint16 CPUWRITE : 1;         // 2     Master CPU Write Access Violation
                                     //       Flag Force
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CMAVFRC_REG {
        Uint32 all;
        struct CMAVFRC_BITS bit;
};

struct CMAVCLR_BITS {                // bits description
        Uint16 CPUFETCH : 1;         // 0     Master CPU Fetch Access Violation
                                     //       Flag Clear
        Uint16 DMAWRITE : 1;         // 1     Master DMA Write Access Violation
                                     //       Flag Clear
        Uint16 CPUWRITE : 1;         // 2     Master CPU Write Access Violation
                                     //       Flag Clear
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CMAVCLR_REG {
        Uint32 all;
        struct CMAVCLR_BITS bit;
};

struct CMAVIE_BITS {                 // bits description
        Uint16 CPUFETCH : 1;         // 0     Master CPU Fetch Access Violation
                                     //       Flag Clear
        Uint16 DMAWRITE : 1;         // 1     Master DMA Write Access Violation
                                     //       Flag Clear
        Uint16 CPUWRITE : 1;         // 2     Master CPU Write Access Violation
                                     //       Flag Clear
        Uint16 rsvd1 : 13;           // 15:3  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union CMAVIE_REG {
        Uint32 all;
        struct CMAVIE_BITS bit;
};

struct RAM_REGS {
        union    CLxDRCR1_REG CLxDRCR1;               // C28 Lx DEDRAM Config
                                                      // Register 1
        Uint16 rsvd1[2];                              // Reserved
        union    CLxSRCR1_REG CLxSRCR1;               // C28 Lx SHRAM Config
                                                      // Register 1
        Uint16 rsvd2[2];                              // Reserved
        union    CSxMSEL_REG CSxMSEL;                 // C28 Sx SHRAM Master
                                                      // Select Register
        Uint16 rsvd3[2];                              // Reserved
        Uint16 rsvd4[2];                              // Reserved
        Uint16 rsvd5[2];                              // Reserved
        union    CSxSRCR1_REG CSxSRCR1;               // C28 Sx SHRAM Config
                                                      // Register 1
        union    CSxSRCR2_REG CSxSRCR2;               // C28 Sx SHRAM Config
                                                      // Register 2
        Uint16 rsvd6[2];                              // Reserved
        Uint16 rsvd7[2];                              // Reserved
        Uint16 rsvd8[2];                              // Reserved
        union    CTOMMSGRCR_REG CTOMMSGRCR;           // C28 to M3 Message RAM
                                                      // Config Register
        Uint16 rsvd9[2];                              // Reserved
        Uint16 rsvd10[2];                             // Reserved
        union    CRTESTINIT_REG CRTESTINIT;           // M0, M1 AND C28 to M3
                                                      // Message RAM TEST and
                                                      // INIT Register
        union    CLxRTESTINIT1_REG CLxRTESTINIT1;     // C28 Lx RAM_TEST and
                                                      // INIT Register 1
        Uint16 rsvd11[2];                             // Reserved
        union    CSxRTESTINIT1_REG CSxRTESTINIT1;     // C28 Sx RAM_TEST and
                                                      // INIT Register 1
        Uint16 rsvd12[2];                             // Reserved
        Uint16 rsvd13[2];                             // Reserved
        Uint16 rsvd14[2];                             // Reserved
        Uint16 rsvd15[2];                             // Reserved
        union    CRINITDONE_REG CRINITDONE;           // M0, M1AND C28T0M3
                                                      // MSGRAM INIT Done
                                                      // Register
        union    CLxRINITDONE_REG CLxRINITDONE;       // C28 Lx RAM_INIT_DONE
                                                      // Register1
        Uint16 rsvd16[2];                             // Reserved
        union    CSxRINITDONE_REG CSxRINITDONE;       // C28 Sx RAM_INIT_DONE
                                                      // Register
        Uint16 rsvd17[2];                             // Reserved
        Uint16 rsvd18[2];                             // Reserved
        Uint16 rsvd19[2];                             // Reserved
};

struct RAM_ERR_REGS {
        Uint32 CCUNCREADDR;                       // C28 CPU Uncorrectable Read
                                                  // Error Address
        Uint32 CDUNCREADDR;                       // C28 DMA Uncorrectable Read
                                                  // Error Address
        Uint32 CCPUCREADDR;                       // C28 CPU Corrected Read
                                                  // Error Address
        Uint32 CDMACREADD;                        // C28 DMA Corrected Read
                                                  // Error Address
        union    CUEFLG_REG CUEFLG;               // C28 Uncorrectable Error
                                                  // Flag Register
        union    CUEFRC_REG CUEFRC;               // C28 Uncorrectable Error
                                                  // Force Register
        union    CUECLR_REG CUECLR;               // C28 Uncorrectable Error
                                                  // Flag Clear Register
        Uint16 rsvd1[2];                          // Reserved
        Uint32 CCECNTR;                           // C28 Corrected Error Counter
                                                  // Register
        Uint32 CCETRES;                           // C28 Corrected Error
                                                  // Threshold Register
        union    CCEFLG_REG CCEFLG;               // C28 Corrected Error
                                                  // Threshold Exceeded Flag
                                                  // Register
        union    CCEFRC_REG CEFRC;                // C28 Corrected Error
                                                  // Threshold Exceeded Force
                                                  // Register
        union    CCECLR_REG CCECLR;               // C28 Corrected Error
                                                  // Threshold Exceeded Flag
                                                  // Clear Register
        union    CCEIE_REG CCEIE;                 // C28 Single Error Interrupt
                                                  // Enable Register
        Uint16 rsvd2[2];                          // Reserved
        Uint16 rsvd3[2];                          // Reserved
        union    CNMAVFLG_REG CNMAVFLG;           // Non-Master Access Violation
                                                  // Flag Register
        union    CNMAVFRC_REG CNMAVFRC;           // Non-Master RAM Access
                                                  // Violation Force Register
        union    CNMAVCLR_REG CNMAVCLR;           // Non-Master RAM Access
                                                  // Violation Flag Clear
                                                  // Register
        union    CNMAVIE_REG CNMAVIE;             // Non-Master RAM Access
                                                  // Violation Interrupt Enable
                                                  // Register
        union    CMAVFLG_REG CMAVFLG;             // Master Access Violation
                                                  // Flag Register
        union    CMAVFRC_REG CMAVFRC;             // Master RAM Access Violation
                                                  // Force Register
        union    CMAVCLR_REG CMAVCLR;             // Master RAM Access Violation
                                                  // Flag Clear Register
        union    CMAVIE_REG CMAVIE;               // Master RAM Access Violation
                                                  // Interrupt Enable Register
        Uint32 CNMWRAVADDR;                       // Non-Master CPU Write Access
                                                  // Violation Address
        Uint32 CNMDMAWRAVADDR;                    // Non-Master DMA Write Access
                                                  // Violation Address
        Uint32 CNMFAVADDR;                        // Non-Master CPU Fetch Access
                                                  // Violation Address
        Uint16 rsvd4[2];                          // Reserved
        Uint32 CMWRAVADDR;                        // Master CPU Write Access
                                                  // Violation Address
        Uint32 CMDMAWRAVADDR;                     // Master DMA Write Access
                                                  // Violation Address
        Uint32 CMFAVADDR;                         // Master CPU Fetch Access
                                                  // Violation Address
};

//---------------------------------------------------------------------------
// Ram External References & Function Declarations:
extern volatile struct RAM_REGS RAMRegs;
extern volatile struct RAM_ERR_REGS RAMErrRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_RAM_H definition


