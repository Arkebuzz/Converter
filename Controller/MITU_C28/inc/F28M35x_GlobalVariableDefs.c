//###########################################################################
// FILE:    F28M35x_GlobalVariableDefs.c
// TITLE:   F28M35x Global Variables and Data Section Pragmas.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#include "F28M35x_Device.h"     // F28M35x Headerfile Include File

//---------------------------------------------------------------------------
// Define Global Peripheral Variables:
//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("AdcRegsFile")
#else
#pragma DATA_SECTION(AdcRegs,"AdcRegsFile");
#endif
volatile struct ADC_REGS AdcRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("AdcResultFile")
#else
#pragma DATA_SECTION(AdcResult,"AdcResultFile");
#endif
volatile struct ADC_RESULT_REGS AdcResult;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Adc1RegsFile")
#else
#pragma DATA_SECTION(Adc1Regs,"Adc1RegsFile");
#endif
volatile struct ADC_REGS Adc1Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Adc1ResultFile")
#else
#pragma DATA_SECTION(Adc1Result,"Adc1ResultFile");
#endif
volatile struct ADC_RESULT_REGS Adc1Result;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Adc2RegsFile")
#else
#pragma DATA_SECTION(Adc2Regs,"Adc2RegsFile");
#endif
volatile struct ADC_REGS Adc2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Adc2ResultFile")
#else
#pragma DATA_SECTION(Adc2Result,"Adc2ResultFile");
#endif
volatile struct ADC_RESULT_REGS Adc2Result;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("AnalogSysctrlRegsFile")
#else
#pragma DATA_SECTION(AnalogSysctrlRegs,"AnalogSysctrlRegsFile");
#endif
volatile struct ANALOG_SYSCTRL_REGS AnalogSysctrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Comp1RegsFile")
#else
#pragma DATA_SECTION(Comp1Regs,"Comp1RegsFile");
#endif
volatile struct COMP_REGS Comp1Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Comp2RegsFile")
#else
#pragma DATA_SECTION(Comp2Regs,"Comp2RegsFile");
#endif
volatile struct COMP_REGS Comp2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Comp3RegsFile")
#else
#pragma DATA_SECTION(Comp3Regs,"Comp3RegsFile");
#endif
volatile struct COMP_REGS Comp3Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Comp4RegsFile")
#else
#pragma DATA_SECTION(Comp4Regs,"Comp4RegsFile");
#endif
volatile struct COMP_REGS Comp4Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Comp5RegsFile")
#else
#pragma DATA_SECTION(Comp5Regs,"Comp5RegsFile");
#endif
volatile struct COMP_REGS Comp5Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("Comp6RegsFile")
#else
#pragma DATA_SECTION(Comp6Regs,"Comp6RegsFile");
#endif
volatile struct COMP_REGS Comp6Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CpuTimer0RegsFile")
#else
#pragma DATA_SECTION(CpuTimer0Regs,"CpuTimer0RegsFile");
#endif
volatile struct CPUTIMER_REGS CpuTimer0Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CpuTimer1RegsFile")
#else
#pragma DATA_SECTION(CpuTimer1Regs,"CpuTimer1RegsFile");
#endif
volatile struct CPUTIMER_REGS CpuTimer1Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CpuTimer2RegsFile")
#else
#pragma DATA_SECTION(CpuTimer2Regs,"CpuTimer2RegsFile");
#endif
volatile struct CPUTIMER_REGS CpuTimer2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CsmPwlFile")
#else
#pragma DATA_SECTION(CsmPwl,"CsmPwlFile");
#endif
volatile struct CSM_PWL CsmPwl;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CsmRegsFile")
#else
#pragma DATA_SECTION(CsmRegs,"CsmRegsFile");
#endif
volatile struct CSM_REGS CsmRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("DevEmuRegsFile")
#else
#pragma DATA_SECTION(DevEmuRegs,"DevEmuRegsFile");
#endif
volatile struct DEV_EMU_REGS DevEmuRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("DmaRegsFile")
#else
#pragma DATA_SECTION(DmaRegs,"DmaRegsFile");
#endif
volatile struct DMA_REGS DmaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EcslPwlFile")
#else
#pragma DATA_SECTION(EcslPwl,"EcslPwlFile");
#endif
volatile struct ECSL_PWL EcslPwl;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm1RegsFile")
#else
#pragma DATA_SECTION(EPwm1Regs,"EPwm1RegsFile");
#endif
volatile struct EPWM_REGS EPwm1Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm2RegsFile")
#else
#pragma DATA_SECTION(EPwm2Regs,"EPwm2RegsFile");
#endif
volatile struct EPWM_REGS EPwm2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm3RegsFile")
#else
#pragma DATA_SECTION(EPwm3Regs,"EPwm3RegsFile");
#endif
volatile struct EPWM_REGS EPwm3Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm4RegsFile")
#else
#pragma DATA_SECTION(EPwm4Regs,"EPwm4RegsFile");
#endif
volatile struct EPWM_REGS EPwm4Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm5RegsFile")
#else
#pragma DATA_SECTION(EPwm5Regs,"EPwm5RegsFile");
#endif
volatile struct EPWM_REGS EPwm5Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm6RegsFile")
#else
#pragma DATA_SECTION(EPwm6Regs,"EPwm6RegsFile");
#endif
volatile struct EPWM_REGS EPwm6Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm7RegsFile")
#else
#pragma DATA_SECTION(EPwm7Regs,"EPwm7RegsFile");
#endif
volatile struct EPWM_REGS EPwm7Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm8RegsFile")
#else
#pragma DATA_SECTION(EPwm8Regs,"EPwm8RegsFile");
#endif
volatile struct EPWM_REGS EPwm8Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm9RegsFile")
#else
#pragma DATA_SECTION(EPwm9Regs,"EPwm9RegsFile");
#endif
volatile struct EPWM_REGS EPwm9Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap1RegsFile")
#else
#pragma DATA_SECTION(ECap1Regs,"ECap1RegsFile");
#endif
volatile struct ECAP_REGS ECap1Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap2RegsFile")
#else
#pragma DATA_SECTION(ECap2Regs,"ECap2RegsFile");
#endif
volatile struct ECAP_REGS ECap2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap3RegsFile")
#else
#pragma DATA_SECTION(ECap3Regs,"ECap3RegsFile");
#endif
volatile struct ECAP_REGS ECap3Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap4RegsFile")
#else
#pragma DATA_SECTION(ECap4Regs,"ECap4RegsFile");
#endif
volatile struct ECAP_REGS ECap4Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap5RegsFile")
#else
#pragma DATA_SECTION(ECap5Regs,"ECap5RegsFile");
#endif
volatile struct ECAP_REGS ECap5Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap6RegsFile")
#else
#pragma DATA_SECTION(ECap6Regs,"ECap6RegsFile");
#endif
volatile struct ECAP_REGS ECap6Regs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EQep1RegsFile")
#else
#pragma DATA_SECTION(EQep1Regs,"EQep1RegsFile");
#endif
volatile struct EQEP_REGS EQep1Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EQep2RegsFile")
#else
#pragma DATA_SECTION(EQep2Regs,"EQep2RegsFile");
#endif
volatile struct EQEP_REGS EQep2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EQep3RegsFile")
#else
#pragma DATA_SECTION(EQep3Regs,"EQep3RegsFile");
#endif
volatile struct EQEP_REGS EQep3Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("FlashEccRegsFile")
#else
#pragma DATA_SECTION(FlashEccRegs,"FlashEccRegsFile");
#endif
volatile struct FLASH_ECC_REGS FlashEccRegs;
//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("FlashCtrlRegsFile")
#else
#pragma DATA_SECTION(FlashCtrlRegs,"FlashCtrlRegsFile");
#endif
volatile struct FLASH_CTRL_REGS FlashCtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("FlashExeOnlyFile")
#else
#pragma DATA_SECTION(FlashExeOnly,"FlashExeOnlyFile");
#endif
volatile struct FLASH_EXE_ONLY FlashExeOnly;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioCtrlRegsFile")
#else
#pragma DATA_SECTION(GpioCtrlRegs,"GpioCtrlRegsFile");
#endif
volatile struct GPIOG1_CTRL_REGS GpioCtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioDataRegsFile")
#else
#pragma DATA_SECTION(GpioDataRegs,"GpioDataRegsFile");
#endif
volatile struct GPIOG1_DATA_REGS GpioDataRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioTripRegsFile")
#else
#pragma DATA_SECTION(GpioTripRegs,"GpioTripRegsFile");
#endif
volatile struct GPIOG1_TRIP_REGS GpioTripRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioG1CtrlRegsFile")
#else
#pragma DATA_SECTION(GpioG1CtrlRegs,"GpioG1CtrlRegsFile");
#endif
volatile struct GPIOG1_CTRL_REGS GpioG1CtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioG1DataRegsFile")
#else
#pragma DATA_SECTION(GpioG1DataRegs,"GpioG1DataRegsFile");
#endif
volatile struct GPIOG1_DATA_REGS GpioG1DataRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioG1TripRegsFile")
#else
#pragma DATA_SECTION(GpioG1TripRegs,"GpioG1TripRegsFile");
#endif
volatile struct GPIOG1_TRIP_REGS GpioG1TripRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioG2CtrlRegsFile")
#else
#pragma DATA_SECTION(GpioG2CtrlRegs,"GpioG2CtrlRegsFile");
#endif
volatile struct GPIOG2_CTRL_REGS GpioG2CtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioG2DataRegsFile")
#else
#pragma DATA_SECTION(GpioG2DataRegs,"GpioG2DataRegsFile");
#endif
volatile struct GPIOG2_DATA_REGS GpioG2DataRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("HWBistRegsFile")
#else
#pragma DATA_SECTION(HWBistRegs,"HWBistRegsFile");
#endif
volatile struct HWBIST_REGS HWBistRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CtoMIpcRegsFile")
#else
#pragma DATA_SECTION(CtoMIpcRegs,"CtoMIpcRegsFile");
#endif
volatile struct CTOM_IPC_REGS CtoMIpcRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("I2caRegsFile")
#else
#pragma DATA_SECTION(I2caRegs,"I2caRegsFile");
#endif
volatile struct I2C_REGS I2caRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("McbspaRegsFile")
#else
#pragma DATA_SECTION(McbspaRegs,"McbspaRegsFile");
#endif
volatile struct McBSP_REGS McbspaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("M3PllRegsFile")
#else
#pragma DATA_SECTION(M3PllRegs,"M3PllRegsFile");
#endif
volatile struct M3PLL_REGS M3PllRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("NmiIntruptRegsFile")
#else
#pragma DATA_SECTION(NmiIntruptRegs,"NmiIntruptRegsFile");
#endif
volatile struct NMI_INTRUPT_REGS NmiIntruptRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("PieCtrlRegsFile")
#else
#pragma DATA_SECTION(PieCtrlRegs,"PieCtrlRegsFile");
#endif
volatile struct PIE_CTRL_REGS PieCtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("PieVectTableFile")
#else
#pragma DATA_SECTION(PieVectTable,"PieVectTableFile");
#endif
struct PIE_VECT_TABLE PieVectTable;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("PieVectTableCopyFile")
#else
#pragma DATA_SECTION(PieVectTableCopy,"PieVectTableCopyFile");
#endif
struct PIE_VECT_TABLE PieVectTableCopy;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("RAMRegsFile")
#else
#pragma DATA_SECTION(RAMRegs,"RAMRegsFile");
#endif
volatile struct RAM_REGS RAMRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("RAMErrRegsFile")
#else
#pragma DATA_SECTION(RAMErrRegs,"RAMErrRegsFile");
#endif
volatile struct RAM_ERR_REGS RAMErrRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("SciaRegsFile")
#else
#pragma DATA_SECTION(SciaRegs,"SciaRegsFile");
#endif
volatile struct SCI_REGS SciaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("SpiaRegsFile")
#else
#pragma DATA_SECTION(SpiaRegs,"SpiaRegsFile");
#endif
volatile struct SPI_REGS SpiaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("SysCtrlRegsFile")
#else
#pragma DATA_SECTION(SysCtrlRegs,"SysCtrlRegsFile");
#endif
volatile struct SYS_CTRL_REGS SysCtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("XIntruptRegsFile")
#else
#pragma DATA_SECTION(XIntruptRegs,"XIntruptRegsFile");
#endif
volatile struct XINTRUPT_REGS XIntruptRegs;





