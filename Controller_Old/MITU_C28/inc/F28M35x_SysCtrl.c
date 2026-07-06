//###########################################################################
// FILE:   F28M35x_SysCtrl.c
// TITLE:  F28M35x Device System Control Initialization & Support Functions.
//
// DESCRIPTION:  Example initialization of system resources.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#include "F28M35x_Device.h" // Headerfile Include File
#include "F28M35x_Examples.h" // Examples Include File

// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped to a load and
// run address using the linker cmd file.

#pragma CODE_SECTION(InitFlash, "ramfuncs");
#pragma CODE_SECTION(SetupFlash, "ramfuncs");
#pragma CODE_SECTION(FlashGainPump,"ramfuncs");
#pragma CODE_SECTION(FlashLeavePump,"ramfuncs");

//---------------------------------------------------------------------------
// InitSysCtrl:
//---------------------------------------------------------------------------
// This function initializes the System Control registers to a known state.
// - Set the pre-scaler for the high and low frequency peripheral clocks
// - Enable the clocks to the peripherals
// - Populate analog trims

void InitSysCtrl(void)
{

    // *IMPORTANT*
    // The Device_cal function MUST be called
    // for the ADC and oscillators to function according
    // to specification. The clocks to the ADC MUST be enabled before calling
    // this function. See the device data manual and/or the ADC Reference
    // Manual for more information.

    (**InitAnalogSystemClock)(ACLKDIV4);

    EALLOW;
    // Initialize the Analog Sub-System and set the clock divider to divide by 4
    while((**AnalogClockEnable)(AnalogConfig1,ADC1_ENABLE));    // Enable ADC 1
    while((**AnalogClockEnable)(AnalogConfig2,ADC2_ENABLE));    // Enable ADC 2
    
    // Reset both ADC in case the last reset was a debugger reset (which doesn't 
    // reset the ADCs
    Adc1Regs.ADCCTL1.bit.RESET = 1;
    Adc2Regs.ADCCTL1.bit.RESET = 1;
    // Wait to ensure ADCs are out of reset before device cal is called
    __asm(" nop");
    __asm(" nop");
    
    
    // Calibrate the device for temperature
    (**Device_Cal)();
    
    while((**AnalogClockDisable)(AnalogConfig1,ADC1_ENABLE));   // Disable ADC1
    while((**AnalogClockDisable)(AnalogConfig2,ADC2_ENABLE));   // Disable ADC2

    RAMRegs.CSxMSEL.bit.S6MSEL = 1;
    RAMRegs.CSxMSEL.bit.S7MSEL = 1;


    EDIS;

    // Initialize the peripheral clocks
    InitPeripheralClocks();
}


   //---------------------------------------------------------------------------
   // Example: InitFlash:
   //---------------------------------------------------------------------------
   // This function initializes the Flash Control registers

   //                   CAUTION
   // This function MUST be executed out of RAM. Executing it
   // out of OTP/Flash will yield unpredictable results

   void InitFlash(void)
   {
       EALLOW;
       
       //At reset bank and pump are in sleep
       //A Flash access will power up the bank and pump automatically
       //After a Flash access, bank and pump go to low power mode (configurable in FBFALLBACK/FPAC1 registers)-
       //if there is no further access to flash 
       //Power up Flash bank and pump and this also sets the fall back mode of flash and pump as active
       FlashCtrlRegs.FPAC1.bit.PMPPWR = 0x1;
       FlashCtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3; 
       
       //Disable Cache and prefetch mechanism before changing wait states
       FlashCtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
       FlashCtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 0; 
       
       //Set waitstates according to frequency        
       //                CAUTION
       //Minimum waitstates required for the flash operating
       //at a given CPU rate must be characterized by TI.
       //Refer to the datasheet for the latest information.
       #if CPU_FRQ_150MHZ
       FlashCtrlRegs.FRDCNTL.bit.RWAIT = 0x3;
       #endif
    
       #if CPU_FRQ_100MHZ
       FlashCtrlRegs.FRDCNTL.bit.RWAIT = 0x2;
       #endif
       
       #if CPU_FRQ_60MHZ
       FlashCtrlRegs.FRDCNTL.bit.RWAIT = 0x1;
       #endif              
       
       //Enable Cache and prefetch mechanism to improve performance
       //of code executed from Flash.
       FlashCtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
       FlashCtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 1; 
         
       //At reset, ECC is enabled
       //If it is disabled by application software and if application again wants to enable ECC
       FlashEccRegs.ECC_ENABLE.bit.ENABLE = 0xA;
    
       EDIS;
    
       //Force a pipeline flush to ensure that the write to
       //the last register configured occurs before returning.
    
       __asm(" RPT #7 || NOP");        
                
   }

   //---------------------------------------------------------------------------
   // Example: SetupFlash:
   //---------------------------------------------------------------------------
   // This function initializes the Flash Control registers

   //                   CAUTION
   // This function MUST be executed out of RAM. Executing it
   // out of OTP/Flash will yield unpredictable results

   void SetupFlash(unsigned short waitStates)
   {
       EALLOW;
       
       //At reset bank and pump are in sleep
       //A Flash access will power up the bank and pump automatically
       //After a Flash access, bank and pump go to low power mode (configurable in FBFALLBACK/FPAC1 registers)-
       //if there is no further access to flash 
       //Power up Flash bank and pump and this also sets the fall back mode of flash and pump as active
       FlashCtrlRegs.FPAC1.bit.PMPPWR = 0x1;
       FlashCtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3; 
       
       //Disable Cache and prefetch mechanism before changing wait states
       FlashCtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
       FlashCtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 0; 
       
       //Set waitstates according to frequency        
       //                CAUTION
       //Minimum waitstates required for the flash operating
       //at a given CPU rate must be characterized by TI.
       //Refer to the datasheet for the latest information.

       FlashCtrlRegs.FRDCNTL.bit.RWAIT = waitStates;
           
       
       //Enable Cache and prefetch mechanism to improve performance
       //of code executed from Flash.
       FlashCtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
       FlashCtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 1; 
         
       //At reset, ECC is enabled
       //If it is disabled by application software and if application again wants to enable ECC
       FlashEccRegs.ECC_ENABLE.bit.ENABLE = 0xA;
    
       EDIS;
    
       //Force a pipeline flush to ensure that the write to
       //the last register configured occurs before returning.
    
       __asm(" RPT #7 || NOP");        
                
   }

//******************************************************************************
//  Functions to gain and relinquish control on Flash pump
//******************************************************************************

void FlashGainPump (void)
{
	EALLOW;
	
    // Wait for flashpump to be idle
    while(CtoMIpcRegs.CPUMPREQUEST != 0){}
    
    // Take control of the flash pump
    CtoMIpcRegs.CPUMPREQUEST = 0x4CE73950 | 0x1;
    
    EDIS;
}





void FlashLeavePump (void)
{
    EALLOW;
    
    // Leave control of the flash pump
    CtoMIpcRegs.CPUMPREQUEST = 0x4CE73950 | 0x0;
    
    EDIS;
}





//--------------------------------------------------------------------------
// Example: InitPeripheralClocks:
//---------------------------------------------------------------------------
// This function initializes the clocks to the peripheral modules.
// First the high and low clock prescalers are set
// Second the clocks are enabled to each peripheral.
// To reduce power, leave clocks to unused peripherals disabled
// Note: If a peripherals clock is not enabled then you cannot
// read or write to the registers for that peripheral

void InitPeripheralClocks(void)
{
    // LOSPCP prescale register settings, normally it will be set to default
    // values
    EALLOW;
    SysCtrlRegs.LOSPCP.all = 0x0002;
    EDIS;

    // Initialize the Analog Sub-System and set the clock divider to divide by 4
    (**InitAnalogSystemClock)(ACLKDIV4);

    EALLOW;
    while((**AnalogClockEnable)(AnalogConfig1,ADC1_ENABLE)); // Enable ADC 1
    while((**AnalogClockEnable)(AnalogConfig2,ANALOGCONFIG2ALL)); // Enable 
                                                                  // ADC 2,
                                                                  // COMP1,2,3,
                                                                  // 4,5,6

    // Peripheral clock enables set for the selected peripherals.
    // If you are not using a peripheral leave the clock off
    // to save on power.
    // This function is not written to be an example of efficient code.

    //SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1; // eCAP1
    //SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1; // eCAP2
    //SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 1; // eCAP3
    //SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 1; // eCAP4
    //SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 1; // eCAP5
    //SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 1; // eCAP6

    //SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1; // eQEP1
    //SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1; // eQEP2
    //SysCtrlRegs.PCLKCR2.bit.EQEP3ENCLK = 1; // eQEP3

    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1; // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1; // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1; // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1; // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1; // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1; // ePWM6
    SysCtrlRegs.PCLKCR1.bit.EPWM7ENCLK = 1; // ePWM7
    //SysCtrlRegs.PCLKCR1.bit.EPWM8ENCLK = 1; // ePWM8
    //SysCtrlRegs.PCLKCR2.bit.EPWM9ENCLK = 1; // ePWM9

    SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;   // DMA
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // Timer 0
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // Timer 1
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // Timer 2

    //SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   // SPI-A
    //SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;   // I2C-A
    //SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 1; // McBSP-A

    //SysCtrlRegs.PCLKCR0.bit.HRPWMENCLK = 1; // HRPWM
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;  // Enable TBCLK within the ePWM

    EDIS;
}
//---------------------------------------------------------------------------
// Example: CsmUnlock:
//---------------------------------------------------------------------------
// This function unlocks the CSM. User must replace the 0xFFFF FFFF's assigned
// to CSMRegs.CSMKEY0 to CSMRegs.CSMKEY3 with the correct password.
void CsmUnlock(void)
{
   // if the CSM flash memory location is different from 0xFFFF FFFF unlock the
   // device by providing the right password.

    if((CsmPwl.PSWD0 & CsmPwl.PSWD1 & CsmPwl.PSWD2 & CsmPwl.PSWD3) != 0xFFFFFFFF)
    {

        EALLOW;
        CsmRegs.CSMKEY0 = 0xFFFFFFFF;
        CsmRegs.CSMKEY1 = 0xFFFFFFFF;
        CsmRegs.CSMKEY2 = 0xFFFFFFFF;
        CsmRegs.CSMKEY3 = 0xFFFFFFFF;
        EDIS;

    }
}
//---------------------------------------------------------------------------
// Example: EcslUnlock:
//---------------------------------------------------------------------------
// This function unlocks the ECSL. User must replace 0xFFFFFFFF's assigned to
// CSMRegs.ECSLKEY0 and CSMRegs.ECSLKEY1 with the correct password.
void EcslUnlock(void)
{
    // if the ECSL flash memory location is different from 0xFFFF FFFF unlock the
    // device by providing the right password.

    if((EcslPwl.PSWD0 & EcslPwl.PSWD1 ) != 0xFFFFFFFF)
    {
        EALLOW;
        CsmRegs.ECSLKEY0 = 0xFFFFFFFF;
        CsmRegs.ECSLKEY1 = 0xFFFFFFFF;
        EDIS;
    }
}
//---------------------------------------------------------------------------
// Example: LockDevice:
//---------------------------------------------------------------------------
// This function locks the device, once a password is programmed into CSM or
// ECSL password lock flash location
void LockDevice(void)
{
    EALLOW;
    CsmRegs.CSMCR.bit.FORCESEC = 1;
    EDIS;

}
//---------------------------------------------------------------------------
// Example: GetEXEstatus:
//---------------------------------------------------------------------------
// This function returns the EXE-ONLY status of a sector. Returns 0 if the
// sector is not EXE-ONLY security protected, 1 if the sector is EXE-ONLY
// security protected. Input values for sector argument are 0 to 13
// corresponding to the Flash sectors
Uint16 GetEXEstatus(Uint16 sector)
{
    Uint32 temp;

    // Read EXE-ONLY flash location to transfer data to EXE- ONLY register
    temp = FlashExeOnly.FLASH_EXE_ONLY_REG;
    if((( CsmRegs.EXEONLYR.all  >> sector) & 0x00000001) == 1)
    {
        return(1);
    }
    else
    {
        return(0);
    }

}
//---------------------------------------------------------------------------
// Example: CSMSecurityStatus:
//---------------------------------------------------------------------------
// This function returns the security status of CSM. Returns 0 if CSM
// is not secure and 1 if secure.
Uint16 CSMSecurityStatus(void)
{
    // If ARMED bit is one and ALLONE or MATCH bits or both are one, 
    // then CSM is unsecure
    if((CsmRegs.CSMCR.bit.CSM_ARMED) && 
       (CsmRegs.CSMCR.bit.CSM_ALLONE || CsmRegs.CSMCR.bit.CSM_MATCH))
    {
        return(0);
    }
    else
    {
        return(1);
    }

}
//---------------------------------------------------------------------------
// Example: ECSLSecurityStatus:
//---------------------------------------------------------------------------
// This function returns the security status of ECSL. Returns 0 if ECSL
// is not secure and 1 if secure.
Uint16 ECSLSecurityStatus(void)
{
    // if ARMED bit is one and ALLONE or MATCH bits or both are one, then ECSL is unsecure
    if((CsmRegs.CSMCR.bit.ECSL_ARMED) && 
       (CsmRegs.CSMCR.bit.ECSL_ALLONE || CsmRegs.CSMCR.bit.ECSL_MATCH))
    {
        return(0);
    }
    else
    {
        return(1);
    }
}

