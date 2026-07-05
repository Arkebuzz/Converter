//###########################################################################
// FILE:   F28M35x_Dma_defines.h
// TITLE:  #defines used in DMA examples
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_DMA_DEFINES_H
#define F28M35x_DMA_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

// MODE
// PERINTSEL bits
#define DMA_ADCINT1     1
#define DMA_ADCINT2     2
#define DMA_XINT1       3
#define DMA_XINT2       4
#define DMA_XINT3       5

#define DMA_EPWM8_SOCA  7
#define DMA_EPWM8_SCOB  8
#define DMA_EPWM9_SOCA  9
#define DMA_EPWM9_SCOB  10
#define DMA_TINT0       11
#define DMA_TINT1       12
#define DMA_TINT2       13
#define DMA_MXEVTA      14
#define DMA_MREVTA      15
#define DMA_ADCINT3     16
#define DMA_ADCINT4     17
#define DMA_EPWM1_SOCA  18
#define DMA_EPWM1_SCOB  19
#define DMA_EPWM2_SOCA  20
#define DMA_EPWM2_SCOB  21
#define DMA_EPWM3_SOCA  22
#define DMA_EPWM3_SCOB  23
#define DMA_EPWM4_SOCA  24
#define DMA_EPWM4_SCOB  25
#define DMA_EPWM5_SOCA  26
#define DMA_EPWM5_SCOB  27
#define DMA_EPWM6_SOCA  28
#define DMA_EPWM6_SCOB  29
#define DMA_EPWM7_SOCA  30
#define DMA_EPWM7_SCOB  31
// OVERINTE bit
#define OVRFLOW_DISABLE 0x0
#define OVEFLOW_ENABLE  0x1
// PERINTE bit
#define PERINT_DISABLE  0x0
#define PERINT_ENABLE   0x1
// CHINTMODE bits
#define CHINT_BEGIN     0x0
#define CHINT_END       0x1
// ONESHOT bits
#define ONESHOT_DISABLE 0x0
#define ONESHOT_ENABLE  0x1
// CONTINOUS bit
#define CONT_DISABLE    0x0
#define CONT_ENABLE     0x1
// SYNCE bit
#define SYNC_DISABLE    0x0
#define SYNC_ENABLE     0x1
// SYNCSEL bit
#define SYNC_SRC        0x0
#define SYNC_DST        0x1
// DATASIZE bit
#define SIXTEEN_BIT     0x0
#define THIRTYTWO_BIT   0x1
// CHINTE bit
#define CHINT_DISABLE   0x0
#define CHINT_ENABLE    0x1

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of F28M35x_DMA_DEFINES_H



