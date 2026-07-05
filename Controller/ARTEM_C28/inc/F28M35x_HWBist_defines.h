//###########################################################################
// FILE:   F28M35x_hwbist_defines.h
// TITLE:  Prototypes for the HWBIST driver.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_HWBISTFUNCS_H
#define F28M35x_HWBISTFUNCS_H

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// Coverage configuration options for HWBISTSelfTestInit(...)
//*****************************************************************************
#define HWBIST_CONFIG_COV_99PERCENT   0x00000000
#define HWBIST_CONFIG_COV_95PERCENT   0x00000001

//*****************************************************************************
// Memory addresses for the 95% coverage MISR
//*****************************************************************************
#define HWBIST_95PERCENT_MISR   0x003FFFA8
#define HWBIST_95PERCENT_MISR0  (HWBIST_95PERCENT_MISR + 6)
#define HWBIST_95PERCENT_MISR1  (HWBIST_95PERCENT_MISR + 4)
#define HWBIST_95PERCENT_MISR2  (HWBIST_95PERCENT_MISR + 2)
#define HWBIST_95PERCENT_MISR3  (HWBIST_95PERCENT_MISR)

//*****************************************************************************
// Memory addresses for the 99% coverage MISR
//*****************************************************************************
#define HWBIST_99PERCENT_MISR   0x003FFFB0
#define HWBIST_99PERCENT_MISR0  (HWBIST_99PERCENT_MISR + 6)
#define HWBIST_99PERCENT_MISR1  (HWBIST_99PERCENT_MISR + 4)
#define HWBIST_99PERCENT_MISR2  (HWBIST_99PERCENT_MISR + 2)
#define HWBIST_99PERCENT_MISR3  (HWBIST_99PERCENT_MISR)

//*****************************************************************************
// Shift clock divider configuration options for HWBISTShiftClockSet(...)
//*****************************************************************************
#define HWBIST_CONFIG_SHIFTCLOCKDIV_1    0x0
#define HWBIST_CONFIG_SHIFTCLOCKDIV_2    0x1
#define HWBIST_CONFIG_SHIFTCLOCKDIV_4    0x2

//*****************************************************************************
// Return bit masks for HWBISTResultGet()
//*****************************************************************************
#define HWBIST_RESULT_BISTDONE   0x1     // HWBIST Test Complete
#define HWBIST_RESULT_NMI        0x4     // Early exit due to NMI
#define HWBIST_RESULT_BISTFAIL   0x8     // HWBIST Failure
#define HWBIST_RESULT_INTCMPF    0x10    // Intermediate Comparison Failure
#define HWBIST_RESULT_TOFAIL     0x20    // Time Out Failure

//*****************************************************************************
// Multiple Input Signature Register 128-bit Type
// Use this struct to pass MISR signatures between functions and the app
//*****************************************************************************
typedef struct
{
    unsigned long Misr0;
    unsigned long Misr1;
    unsigned long Misr2;
    unsigned long Misr3;
}
tMisr;

void HWBISTSelfTestInit(unsigned long ulMicroInterval,
                       unsigned long ulCoverage,
                       unsigned char ucShiftClock);
int HWBISTSelfTestMicroRun(void);
int HWBISTSelfTestFullRun(void);
void HWBISTSelfTestRestart(void);
void HWBISTTestLogicSet(unsigned long ulLogicError);
void HWBISTTestNmiSet(int bEnable);
void HWBISTTestTimeoutSet(int bEnable);
void HWBISTShiftClockSet(unsigned char ucShiftClock);
unsigned long HWBISTResultGet(void);
int HWBISTResultBasicGet(void);
tMisr HWBISTMisrGet(void);
tMisr HWBISTMisrGoldenGet(void);
int HWBISTCompareMisr(tMisr m1, tMisr m2);

//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // F28M35x_HWBISTFUNCS_H
