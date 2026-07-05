//###########################################################################
// FILE:   F28M35x_Examples.h
// TITLE:  F28M35x Device Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_EXAMPLES_H
#define F28M35x_EXAMPLES_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
      Specify the clock rate of the CPU (SYSCLKOUT) in nS.

      Use one of the values provided, or define your own.
      The trailing L is required and tells the compiler to treat
      the number as a 64-bit value.

      Only one statement should be uncommented.

      Example:   150 MHz devices:
                 CLKIN is a 20 MHz crystal

                 From the M3 PLL Settings
                 150 MHz CPU clock (PLLSYSCLK = 150 MHz).

                 In this case, the CPU_RATE will be 6.667L
                 Uncomment the line: #define CPU_RATE 6.667L

   --------------------------------------------------------------------------*/
#define CPU_RATE     6.667L    // for a 150MHz CPU clock speed (PLLSYSCLK)
//#define CPU_RATE     10.00L    // for a 100MHz CPU clock speed (PLLSYSCLK)
//#define CPU_RATE     13.33L    // for a 75MHz CPU clock speed (PLLSYSCLK)
//#define CPU_RATE     16.667L   // for a 60MHz CPU clock speed (PLLSYSCLK)
//----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
      Target device (in F28M35x_Device.h) determines CPU frequency
      (for examples) - either 150 MHz (for H devices) 75 MHz
      (for M devices), and 60 (for E devices). User does not have to 
      change anything here.
-----------------------------------------------------------------------------*/
#if DSP28_F28M35Ex                // F28M35Ex device only
  #define CPU_FRQ_60MHZ     1     // 60 Mhz CPU Freq (20 MHz input freq)
  #define CPU_FRQ_75MHZ     0
  #define CPU_FRQ_150MHZ    0
#elif DSP28_F28M35Mx              // F28M35Mx device only
  #define CPU_FRQ_60MHZ     0     // 75 Mhz CPU Freq (20 MHz input freq)
  #define CPU_FRQ_75MHZ     1
  #define CPU_FRQ_150MHZ    0
#elif DSP28_F28M35Hx              // F28M35Hx device only
  #define CPU_FRQ_60MHZ     0     // 150 Mhz CPU Freq (20 MHz input freq)
  #define CPU_FRQ_75MHZ     0
  #define CPU_FRQ_150MHZ    1
#else                             // CUSTOM CONFIGURATION
  #define CPU_FRQ_60MHZ     0     // User defined Setup
  #define CPU_FRQ_75MHZ     0
  #define CPU_FRQ_100MHZ    0
  #define CPU_FRQ_150MHZ    1
#endif


//----------------------------------------------------------------------------
// Include Example Header Files:
//----------------------------------------------------------------------------

#include "F28M35x_GlobalPrototypes.h"         // Prototypes for global functions
                                              // within the .c files.
#include "F28M35x_EPwm_defines.h"             // Macros used for PWM examples.
#include "F28M35x_I2c_defines.h"              // Macros used for I2C examples.
#include "F28M35x_Dma_defines.h"              // Macros used for DMA examples.
#include "F28M35x_AnalogSysCtrl_defines.h"    // Macros used for Analog
                                              // examples.
#include "F28M35x_set_pinout.h"               // Macros used for GPIO setup in
                                              // examples.

#define    PARTNO_F28M35E52C1Q    0x40
#define    PARTNO_F28M35E52B1Q    0x41
#define    PARTNO_F28M35E50C1Q    0x42
#define    PARTNO_F28M35E50B1Q    0x43
#define    PARTNO_F28M35E22C1Q    0x44
#define    PARTNO_F28M35E22B1Q    0x45
#define    PARTNO_F28M35E20C1Q    0x46
#define    PARTNO_F28M35E20B1Q    0x47

#define    PARTNO_F28M35E52C1T    0x48
#define    PARTNO_F28M35E52B1T    0x49
#define    PARTNO_F28M35E50C1T    0x4A
#define    PARTNO_F28M35E50B1T    0x4B
#define    PARTNO_F28M35E22C1T    0x4C
#define    PARTNO_F28M35E22B1T    0x4D
#define    PARTNO_F28M35E20C1T    0x4E
#define    PARTNO_F28M35E20B1T    0x4F

#define    PARTNO_F28M35H52C1Q    0x50
#define    PARTNO_F28M35H52B1Q    0x51
#define    PARTNO_F28M35H50C1Q    0x52
#define    PARTNO_F28M35H50B1Q    0x53
#define    PARTNO_F28M35H22C1Q    0x54
#define    PARTNO_F28M35H22B1Q    0x55
#define    PARTNO_F28M35H20C1Q    0x56
#define    PARTNO_F28M35H20B1Q    0x57

#define    PARTNO_F28M35H52C1T    0x58
#define    PARTNO_F28M35H52B1T    0x59
#define    PARTNO_F28M35H50C1T    0x5A
#define    PARTNO_F28M35H50B1T    0x5B
#define    PARTNO_F28M35H22C1T    0x5C
#define    PARTNO_F28M35H22B1T    0x5D
#define    PARTNO_F28M35H20C1T    0x5E
#define    PARTNO_F28M35H20B1T    0x5F

#define    PARTNO_F28M35M52C1Q    0x08
#define    PARTNO_F28M35M52B1Q    0x09
#define    PARTNO_F28M35M50C1Q    0x0A
#define    PARTNO_F28M35M50B1Q    0x0B
#define    PARTNO_F28M35M22C1Q    0x0C
#define    PARTNO_F28M35M22B1Q    0x0D
#define    PARTNO_F28M35M20C1Q    0x0E
#define    PARTNO_F28M35M20B1Q    0x0F

// Include files not used with DSP/BIOS
#ifndef DSP28_BIOS
#include "F28M35x_DefaultISR.h"
#endif

// DO NOT MODIFY THIS LINE.
#define DELAY_US(A)  DSP28x_usDelay( \
        ((((long double) A * \
           1000.0L) / \
          (long double)CPU_RATE) - 9.0L) / 5.0L)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of F28M35x_EXAMPLES_H definition



