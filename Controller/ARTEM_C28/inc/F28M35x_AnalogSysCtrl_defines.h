//###########################################################################
// FILE:    F28M35x_AnalogSysCtrl_defines.h
// TITLE:   F28M35x SysCtrl Code Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_ANALOGSYSCTRL_DEFINES_H
#define F28M35x_ANALOGSYSCTRL_DEFINES_H

//---------------------------------------------------------------------------
// The following are values that can be passed to the AnalogClockEnable(),
// AnalogClockDisable(), and ReadAnalogClockStatus() functions as the
// AnalogConfigReg parameter.
//---------------------------------------------------------------------------
#define AnalogConfig1           1         // Choose Analog Config1 Register
#define AnalogConfig2           2         // Choose Analog Config2 Register

//---------------------------------------------------------------------------
// The following are values that can be passed to the AnalogClockEnable() and
// AnalogClockDisable() functions as the AnalogClockMask parameter.
//---------------------------------------------------------------------------
#define ADC1_ENABLE             0x0008    // Mask to Enable ADC1
                                          // (AnalogConfig1 Register)
#define ADC2_ENABLE             0x8000    // Mask to Enable ADC2
                                          // (AnalogConfig2 Register)
#define COMP1_ENABLE            0x0001    // Mask to Enable COMP1(AnalogConfig2
                                          // Register)
#define COMP2_ENABLE            0x0002    // Mask to Enable COMP2(AnalogConfig2
                                          // Register)
#define COMP3_ENABLE            0x0004    // Mask to Enable COMP3(AnalogConfig2
                                          // Register)
#define COMP4_ENABLE            0x0008    // Mask to Enable COMP4(AnalogConfig2
                                          // Register)
#define COMP5_ENABLE            0x0010    // Mask to Enable COMP5(AnalogConfig2
                                          // Register)
#define COMP6_ENABLE            0x0020    // Mask to Enable COMP6(AnalogConfig2
                                          // Register)
#define ANALOGCONFIG2ALL        0x803F    // Mask to Enable ADC2,COMP1,2,3,4,5,6
                                          // (AnalogConfig2 Register)

//---------------------------------------------------------------------------
// The following are values that can be passed to the InitAnalogSystemClock()
// function as the ClockDivider parameter.
//---------------------------------------------------------------------------
#define ACLKDIVOFF  0    // Clock divider off
#define ACLKDIV1    1    // Clock divider /1 (PLLSYSCLK/1)
#define ACLKDIV2    2    // Clock divider /2 (PLLSYSCLK/2)
#define ACLKDIV4    3    // Clock divider /4 (PLLSYSCLK/4)
#define ACLKDIV8    4    // Clock divider /8 (PLLSYSCLK/8)

#endif  // end of F28M35x_ANALOGSYSCTRL_DEFINES_H definition



