//###########################################################################
// FILE:   F28M35x_set_pinout.h
// TITLE:  Defines for board specific GPIO setup
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_SET_PINOUT_H
#define F28M35x_SET_PINOUT_H

#ifdef __cplusplus
extern "C" {
#endif

#define LED_0_DIR_REG GpioG1CtrlRegs.GPCDIR.bit.GPIO70
#define LED_0_DAT_REG GpioG1DataRegs.GPCDAT.bit.GPIO70    

#define LED_1_DIR_REG GpioG1CtrlRegs.GPCDIR.bit.GPIO71
#define LED_1_DAT_REG GpioG1DataRegs.GPCDAT.bit.GPIO71    

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of F28M35x_DMA_DEFINES_H



