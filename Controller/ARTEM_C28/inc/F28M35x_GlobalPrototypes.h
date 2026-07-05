//###########################################################################
// FILE:   F28M35x_GlobalPrototypes.h
// TITLE:  Global prototypes for 28M35x Examples
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_GLOBALPROTOTYPES_H
#define F28M35x_GLOBALPROTOTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*---- shared global function prototypes -----------------------------------*/

// Analog Functions
#define AnalogClockEnable (unsigned short   (**)(unsigned short \
                                                 AnalogConfigReg, \
                                                 unsigned short \
                                                  AnalogClockMask))0x00240214
#define AnalogClockDisable (unsigned short   (**) (unsigned short \
                                                   AnalogConfigReg, \
                                                   unsigned short \
                                                   AnalogClockMask)) 0x00240216
#define ReadAnalogClockStatus (unsigned short (**) (unsigned short \
                                                    AnalogConfigReg)) \
    0x00240218
#define InitAnalogSystemClock (unsigned short (**) (unsigned short ClockDivider)) \
    0x0024021E
#define Device_Cal (void (**) (void)) 0x00240220
//////////////////////////////////////////// ADC1 Functions
extern void InitAdc1(void);
extern void InitAdc1Aio(void);
extern void Adc1OffsetSelfCal(void);
extern void Adc1ChanSelect(Uint16 ch_no);
extern Uint16 Adc1Conversion (void);
// ADC2 Functions
extern void InitAdc2(void);
extern void InitAdc2Aio(void);
extern void Adc2OffsetSelfCal(void);
extern void Adc2ChanSelect(Uint16 ch_no);
extern Uint16 Adc2Conversion (void);

extern void InitPeripherals(void);

extern void InitCompGpio(void);
extern void InitComp1Gpio(void);
extern void InitComp2Gpio(void);
extern void InitComp3Gpio(void);
extern void InitComp4Gpio(void);
extern void InitComp5Gpio(void);
extern void InitComp6Gpio(void);

// DMA Functions
extern void DMAInitialize(void);
// DMA Channel 1
extern void DMACH1AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH1BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH1TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH1WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize,
                             int16 deswstep);
extern void DMACH1ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot,
                             Uint16 cont, Uint16 synce, Uint16 syncsel,
                             Uint16 ovrinte, Uint16 datasize,
                             Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH1(void);
// DMA Channel 2
extern void DMACH2AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH2BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH2TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH2WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize,
                             int16 deswstep);
extern void DMACH2ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot,
                             Uint16 cont, Uint16 synce, Uint16 syncsel,
                             Uint16 ovrinte, Uint16 datasize,
                             Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH2(void);
// DMA Channel 3
extern void DMACH3AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH3BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH3TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH3WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize,
                             int16 deswstep);
extern void DMACH3ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot,
                             Uint16 cont, Uint16 synce, Uint16 syncsel,
                             Uint16 ovrinte, Uint16 datasize,
                             Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH3(void);
// DMA Channel 4
extern void DMACH4AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH4BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH4TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH4WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize,
                             int16 deswstep);
extern void DMACH4ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot,
                             Uint16 cont, Uint16 synce, Uint16 syncsel,
                             Uint16 ovrinte, Uint16 datasize,
                             Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH4(void);
// DMA Channel 5
extern void DMACH5AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH5BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH5TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH5WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize,
                             int16 deswstep);
extern void DMACH5ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot,
                             Uint16 cont, Uint16 synce, Uint16 syncsel,
                             Uint16 ovrinte, Uint16 datasize,
                             Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH5(void);
// DMA Channel 6
extern void DMACH6AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH6BurstConfig(Uint16 bsize,Uint16 srcbstep, int16 desbstep);
extern void DMACH6TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH6WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize,
                             int16 deswstep);
extern void DMACH6ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot,
                             Uint16 cont, Uint16 synce, Uint16 syncsel,
                             Uint16 ovrinte, Uint16 datasize,
                             Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH6(void);

// ECAP Functions
extern void InitECap(void);
extern void InitECapGpio(void);
extern void InitECap1Gpio(void);
extern void InitECap2Gpio(void);
extern void InitECap3Gpio(void);
extern void InitECap4Gpio(void);
extern void InitECap5Gpio(void);
extern void InitECap6Gpio(void);

// EPWM Functions
extern void InitEPwm(void);
extern void InitEPwmGpio(void);
extern void InitEPwm1Gpio(void);
extern void InitEPwm2Gpio(void);
extern void InitEPwm3Gpio(void);
extern void InitEPwm4Gpio(void);
extern void InitEPwm5Gpio(void);
extern void InitEPwm6Gpio(void);
extern void InitEPwm7Gpio(void);
extern void InitEPwm8Gpio(void);
extern void InitEPwm9Gpio(void);

// EQEP Functions
extern void InitEQep(void);
extern void InitEQepGpio(void);
extern void InitEQep1Gpio(void);
extern void InitEQep2Gpio(void);
extern void InitEQep3Gpio(void);

// GPIO Functions
extern void InitGpio(void);

// I2C Functions
extern void InitI2CGpio(void);

// McBSP Functions
extern void InitMcbsp(void);
extern void InitMcbspa(void);
extern void InitMcbspaInt(void);
extern void delay_loop(void);
extern void clkg_delay_loop(void);
extern void InitMcbspGpio(void);
extern void InitMcbspaGpio(void);
extern void InitMcbspa8bit(void);
extern void InitMcbspa12bit(void);
extern void InitMcbspa16bit(void);
extern void InitMcbspa20bit(void);
extern void InitMcbspa24bit(void);
extern void InitMcbspa32bit(void);

// PIE Functions
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);

// SCI Functions
extern void InitSci(void);
extern void InitSciGpio(void);
extern void InitSciaGpio(void);

// SPI Functions
extern void InitSpi(void);
extern void InitSpiGpio(void);
extern void InitSpiaGpio(void);

// Other Functions
extern void InitSysCtrl(void);
extern void InitTzGpio(void);
extern void InitXIntrupt(void);
extern void InitPeripheralClocks(void);
extern void EnableInterrupts(void);
extern void DSP28x_usDelay(Uint32 Count);


// Security Functions
extern void CsmUnlock(void);
extern void EcslUnlock(void);
extern void LockDevice(void);
extern Uint16 GetEXEstatus(Uint16 sector);
extern Uint16 CSMSecurityStatus(void);
extern Uint16 ECSLSecurityStatus(void);


// Temperature Sensor Functions
extern int16 GetTemperatureC(int16 sensorSample); // returns temp in deg. C
extern int16 GetTemperatureK(int16 sensorSample); // returns temp in deg. K

// DSP28_DBGIER.asm
extern void SetDBGIER(Uint16 dbgier);

//                 CAUTION
// These functions MUST be executed out of RAM. Executing them
// out of OTP/Flash will yield unpredictable results
extern void InitFlash(void);
extern void SetupFlash(unsigned short waitStates);
extern void FlashGainPump();
extern void FlashLeavePump();

//---------------------------------------------------------------------------
// External symbols created by the linker cmd file
// DSP28 examples will use these to relocate code from one LOAD location
// in Flash to a different RUN location in internal
// RAM
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of F28M35x_GLOBALPROTOTYPES_H



