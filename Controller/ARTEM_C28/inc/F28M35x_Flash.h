//###########################################################################
// FILE:   F28M35x_Flash.h
// TITLE:  F28M35x Device Flash Error Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_FLASH_H
#define F28M35x_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// Flash Individual Register Bit Definitions:
struct ECC_ENABLE_BITS {               // bits description
        Uint16 ENABLE : 4;             // 3:0    Enable ECC
        Uint16 rsvd1 : 12;             // 15:4    Reserved
        Uint16 rsvd2 : 16;             // 31:16 Reserved
};

union ECC_ENABLE_REG {
        Uint32 all;
        struct ECC_ENABLE_BITS bit;
};

struct ERR_STATUS_BITS {                // bits description
        Uint16 FAIL0 : 1;               // 0     Single Bit Error Corrected Value
                                        //       0
        Uint16 FAIL1 : 1;               // 1     Single Bit Error Corrected Value
                                        //       1
        Uint16 UNCERR : 1;              // 2     Uncorrectable Error Occurred
        Uint16 rsvd1 : 13;              // 15:6  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union ERR_STATUS_REG {
        Uint32 all;
        struct ERR_STATUS_BITS bit;
};

struct ERR_POS_BITS {                   // bits description
        Uint16 ERRPOS : 6;              // 5:0   Bit Position of Error
        Uint16 ECCLH : 1;               // 6     ECC Error High or Low Bits
        Uint16 rsvd1 : 1;               // 7     Reserved
        Uint16 ERRTYPE : 1;             // 8     Error Type
        Uint16 rsvd2 : 7;               // 15:9  Reserved
        Uint16 rsvd3 : 16;              // 31:16 Reserved
};

union ERR_POS_REG {
        Uint32 all;
        struct ERR_POS_BITS bit;
};

struct ERR_STATUS_CLR_BITS {             // bits description
        Uint16 FAIL0CLR : 1;             // 0     Single Bit Error Corrected
                                         //       Value 0 Clear
        Uint16 FAIL1CLR : 1;             // 1     Single Bit Error Corrected
                                         //       Value 1 Clear
        Uint16 UNCERRCLR : 1;            // 2     Uncorrectable Error Occurred
                                         //       Clear
        Uint16 rsvd1 : 13;               // 15:6  Reserved
        Uint16 rsvd2 : 16;               // 31:16 Reserved
};

union ERR_STATUS_CLR_REG {
        Uint32 all;
        struct ERR_STATUS_CLR_BITS bit;
};

struct  ERR_CNT_BITS{               // bit description
        Uint16 ERR_CNT : 16;        // 15:0   Error counter
        Uint16 rsvd1   : 16;        // 31:16   Reserved
};

union ERR_CNT_REG {
        Uint32 all;
        struct ERR_CNT_BITS bit;
};

struct  ERR_THRESHOLD_BITS{                  // bit description
        Uint16 ERR_THRESHOLD : 16;           // 15:0   Error threshold
        Uint16 rsvd1         : 16;           // 31:16   Reserved
};

union ERR_THRESHOLD_REG {
        Uint32 all;
        struct ERR_THRESHOLD_BITS bit;
};

struct ERR_INTFLG_BITS {                  // bit description
        Uint16 SINGLE_ERR_INTFLG : 1;     // 0     Single Error Interrupt Flag
        Uint16 UNC_ERR_INTFLG : 1;        // 1     Uncorrectable Error Flag
        Uint16 rsvd1 : 14;                // 15:2  Reserved
        Uint16 rsvd2 : 16;                // 31:16 Reserved
};

union ERR_INTFLG_REG {
        Uint32 all;
        struct ERR_INTFLG_BITS bit;
};

struct ERR_INTCLR_BITS {                // bit description
        Uint16 SINGLE_ERR_INTCLR : 1;   // 0     Single Error Interrupt Flag
                                        //       Clear
        Uint16 UNC_ERR_INTCLR : 1;      // 1     Uncorrectable Error Flag Clear
        Uint16 rsvd1 : 14;              // 15:2  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union ERR_INTCLR_REG {
        Uint32 all;
        struct ERR_INTCLR_BITS bit;
};

struct FADDR_TEST_BITS {               // bit description
        Uint32 rsvd1 : 3;              // 2:0   Reserved
        Uint32 ADDR  : 21;             // 23:3 ECC Address High
        Uint32 rsvd2 : 8;              // 31:24 Reserved
};

union FADDR_TEST_REG {
        Uint32 all;
        struct FADDR_TEST_BITS bit;
};

struct FECC_TEST_BITS {                 // bit description
        Uint16 ECC : 8;                 // 7:0   ECC Control Bits
        Uint16 rsvd1 : 8;               // 15:8  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union FECC_TEST_REG {
        Uint32 all;
        struct FECC_TEST_BITS bit;
};

struct FECC_CTRL_BITS {                // bit description
        Uint16 ECC_TEST_EN : 1;        // 0     Enable ECC Test Logic
        Uint16 ECC_SELECT : 1;         // 1     ECC Bit Select
        Uint16 rsvd1 : 14;             // 15:2  Reserved
        Uint16 rsvd2 : 16;             // 31:16 Reserved
};

union FECC_CTRL_REG {
        Uint32 all;
        struct FECC_CTRL_BITS bit;
};

struct FECC_STATUS_BITS {               // bit description
        Uint16 SINGLE_ERR : 1;          // 0     Test Result is Single Bit Error
        Uint16 UNC_ERR : 1;             // 1     Test Result is Uncorrectable
                                        //       Error
        Uint16 ERR_POS : 6;             // 7:2     Holds Bit Position of Error
        Uint16 CHK_ERR : 1;             // 8     Holds Bit Position of 8 Check
                                        //       Bits of Error
        Uint16 rsvd1 : 7;               // 15:9  Reserved
        Uint16 rsvd2 : 16;              // 31:16 Reserved
};

union FECC_STATUS_REG {
        Uint32 all;
        struct FECC_STATUS_BITS bit;
};

struct  FRDCNTL_BITS{                   // bit description
        Uint16 rsvd1 : 8;               // 7:0   Reserved
        Uint16 RWAIT : 4;               // 11:8  Number of C28x clock in a RDCLK period
        Uint16 rsvd2 : 4;               // 15:12 Reserved
        Uint16 rsvd3 : 16;              // 31:16 Reserved
};

union FRDCNTL_REG {
        Uint32 all;
        struct FRDCNTL_BITS bit;
};

struct FSPRD_BITS {                       // bit description
        Uint16 RM0   : 1;        // 0     ReadMargin0 Configuration
        Uint16 RM1   : 1;        // 1     Readmargin1 Configuration
        Uint16 rsvd1 : 14;                // 15:2  Reserved
        Uint16 rsvd2 : 16;                // 31:16 Reserved
};

union FSPRD_REG {
        Uint32 all;
        struct FSPRD_BITS bit;
};

struct  FBAC_BITS{                  // bit description
        Uint16 VREADST : 8;         // 7:0    VREAD Setup Time Count
        Uint16 BAGP    : 8;         // 15:8   Bank Active Grace Period
        Uint16 rsvd1   : 16;        // 31:16 Reserved
};

union FBAC_REG {
        Uint32 all;
        struct FBAC_BITS bit;
};

struct  FBFALLBACK_BITS{            // bit description
        Uint16 BNKPWR0  : 2;        // 1:0    Bank Power Mode
        Uint16 rsvd1   : 14;        // 15:2   Reserved
        Uint16 rsvd2   : 16;        // 31:16   Reserved
};

union FBFALLBACK_REG {
        Uint32 all;
        struct FBFALLBACK_BITS bit;
};

struct  FPAC1_BITS{                 // bit description
        Uint16 PMPPWR  : 1;         // 0      Charge Pump Fallback Power Mode
        Uint16 rsvd1   : 15;        // 15:1   Reserved
        Uint16 PSLEEP  : 11;        // 26:16  Pump Sleep Down Count
        Uint16 rsvd2   : 5;         // 31:27  Reserved
};

union FPAC1_REG {
        Uint32 all;
        struct FPAC1_BITS bit;
};

struct  FPAC2_BITS{                 // bit description
        Uint16 PAGP    : 16;        // 15:0   Pump Active Grace Period
        Uint16 rsvd1   : 16;        // 31:16  Reserved
};

union FPAC2_REG {
        Uint32 all;
        struct FPAC2_BITS bit;
};

struct  FBPRDY_BITS{                // bit description
        Uint16 BANKRDY  : 1;        // 1       Flash Bank n Active Power State
        Uint16 rsvd1    : 14;       // 14:1    Reserved
        Uint16 PUMPRDY  : 1;        // 15      Flash Pump Active Power Mode
        Uint16 rsvd2    : 16;       // 31:16   Reserved
};

union FBPRDY_REG {
        Uint32 all;
        struct FBPRDY_BITS bit;
};

struct  FMAC_BITS{                  // bit description
        Uint16 BANK     : 3;        // 2:0  Flash Bank for FSM and Software Interface Operations
        Uint16 rsvd1    : 13;       // 15:3 Reserved
        Uint16 rsvd2    : 16;       // 31:16   Reserved
};

union FMAC_REG {
        Uint32 all;
        struct FMAC_BITS bit;
};

struct  FMSTAT_BITS{                // bit description
        Uint16 rsvd1    : 1;        // 0   Reserved
        Uint16 PSUSP    : 1;        // 1   Program Suspend Status
        Uint16 ESUSP    : 1;        // 2   Erase Suspend Status
        Uint16 VOLTSTAT : 1;        // 3   Flash Pump Power Status
        Uint16 CSTAT    : 1;        // 4   Command Fail Status
        Uint16 INVDAT   : 1;        // 5   Invalid Data
        Uint16 PGM      : 1;        // 6   Program Operation Status
        Uint16 ERS      : 1;        // 7   Erase Operation Status
        Uint16 BUSY     : 1;        // 8   Busy Bit
        Uint16 CV       : 1;        // 9   Compact Verify Status
        Uint16 EV       : 1;        // 10  Erase Verify Status
        Uint16 PCV      : 1;        // 11  Precondition Verify Status
        Uint16 PGV      : 1;        // 12  Programming Verify Status
        Uint16 DBF      : 1;        // 13
        Uint16 ILA      : 1;        // 14  Illegal Address Detected
        Uint16 RVF      : 1;        // 15  Read Verify Mismatch
        Uint16 RDVER    : 1;        // 16  Read Verify in Progress
        Uint16 RVSUP    : 1;        // 17  Read Verify Suspend
        Uint16 rsvd2    : 14;       // 31:18 Reserved
};

union FMSTAT_REG {
        Uint32 all;
        struct FMSTAT_BITS bit;
};

struct  FRD_INTF_CTRL_BITS{         // bit description
        Uint16 PREFETCH_EN   : 1;   // 0    Prefetch Enable
        Uint16 DATA_CACHE_EN : 1;   // 1    Data Cache Enable
        Uint16 rsvd1         : 14;  // 16:3 Reserved
        Uint16 rsvd2         : 16;  // 31:16 Reserved
};

union FRD_INTF_CTRL_REG {
        Uint32 all;
        struct FRD_INTF_CTRL_BITS bit;
};

struct FLASH_CTRL_REGS {
        union FRDCNTL_REG         FRDCNTL;      //Flash Read Control Register
		union FSPRD_REG           FSPRD;        //Flash Read Margin Control Register
        Uint32                  rsvd1[13];
        union FBAC_REG             FBAC;        //Flash Bank Access Control Register
        union FBFALLBACK_REG     FBFALLBACK;    //Flash Bank Fallback Power Register
        union FBPRDY_REG         FBPRDY;        //Flash Bank Pump Ready Register
        union FPAC1_REG         FPAC1;          //Flash Pump Access Control Register 1
        union FPAC2_REG         FPAC2;          //Flash Pump Access Control Register 2
        union FMAC_REG             FMAC;        //Flash Module Access Control Register
        union FMSTAT_REG         FMSTAT;        //Flash Module Status Register
        Uint32                  rsvd2[170];
        union FRD_INTF_CTRL_REG FRD_INTF_CTRL;  //Flash Read Interface Control Register
};

struct FLASH_ECC_REGS {
        union  ECC_ENABLE_REG ECC_ENABLE;             // ECC Enable
        Uint32 SINGLE_ERR_ADDR;                       // Single Error Address
        Uint32 UNC_ERR_ADDR;                          // Uncorrectable Error Address
        union  ERR_STATUS_REG ERR_STATUS;             // Error Status
        union  ERR_POS_REG ERR_POS;                   // Error Position
        union  ERR_STATUS_CLR_REG ERR_STATUS_CLR;     // Error Status Clear
        union  ERR_CNT_REG ERR_CNT;                   // Error Control
        union  ERR_THRESHOLD_REG ERR_THRESHOLD;       // Error Threshold
        union  ERR_INTFLG_REG ERR_INTFLG;             // Error Interrupt Flag
        union  ERR_INTCLR_REG ERR_INTCLR;             // Error Interrupt Flag Clear
        Uint32 FDATAH_TEST;                           // Data High Test
        Uint32 FDATAL_TEST;                           // Data Low Test
        union  FADDR_TEST_REG FADDR_TEST;             // ECC Test Address
        union  FECC_TEST_REG FECC_TEST;               // ECC Test
        union  FECC_CTRL_REG FECC_CTRL;               // ECC Control
        Uint32 FECC_FOUTH_TEST;                       // Test Data Out High
        Uint32 FECC_FOUTL_TEST;                       // Test Data Out Low
        union  FECC_STATUS_REG FECC_STATUS;           // ECC Status
};

//---------------------------------------------------------------------------
// Flash External References & Function Declarations:
extern volatile struct FLASH_ECC_REGS FlashEccRegs;
extern volatile struct FLASH_CTRL_REGS FlashCtrlRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_FLASH_H definition


