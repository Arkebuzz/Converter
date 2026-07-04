//###########################################################################
// FILE:   F28M35x_Ipc.h
// TITLE:  F28M35x Device IPC Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_IPC_H
#define F28M35x_IPC_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// IPC Individual Register Bit Definitions:

struct CTOMIPCSET_BITS {         // bits description
        Uint16 IPC1 : 1;         // 0    C28 to M3 IPC INT 1 Set
        Uint16 IPC2 : 1;         // 1    C28 to M3 IPC INT 2 Set
        Uint16 IPC3 : 1;         // 2    C28 to M3 IPC INT 3 Set
        Uint16 IPC4 : 1;         // 3    C28 to M3 IPC INT 4 Set
        Uint16 IPC5 : 1;         // 4    C28 to M3 IPC INT 5 Set
        Uint16 IPC6 : 1;         // 5    C28 to M3 IPC INT 6 Set
        Uint16 IPC7 : 1;         // 6    C28 to M3 IPC INT 7 Set
        Uint16 IPC8 : 1;         // 7    C28 to M3 IPC INT 8 Set
        Uint16 IPC9 : 1;         // 8    C28 to M3 IPC INT 9 Set
        Uint16 IPC10 : 1;        // 9    C28 to M3 IPC INT 10 Set
        Uint16 IPC11 : 1;        // 10   C28 to M3 IPC INT 11 Set
        Uint16 IPC12 : 1;        // 11   C28 to M3 IPC INT 12 Set
        Uint16 IPC13 : 1;        // 12   C28 to M3 IPC INT 13 Set
        Uint16 IPC14 : 1;        // 13   C28 to M3 IPC INT 14 Set
        Uint16 IPC15 : 1;        // 14   C28 to M3 IPC INT 15 Set
        Uint16 IPC16 : 1;        // 15   C28 to M3 IPC INT 16 Set
        Uint16 IPC17 : 1;        // 16   C28 to M3 IPC INT 17 Set
        Uint16 IPC18 : 1;        // 17   C28 to M3 IPC INT 18 Set
        Uint16 IPC19 : 1;        // 18   C28 to M3 IPC INT 19 Set
        Uint16 IPC20 : 1;        // 19   C28 to M3 IPC INT 20 Set
        Uint16 IPC21 : 1;        // 20   C28 to M3 IPC INT 21 Set
        Uint16 IPC22 : 1;        // 21   C28 to M3 IPC INT 22 Set
        Uint16 IPC23 : 1;        // 22   C28 to M3 IPC INT 23 Set
        Uint16 IPC24 : 1;        // 23   C28 to M3 IPC INT 24 Set
        Uint16 IPC25 : 1;        // 24   C28 to M3 IPC INT 25 Set
        Uint16 IPC26 : 1;        // 25   C28 to M3 IPC INT 26 Set
        Uint16 IPC27 : 1;        // 26   C28 to M3 IPC INT 27 Set
        Uint16 IPC28 : 1;        // 27   C28 to M3 IPC INT 28 Set
        Uint16 IPC29 : 1;        // 28   C28 to M3 IPC INT 29 Set
        Uint16 IPC30 : 1;        // 29   C28 to M3 IPC INT 30 Set
        Uint16 IPC31 : 1;        // 30   C28 to M3 IPC INT 31 Set
        Uint16 IPC32 : 1;        // 31   C28 to M3 IPC INT 32 Set
};

union CTOMIPCSET_REG {
        Uint32 all;
        struct CTOMIPCSET_BITS bit;
};

struct CTOMIPCCLR_BITS {         // bits description
        Uint16 IPC1 : 1;         // 0    C28 to M3 IPC INT 1 Clear
        Uint16 IPC2 : 1;         // 1    C28 to M3 IPC INT 2 Clear
        Uint16 IPC3 : 1;         // 2    C28 to M3 IPC INT 3 Clear
        Uint16 IPC4 : 1;         // 3    C28 to M3 IPC INT 4 Clear
        Uint16 IPC5 : 1;         // 4    C28 to M3 IPC INT 5 Clear
        Uint16 IPC6 : 1;         // 5    C28 to M3 IPC INT 6 Clear
        Uint16 IPC7 : 1;         // 6    C28 to M3 IPC INT 7 Clear
        Uint16 IPC8 : 1;         // 7    C28 to M3 IPC INT 8 Clear
        Uint16 IPC9 : 1;         // 8    C28 to M3 IPC INT 9 Clear
        Uint16 IPC10 : 1;        // 9    C28 to M3 IPC INT 10 Clear
        Uint16 IPC11 : 1;        // 10   C28 to M3 IPC INT 11 Clear
        Uint16 IPC12 : 1;        // 11   C28 to M3 IPC INT 12 Clear
        Uint16 IPC13 : 1;        // 12   C28 to M3 IPC INT 13 Clear
        Uint16 IPC14 : 1;        // 13   C28 to M3 IPC INT 14 Clear
        Uint16 IPC15 : 1;        // 14   C28 to M3 IPC INT 15 Clear
        Uint16 IPC16 : 1;        // 15   C28 to M3 IPC INT 16 Clear
        Uint16 IPC17 : 1;        // 16   C28 to M3 IPC INT 17 Clear
        Uint16 IPC18 : 1;        // 17   C28 to M3 IPC INT 18 Clear
        Uint16 IPC19 : 1;        // 18   C28 to M3 IPC INT 19 Clear
        Uint16 IPC20 : 1;        // 19   C28 to M3 IPC INT 20 Clear
        Uint16 IPC21 : 1;        // 20   C28 to M3 IPC INT 21 Clear
        Uint16 IPC22 : 1;        // 21   C28 to M3 IPC INT 22 Clear
        Uint16 IPC23 : 1;        // 22   C28 to M3 IPC INT 23 Clear
        Uint16 IPC24 : 1;        // 23   C28 to M3 IPC INT 24 Clear
        Uint16 IPC25 : 1;        // 24   C28 to M3 IPC INT 25 Clear
        Uint16 IPC26 : 1;        // 25   C28 to M3 IPC INT 26 Clear
        Uint16 IPC27 : 1;        // 26   C28 to M3 IPC INT 27 Clear
        Uint16 IPC28 : 1;        // 27   C28 to M3 IPC INT 28 Clear
        Uint16 IPC29 : 1;        // 28   C28 to M3 IPC INT 29 Clear
        Uint16 IPC30 : 1;        // 29   C28 to M3 IPC INT 30 Clear
        Uint16 IPC31 : 1;        // 30   C28 to M3 IPC INT 31 Clear
        Uint16 IPC32 : 1;        // 31   C28 to M3 IPC INT 32 Clear
};

union CTOMIPCCLR_REG {
        Uint32 all;
        struct CTOMIPCCLR_BITS bit;
};

struct CTOMIPCFLG_BITS {         // bits description
        Uint16 IPC1 : 1;         // 0    C28 to M3 IPC INT 1 Flag Status
        Uint16 IPC2 : 1;         // 1    C28 to M3 IPC INT 2 Flag Status
        Uint16 IPC3 : 1;         // 2    C28 to M3 IPC INT 3 Flag Status
        Uint16 IPC4 : 1;         // 3    C28 to M3 IPC INT 4 Flag Status
        Uint16 IPC5 : 1;         // 4    C28 to M3 IPC INT 5 Flag Status
        Uint16 IPC6 : 1;         // 5    C28 to M3 IPC INT 6 Flag Status
        Uint16 IPC7 : 1;         // 6    C28 to M3 IPC INT 7 Flag Status
        Uint16 IPC8 : 1;         // 7    C28 to M3 IPC INT 8 Flag Status
        Uint16 IPC9 : 1;         // 8    C28 to M3 IPC INT 9 Flag Status
        Uint16 IPC10 : 1;        // 9    C28 to M3 IPC INT 10 Flag Status
        Uint16 IPC11 : 1;        // 10   C28 to M3 IPC INT 11 Flag Status
        Uint16 IPC12 : 1;        // 11   C28 to M3 IPC INT 12 Flag Status
        Uint16 IPC13 : 1;        // 12   C28 to M3 IPC INT 13 Flag Status
        Uint16 IPC14 : 1;        // 13   C28 to M3 IPC INT 14 Flag Status
        Uint16 IPC15 : 1;        // 14   C28 to M3 IPC INT 15 Flag Status
        Uint16 IPC16 : 1;        // 15   C28 to M3 IPC INT 16 Flag Status
        Uint16 IPC17 : 1;        // 16   C28 to M3 IPC INT 17 Flag Status
        Uint16 IPC18 : 1;        // 17   C28 to M3 IPC INT 18 Flag Status
        Uint16 IPC19 : 1;        // 18   C28 to M3 IPC INT 19 Flag Status
        Uint16 IPC20 : 1;        // 19   C28 to M3 IPC INT 20 Flag Status
        Uint16 IPC21 : 1;        // 20   C28 to M3 IPC INT 21 Flag Status
        Uint16 IPC22 : 1;        // 21   C28 to M3 IPC INT 22 Flag Status
        Uint16 IPC23 : 1;        // 22   C28 to M3 IPC INT 23 Flag Status
        Uint16 IPC24 : 1;        // 23   C28 to M3 IPC INT 24 Flag Status
        Uint16 IPC25 : 1;        // 24   C28 to M3 IPC INT 25 Flag Status
        Uint16 IPC26 : 1;        // 25   C28 to M3 IPC INT 26 Flag Status
        Uint16 IPC27 : 1;        // 26   C28 to M3 IPC INT 27 Flag Status
        Uint16 IPC28 : 1;        // 27   C28 to M3 IPC INT 28 Flag Status
        Uint16 IPC29 : 1;        // 28   C28 to M3 IPC INT 29 Flag Status
        Uint16 IPC30 : 1;        // 29   C28 to M3 IPC INT 30 Flag Status
        Uint16 IPC31 : 1;        // 30   C28 to M3 IPC INT 31 Flag Status
        Uint16 IPC32 : 1;        // 31   C28 to M3 IPC INT 32 Flag Status
};

union CTOMIPCFLG_REG {
        Uint32 all;
        struct CTOMIPCFLG_BITS bit;
};

struct MTOCIPCACK_BITS {         // bits description
        Uint16 IPC1 : 1;         // 0    M3 to C28 IPC INT 1 Acknowledge
        Uint16 IPC2 : 1;         // 1    M3 to C28 IPC INT 2 Acknowledge
        Uint16 IPC3 : 1;         // 2    M3 to C28 IPC INT 3 Acknowledge
        Uint16 IPC4 : 1;         // 3    M3 to C28 IPC INT 4 Acknowledge
        Uint16 IPC5 : 1;         // 4    M3 to C28 IPC INT 5 Acknowledge
        Uint16 IPC6 : 1;         // 5    M3 to C28 IPC INT 6 Acknowledge
        Uint16 IPC7 : 1;         // 6    M3 to C28 IPC INT 7 Acknowledge
        Uint16 IPC8 : 1;         // 7    M3 to C28 IPC INT 8 Acknowledge
        Uint16 IPC9 : 1;         // 8    M3 to C28 IPC INT 9 Acknowledge
        Uint16 IPC10 : 1;        // 9    M3 to C28 IPC INT 10 Acknowledge
        Uint16 IPC11 : 1;        // 10   M3 to C28 IPC INT 11 Acknowledge
        Uint16 IPC12 : 1;        // 11   M3 to C28 IPC INT 12 Acknowledge
        Uint16 IPC13 : 1;        // 12   M3 to C28 IPC INT 13 Acknowledge
        Uint16 IPC14 : 1;        // 13   M3 to C28 IPC INT 14 Acknowledge
        Uint16 IPC15 : 1;        // 14   M3 to C28 IPC INT 15 Acknowledge
        Uint16 IPC16 : 1;        // 15   M3 to C28 IPC INT 16 Acknowledge
        Uint16 IPC17 : 1;        // 16   M3 to C28 IPC INT 17 Acknowledge
        Uint16 IPC18 : 1;        // 17   M3 to C28 IPC INT 18 Acknowledge
        Uint16 IPC19 : 1;        // 18   M3 to C28 IPC INT 19 Acknowledge
        Uint16 IPC20 : 1;        // 19   M3 to C28 IPC INT 20 Acknowledge
        Uint16 IPC21 : 1;        // 20   M3 to C28 IPC INT 21 Acknowledge
        Uint16 IPC22 : 1;        // 21   M3 to C28 IPC INT 22 Acknowledge
        Uint16 IPC23 : 1;        // 22   M3 to C28 IPC INT 23 Acknowledge
        Uint16 IPC24 : 1;        // 23   M3 to C28 IPC INT 24 Acknowledge
        Uint16 IPC25 : 1;        // 24   M3 to C28 IPC INT 25 Acknowledge
        Uint16 IPC26 : 1;        // 25   M3 to C28 IPC INT 26 Acknowledge
        Uint16 IPC27 : 1;        // 26   M3 to C28 IPC INT 27 Acknowledge
        Uint16 IPC28 : 1;        // 27   M3 to C28 IPC INT 28 Acknowledge
        Uint16 IPC29 : 1;        // 28   M3 to C28 IPC INT 29 Acknowledge
        Uint16 IPC30 : 1;        // 29   M3 to C28 IPC INT 30 Acknowledge
        Uint16 IPC31 : 1;        // 30   M3 to C28 IPC INT 31 Acknowledge
        Uint16 IPC32 : 1;        // 31   M3 to C28 IPC INT 32 Acknowledge
};

union MTOCIPCACK_REG {
        Uint32 all;
        struct MTOCIPCACK_BITS bit;
};

struct MTOCIPCSTS_BITS {         // bits description
        Uint16 IPC1 : 1;         // 0    M3 to C28 IPC INT 1 Status
        Uint16 IPC2 : 1;         // 1    M3 to C28 IPC INT 2 Status
        Uint16 IPC3 : 1;         // 2    M3 to C28 IPC INT 3 Status
        Uint16 IPC4 : 1;         // 3    M3 to C28 IPC INT 4 Status
        Uint16 IPC5 : 1;         // 4    M3 to C28 IPC INT 5 Status
        Uint16 IPC6 : 1;         // 5    M3 to C28 IPC INT 6 Status
        Uint16 IPC7 : 1;         // 6    M3 to C28 IPC INT 7 Status
        Uint16 IPC8 : 1;         // 7    M3 to C28 IPC INT 8 Status
        Uint16 IPC9 : 1;         // 8    M3 to C28 IPC INT 9 Status
        Uint16 IPC10 : 1;        // 9    M3 to C28 IPC INT 10 Status
        Uint16 IPC11 : 1;        // 10   M3 to C28 IPC INT 11 Status
        Uint16 IPC12 : 1;        // 11   M3 to C28 IPC INT 12 Status
        Uint16 IPC13 : 1;        // 12   M3 to C28 IPC INT 13 Status
        Uint16 IPC14 : 1;        // 13   M3 to C28 IPC INT 14 Status
        Uint16 IPC15 : 1;        // 14   M3 to C28 IPC INT 15 Status
        Uint16 IPC16 : 1;        // 15   M3 to C28 IPC INT 16 Status
        Uint16 IPC17 : 1;        // 16   M3 to C28 IPC INT 17 Status
        Uint16 IPC18 : 1;        // 17   M3 to C28 IPC INT 18 Status
        Uint16 IPC19 : 1;        // 18   M3 to C28 IPC INT 19 Status
        Uint16 IPC20 : 1;        // 19   M3 to C28 IPC INT 20 Status
        Uint16 IPC21 : 1;        // 20   M3 to C28 IPC INT 21 Status
        Uint16 IPC22 : 1;        // 21   M3 to C28 IPC INT 22 Status
        Uint16 IPC23 : 1;        // 22   M3 to C28 IPC INT 23 Status
        Uint16 IPC24 : 1;        // 23   M3 to C28 IPC INT 24 Status
        Uint16 IPC25 : 1;        // 24   M3 to C28 IPC INT 25 Status
        Uint16 IPC26 : 1;        // 25   M3 to C28 IPC INT 26 Status
        Uint16 IPC27 : 1;        // 26   M3 to C28 IPC INT 27 Status
        Uint16 IPC28 : 1;        // 27   M3 to C28 IPC INT 28 Status
        Uint16 IPC29 : 1;        // 28   M3 to C28 IPC INT 29 Status
        Uint16 IPC30 : 1;        // 29   M3 to C28 IPC INT 30 Status
        Uint16 IPC31 : 1;        // 30   M3 to C28 IPC INT 31 Status
        Uint16 IPC32 : 1;        // 31   M3 to C28 IPC INT 32 Status
};

union MTOCIPCSTS_REG {
        Uint32 all;
        struct MTOCIPCSTS_BITS bit;
};



struct CTOM_IPC_REGS {
        union    CTOMIPCSET_REG CTOMIPCSET;           // C28 to M3 core IPC
                                                      // interrupt request set
                                                      // register
        union    CTOMIPCCLR_REG CTOMIPCCLR;           // C28 to M3 core IPC
                                                      // interrupt request clear
                                                      // register
        union    CTOMIPCFLG_REG CTOMIPCFLG;           // C28 to M3 core IPC
                                                      // interrupt status
                                                      // register
        union    MTOCIPCACK_REG MTOCIPCACK;           // M3 to C28 core IPC
                                                      // interrupt request clear
                                                      // (acknowledge) register
        union    MTOCIPCSTS_REG MTOCIPCSTS;           // M3 to C28 core IPC
                                                      // interrupt status flag
                                                      // register
        Uint16 rsvd1[2];                              // Reserved
        Uint32 MIPCCOUNTERL;                          // IPC Counter Low
                                                      // Register
        Uint32 MIPCCOUNTERH;                          // IPC Counter High
                                                      // Register
        Uint32 CTOMIPCCOM;                            // C28 To M3 IPC Command
                                                      // Register (read/write to
                                                      // C28)
        Uint32 CTOMIPCADDR;                           // C28 To M3 IPC Address
                                                      // Register (read/write to
                                                      // C28)
        Uint32 CTOMIPCDATAW;                          // C28 To M3 IPC Data
                                                      // Write Register
                                                      // (read/write to C28)
        Uint32 CTOMIPCDATAR;                          // C28 To M3 IPC Data Read
                                                      // Register (read only to
                                                      // C28)
        Uint32 MTOCIPCCOM;                            // M3 To C28 IPC Command
                                                      // Register (read only to
                                                      // C28)
        Uint32 MTOCIPCADDR;                           // M3 To C28 IPC Address
                                                      // Register (read only to
                                                      // C28)
        Uint32 MTOCIPCDATAW;                          // M3 To C28 IPC Data
                                                      // Write Register (read
                                                      // only to C28)
        Uint32 MTOCIPCDATAR;                          // M3 To C28 IPC Data Read
                                                      // Register (read/write to
                                                      // C28)
        Uint32 CTOMIPCBOOTSTS;                        // IPC Boot Status
                                                      // Register (read only to
                                                      // C28)
        Uint32 MTOCIPCBOOTMODE;                       // IPC Boot Mode Register
                                                      // (read only to C28)
        Uint32 CPUMPREQUEST;                          // Flash programming
                                                      // semaphore PUMP request
                                                      // register
        Uint32 CCLKREQUEST;                           // Clock configuration
                                                      // semaphore C28 request
                                                      // register
        Uint16 rsvd2[24];                             // Reserved
};

//---------------------------------------------------------------------------
// Ipc External References & Function Declarations:
extern volatile struct CTOM_IPC_REGS CtoMIpcRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_IPC_H definition


