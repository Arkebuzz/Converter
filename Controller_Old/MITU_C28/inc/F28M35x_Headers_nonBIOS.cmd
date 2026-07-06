/*
//###########################################################################
// FILE:    F28M3Xx_Headers_nonBIOS.cmd
// TITLE:   F28M3Xx Peripheral registers linker command file
// DESCRIPTION:
//          This file is for use in Non-BIOS applications.
//          Linker command file to place the peripheral structures
//          used within the F28M3Xx headerfiles into the correct memory
//          mapped locations.
//          This version of the file includes the PieVectorTable structure.
//          For BIOS applications, please use the F28M3Xx_Headers_BIOS.cmd file
//          which does not include the PieVectorTable structure.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################
*/

MEMORY
{
 PAGE 0:    /* Program Memory */

 PAGE 1:    /* Data Memory */

   DEV_EMU             : origin = 0x000880, length = 0x000180     /* device emulation registers */
   CSM                 : origin = 0x000AE0, length = 0x000020       /* code security module registers */

   ADC1_RESULT         : origin = 0x000B00, length = 0x000020     /* ADC1 Results register */
   ADC2_RESULT         : origin = 0x000B40, length = 0x000020     /* ADC2 Results register */

   CPU_TIMER0          : origin = 0x000C00, length = 0x000008     /* CPU Timer0 registers */
   CPU_TIMER1          : origin = 0x000C08, length = 0x000008     /* CPU Timer1 registers */
   CPU_TIMER2          : origin = 0x000C10, length = 0x000008     /* CPU Timer2 registers */

   PIE_CTRL            : origin = 0x000CE0, length = 0x000020     /* PIE control registers */
   PIE_VECT            : origin = 0x000D00, length = 0x000100     /* PIE Vector Table */
   PIE_VECT_CP         : origin = 0x000E00, length = 0x000100     /* PIE Vector Table Copy */

   DMA                 : origin = 0x001000, length = 0x000200     /* DMA registers */

   ASYSCTRLCONFIG      : origin = 0x001700, length = 0x000080       /* Analog System Control Configuration Registers */
   
   HWBIST              : origin = 0x001780, length = 0x000040       /* HW BIST Registers    */

   FLASH_REGS          : origin = 0x004000, length = 0x000300     /* Flash Control registers */
   FLASH_ECC           : origin = 0x004300, length = 0x000040     /* Flash/OTP ECC Error Log registers */

   M3PLL               : origin = 0x004400, length = 0x000040       /* M3 PLL Clock Configuration Registers  */
   
   RAM_REGS            : origin = 0x004900, length = 0x000080     /* RAM Control registers */
   RAM_ERR_REGS        : origin = 0x004A00, length = 0x000080     /* RAM ECC/PARITY/ACCESS Error Log Registers */

   CM_MC_IPC           : origin = 0x004E00, length = 0x000040     /* C28 Control to Master IPC registers */
       
   MCBSPA              : origin = 0x005000, length = 0x000040     /* McBSP-A registers */
   
   EPWM1               : origin = 0x005100, length = 0x000080     /* EPWM1 + HRPWM registers */
   EPWM2               : origin = 0x005180, length = 0x000080     /* EPWM2 + HRPWM registers */
   EPWM3               : origin = 0x005200, length = 0x000080     /* EPWM3 + HRPWM registers */
   EPWM4               : origin = 0x005280, length = 0x000080     /* EPWM4 + HRPWM registers */
   EPWM5               : origin = 0x005300, length = 0x000080     /* EPWM5 + HRPWM registers */
   EPWM6               : origin = 0x005380, length = 0x000080     /* EPWM6 + HRPWM registers */
   EPWM7               : origin = 0x005400, length = 0x000080     /* EPWM7 + HRPWM registers */
   EPWM8               : origin = 0x005480, length = 0x000080     /* EPWM8 + HRPWM registers */
   EPWM9               : origin = 0x005500, length = 0x000080     /* EPWM9 registers */

   ECAP1               : origin = 0x005A00, length = 0x000020     /* Enhanced Capture 1 registers */
   ECAP2               : origin = 0x005A20, length = 0x000020     /* Enhanced Capture 2 registers */
   ECAP3               : origin = 0x005A40, length = 0x000020     /* Enhanced Capture 3 registers */
   ECAP4               : origin = 0x005A60, length = 0x000020     /* Enhanced Capture 4 registers */
   ECAP5               : origin = 0x005A80, length = 0x000020     /* Enhanced Capture 5 registers */
   ECAP6               : origin = 0x005AA0, length = 0x000020     /* Enhanced Capture 6 registers */

   EQEP1               : origin = 0x005B00, length = 0x000040     /* Enhanced QEP 1 registers */
   EQEP2               : origin = 0x005B40, length = 0x000040     /* Enhanced QEP 2 registers */
   EQEP3               : origin = 0x005B80, length = 0x000040     /* Enhanced QEP 3 registers */

   GPIOG1CTRL          : origin = 0x005F80, length = 0x000040     /* GPIO control registers */
   GPIOG1DAT           : origin = 0x005FC0, length = 0x000020     /* GPIO data registers */
   GPIOG1TRIP          : origin = 0x005FE0, length = 0x000020     /* GPIO trip/LPM registers */

   COMP1               : origin = 0x006400, length = 0x000020     /* Comparator + DAC 1 registers */
   COMP2               : origin = 0x006420, length = 0x000020     /* Comparator + DAC 2 registers */
   COMP3               : origin = 0x006440, length = 0x000020     /* Comparator + DAC 3 registers */
   COMP4               : origin = 0x006460, length = 0x000020     /* Comparator + DAC 4 registers */
   COMP5               : origin = 0x006480, length = 0x000020     /* Comparator + DAC 5 registers */
   COMP6               : origin = 0x0064A0, length = 0x000020     /* Comparator + DAC 6 registers */

   GPIOG2CTRL          : origin = 0x006F80, length = 0x000040     /* GPIO control registers */
   GPIOG2DAT           : origin = 0x006FC0, length = 0x000020     /* GPIO data registers */
   
   SYSTEM              : origin = 0x007010, length = 0x000020     /* System control registers */

   SPIA                : origin = 0x007040, length = 0x000010     /* SPI-A registers */\
   SCIA                : origin = 0x007050, length = 0x000010     /* SCI-A registers */

   NMIINTRUPT          : origin = 0x007060, length = 0x000010     /* NMI Watchdog Interrupt Registers */
   XINTRUPT            : origin = 0x007070, length = 0x000010     /* external interrupt registers */

   ADC1                : origin = 0x007100, length = 0x000080     /* ADC1 registers */
   ADC2                : origin = 0x007180, length = 0x000080     /* ADC2 registers */

   I2CA                : origin = 0x007900, length = 0x000040     /* I2C-A registers */

   FLASH_EXE_ONLY      : origin = 0x13FFF2, length = 0x000002       /* FLASH execution only locations */
   ECSL_PWL            : origin = 0x13FFF4, length = 0x000004       /* FLASH ECSL password locations  */
   CSM_PWL             : origin = 0x13FFF8, length = 0x000008     /* FLASH CSM password locations.  */
   
}


SECTIONS
{
/*** PIE Vect Table and Boot ROM Variables Structures ***/
  UNION run = PIE_VECT, PAGE = 1
   {
      PieVectTableFile
      GROUP
      {
         EmuKeyVar
         EmuBModeVar
         FlashCallbackVar
         FlashScalingVar
      }
   }

/*** Peripheral Frame 0 Register Structures ***/
   DevEmuRegsFile       : > DEV_EMU,     PAGE = 1
   CsmRegsFile          : > CSM,         PAGE = 1
   UNION run =              ADC1_RESULT, PAGE = 1
   {
       AdcResultFile
       Adc1ResultFile
   }
   Adc2ResultFile        : > ADC2_RESULT,     PAGE = 1
   CpuTimer0RegsFile     : > CPU_TIMER0,      PAGE = 1
   CpuTimer1RegsFile     : > CPU_TIMER1,      PAGE = 1
   CpuTimer2RegsFile     : > CPU_TIMER2,      PAGE = 1
   PieCtrlRegsFile       : > PIE_CTRL,        PAGE = 1
   PieVectTableCopyFile  : > PIE_VECT_CP,     PAGE = 1
   DmaRegsFile           : > DMA,             PAGE = 1     
   AnalogSysctrlRegsFile : > ASYSCTRLCONFIG,  PAGE = 1 
   HWBistRegsFile         : > HWBIST,         PAGE = 1

/*** Peripheral Frame 1 Register Structures ***/
   FlashCtrlRegsFile : > FLASH_REGS,  PAGE = 1
   FlashEccRegsFile  : > FLASH_ECC,   PAGE = 1
   M3PllRegsFile     : > M3PLL,       PAGE = 1
   RAMRegsFile       : > RAM_REGS,    PAGE = 1
   RAMErrRegsFile    : > RAM_ERR_REGS,PAGE = 1
   CtoMIpcRegsFile   : > CM_MC_IPC,   PAGE = 1
   
/*** Peripheral Frame 2 Register Structures ***/
   SysCtrlRegsFile   : > SYSTEM,      PAGE = 1
   SpiaRegsFile      : > SPIA,        PAGE = 1
   SciaRegsFile      : > SCIA,        PAGE = 1
   NmiIntruptRegsFile: > NMIINTRUPT,  PAGE = 1
   XIntruptRegsFile  : > XINTRUPT,    PAGE = 1
   UNION run =           ADC1,        PAGE = 1
   {
       AdcRegsFile
       Adc1RegsFile
   }
   Adc2RegsFile      : > ADC2,        PAGE = 1
   I2caRegsFile      : > I2CA,        PAGE = 1

/*** Peripheral Frame 3 Register Structures ***/
   McbspaRegsFile    : > MCBSPA,      PAGE = 1
   EPwm1RegsFile     : > EPWM1,       PAGE = 1
   EPwm2RegsFile     : > EPWM2,       PAGE = 1
   EPwm3RegsFile     : > EPWM3,       PAGE = 1
   EPwm4RegsFile     : > EPWM4,       PAGE = 1
   EPwm5RegsFile     : > EPWM5,       PAGE = 1
   EPwm6RegsFile     : > EPWM6,       PAGE = 1
   EPwm7RegsFile     : > EPWM7,       PAGE = 1
   EPwm8RegsFile     : > EPWM8,       PAGE = 1
   EPwm9RegsFile     : > EPWM9,       PAGE = 1
   ECap1RegsFile     : > ECAP1,       PAGE = 1
   ECap2RegsFile     : > ECAP2,       PAGE = 1
   ECap3RegsFile     : > ECAP3,       PAGE = 1
   ECap4RegsFile     : > ECAP4,       PAGE = 1
   ECap5RegsFile     : > ECAP5,       PAGE = 1
   ECap6RegsFile     : > ECAP6,       PAGE = 1
   EQep1RegsFile     : > EQEP1,       PAGE = 1   
   EQep2RegsFile     : > EQEP2,       PAGE = 1               
   EQep3RegsFile     : > EQEP3,       PAGE = 1               
   UNION run =           GPIOG1CTRL,  PAGE = 1
   {
       GpioCtrlRegsFile
       GpioG1CtrlRegsFile
   }
   UNION run =           GPIOG1DAT,  PAGE = 1
   {
       GpioDataRegsFile
       GpioG1DataRegsFile
   }
   UNION run =           GPIOG1TRIP,  PAGE = 1
   {
       GpioTripRegsFile
       GpioG1TripRegsFile
   }

   Comp1RegsFile      : > COMP1,       PAGE = 1
   Comp2RegsFile      : > COMP2,       PAGE = 1
   Comp3RegsFile      : > COMP3,       PAGE = 1
   Comp4RegsFile      : > COMP4,       PAGE = 1
   Comp5RegsFile      : > COMP5,       PAGE = 1
   Comp6RegsFile      : > COMP6,       PAGE = 1
   GpioG2CtrlRegsFile : > GPIOG2CTRL,  PAGE = 1
   GpioG2DataRegsFile : > GPIOG2DAT,   PAGE = 1
   
/*** Code Security Module Register Structures ***/
   FlashExeOnlyFile  : > FLASH_EXE_ONLY,      PAGE = 1
   EcslPwlFile       : > ECSL_PWL,            PAGE = 1
   CsmPwlFile        : > CSM_PWL,             PAGE = 1
}

/*
*/


