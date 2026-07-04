/*
 * PCON_C28.h
 *
 *  Created on: 11.12.2014
 *      Author: VKD
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "InitFunctions.h"
#include "MemFunctions.h"
#include "pid.h"
#include "Filters.h"
#include "math.h"
#include "ERROR_NUMBERS.h"
#include <string.h>
#include "TimedProt.h"

#ifndef PCON_C28_H_
#define PCON_C28_H_



#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#endif

#define PRECHARGE_DC_VOLTAGE    100
#define NOMINAL_DC_VOLTAGE    570
#define MINIMAL_BOOST_DC_VOLTAGE    550
#define MINIMAL_DC_VOLTAGE    500
#define MINIMAL_EXTREME_DC_VOLTAGE    450
#define MINIMAL_EXTREME_UGEN_VOLTAGE    100

#define PRECHARGE_EXC_CURRENT    3


#define C28_FREQ    100         //CPU frequency in MHz
#define ADC_SAMPLE_CYCLE_US    25      //Cycle time for ADC sampling. MUST BE DIVISOR FOR CYCLE LENGTH
#define ADC_SAMPLE_DATA_DIVIDER   4      //Divider for getting data to CPU.
#define ADC_SAMPLE_CYCLE_SHIFT    50      //Cycles to sync measurments with adc sample
#define MAIN_CYCLE_US    300       	   //Cycle time for main calculation cycle wake up

#define MEASURMENTS_BUFFER_SIZE    128  //IRQ buffer for measured values. !!!!!!!!!!! MUST BE 64x aligned for DMA TRANSFERS
#define POWER_MEASURMENTS_BUFFER_SIZE    128
#define MEASURMENTS_BUFFER_SIZE_HALF    64  //IRQ buffer for measured values half size !!!!!!!!!!! MUST BE 32x aligned for DMA TRANSFERS
#define DMA_CHANNELS_TO_TRANSFER 30
#define SHMEM_BUFFER_SIZE    1024  //Shmem buffer size for data transfers. Size must be greater than MEASURMENTS_BUFFER_SIZE_HALFxDMA_CHANNELS_TO_TRANSFER
#define DATA_PACKS_TOTAL    33
#define BUF_COUNT 7

#define FPGA_INPUT_DATA_SIZE 11
#define FPGA_CRITICAL_INPUT_DATA_SIZE 7
#define FPGA_OUTPUT_DATA_SIZE 18
#define FPGA_OUTPUT_OFFSET 50
#define CTOM_DATA_TRANSMITT_COUNT 100
#define MTOC_DATA_TRANSMITT_COUNT 100

#define SETUP_DATA_SIZE 600
#define SETUP_DATA_START_ADDR 200
#define SETUP_UINT_DATA_COUNT 50
#define SETUP_FLOAT_DATA_COUNT 150

#define OSCI_SETUP_DATA_START_ADDR 1000



#define OSCI_SOURCES_COUNT 52

extern interrupt void adc1_isr(void);
extern void FPGA_Read_Data(void);
extern void FPGA_Write_Data (void);
extern void FPGA_Write_Exitation(void);
extern void FPGA_Write_Aux (void);
extern void FPGA_Write_Booster (void);
extern void FPGA_Write_Amp (void);
extern void FPGA_Write_Phase_Freq (void);
extern void FPGA_EXTOUT_SetBit(Uint16 bitnum);
extern void FPGA_EXTOUT_ClearBit(Uint16 bitnum);
extern void M3_Read_Data (void);
extern void M3_Write_Data (Uint32 CyclesCounter);
extern void DMADataTransferSet(void);
extern void ActivateSynchronizer(Uint16 Activate);
extern void CalcFundamentalValues(void);
extern void SignalProcessing(void);
extern void CommandsProcess(void);
extern void MeanOffsetCalc(void);
extern interrupt void  adc1_isr(void);
extern void FPGA_Read_Data (Uint16 CyclesCounter);
extern void FPGA_Get_Data (void);
extern void FPGA_Read_Init (void);
extern void FPGA_Write_Data (void);
extern void FPGA_Write_DMA (void);
extern void M3_Read_Data (void);
extern void M3_Write_Data (Uint32 CyclesCounter);
extern void ErrorSet(Uint32 ErrNum);
extern void ErrorReset(void);
extern void FastProtections(void);
extern void ErrorRemove(void);
extern void SetPhAOCP(Uint16 SetBit);
extern void SetPhBOCP(Uint16 SetBit);
extern void SetPhCOCP(Uint16 SetBit);
extern void SetChopperOCP(Uint16 SetBit);
extern void SetExcitationOCP(Uint16 SetBit);
extern void ActivateInverter(Uint16 Activate);
extern void ActivateChopper(Uint16 Activate);
extern void ActivateBrake(Uint16 Activate);
extern void ActivateExcitation(Uint16 Activate);
extern void ActivateBreakerControl(Uint16 Activate);
extern void ActivateSyncout(Uint16 Activate);
extern void InitData(void);
extern void CalculateValues(void);
extern float CalculateNTC(Uint16 value);
extern void CalculatePower(void);
extern void RotationSpeedMeasurments(void);
extern void ProtectionsCheck(void);
extern void SynchronizerOperation(void);
extern void ExcitationRegulation(void);
extern void DCBoostRegulation(void);
extern void RotationSpeedRegulation(void);
extern void ActivePowerRegulation(void);
extern void ReactivePowerRegulation(void);
extern void StateRecognition(void);
extern void CoolingControl(void);
extern void UnitLoadRegulation();
extern void M3_Get_Setups(void);
extern void Setups_Init(void);
extern void Setups_WriteUintParam(Uint16 ID, Uint16 Value);
extern Uint16 Setups_ReadUintParam(Uint16 ID);
extern void Setups_WriteFloatParam(Uint16 ID, float Value);
extern float Setups_ReadFloatParam(Uint16 ID);
extern Uint16 ReadDataFromFlash(void);
extern void SaveDataToFlash(void);
extern Uint16 SyncReadWordFromFlash(Uint32 Addr);
extern void SyncWriteWordToFlash(Uint32 Addr, Uint16 Data);
extern void OsciChannelsInit(void);
extern void EraseFlashData(void);
extern Uint16 ReadOffsetsFromFlash(void);
extern void SaveOffsetsToFlash(void);
extern void PIDReInit(void);
extern Uint16 GetDINValue(Uint16 InputNum);

extern volatile Uint16 DMABufFPGA1[100];

extern volatile Uint16 *DMADestFPGA;
extern volatile Uint16 *DMASourceFPGA;
extern volatile Uint16 transfer_countFPGA;

extern Uint16 ButtonCounter;
extern Uint16 ButtonCounter2;


extern TimedProtection PR_DCOvervoltage;
extern TimedProtection PR_RectUnbalance;
extern TimedProtection PR_IGBTCurrUnbalance;
extern TimedProtection PR_IGBTAOverCurr;
extern TimedProtection PR_IGBTBOverCurr;
extern TimedProtection PR_IGBTCOverCurr;
extern TimedProtection PR_OutpVoltageUnbalance;
extern TimedProtection PR_NegativeAPower;
extern TimedProtection PR_NegativeRPower;
extern TimedProtection PR_APOverload;
extern TimedProtection PR_RPOverload;
extern TimedProtection PR_OutputFrequency;

extern TimedProtection PR_ActivePowerRegulation;
extern TimedProtection PR_GroundFault;

extern TimedProtection PR_OutpPhAOverCurr;
extern TimedProtection PR_OutpPhBOverCurr;
extern TimedProtection PR_OutpPhCOverCurr;

extern Uint16* UintSetups[SETUP_UINT_DATA_COUNT];
extern float*  FloatSetups[SETUP_FLOAT_DATA_COUNT];
extern Uint16 Const_CurrentSetupVersion1;
extern Uint16 Const_CurrentSetupVersion2;
extern Uint16 Const_CurrentSetupVersion3;
extern Uint16 Const_CurrentSetupVersion4;

extern float*  OsciSources[SETUP_FLOAT_DATA_COUNT];
extern Uint16  SOC[SETUP_UINT_DATA_COUNT];
extern float  OsciSourcesGain[SETUP_FLOAT_DATA_COUNT];
extern float  OsciSourcesOffset[SETUP_FLOAT_DATA_COUNT];

extern Uint16 Uint_DummyVariable;
extern float  Float_DummyVariable;


extern float VPID_P;
extern float VPID_I;
extern float VPID_G;

extern float RPPID_P;
extern float RPPID_I;
extern float RPPID_G;


extern float Const_ReactivePowerMax;
extern float Const_ReactivePowerMin;
extern float Const_ReactivePowerLimiter;

extern float APPID_P;
extern float APPID_I;

extern float FRPID_P;
extern float FRPID_I;
extern float FRPID_D;
extern float FRPID_G;
extern float Const_FreqUpperLim;
extern float Const_FreqLowerLim;
extern float Const_FreqPropLimit;
extern float Const_FreqIntLimit;

extern float PHPID_P;
extern float PHPID_I;
extern float PHPID_G;
extern float Const_PhasePropLimit;
extern float Const_PhaseIntLimit;

extern float DCVPID_P;
extern float DCVPID_I;
extern float DCVPID_POSG;
extern float DCVPID_NEGG;
extern float Const_ExcitationMax;
extern float EXCPID_P;
extern float EXCPID_I;
extern float Const_BaseFrequency;
extern float Const_FrequencyWindow;

extern float Const_SyncMaxPhaseChange;
extern float Const_SyncMaxFreqChange;
extern float Const_SyncMinVoltageMag;
extern float Const_SyncStarPhaseLead;

extern float Prot_ExcCurrentMax;
extern float Prot_DCVoltageMax;
extern float Prot_IGBTTempMax;
extern float Prot_IGBTTempUnbalance;
extern float Prot_GenVoltageMax;
extern float Prot_GenSpeedMax;
extern float Prot_RectCurrUnbalance;
extern float Prot_OutpVoltageUnbalance;
extern float Prot_OutpCurrentUnbalance;
extern float Prot_IGBTCurrentUnbalance;
extern float Prot_OutpOverVoltage;
extern float Prot_OutpOverCurrent;
extern float Prot_APOverload;
extern float Prot_APNegative;
extern float Prot_RPOverload;
extern float Prot_DCVoltageLow;
extern float Prot_GenSpeedOPLow;
extern float Prot_GenSpeedOPHigh;
extern float Prot_OutpFrequencyHigh;
extern float Prot_OutpFrequencyLow;

extern float ProtRMS_RectCurrent;
extern float ProtRMS_OutpVoltage;
extern float ProtRMS_UDCVoltage;
extern float ProtRMS_NeutralCurrent;
extern float ProtRMS_NeutralIGBCurrent;
extern float ProtRMS_ExcCurrent;
extern float ProtRMS_IGBTTemp;
extern float ProtRMS_IGBTCurrent;
extern float ProtRMS_OutpCurrent;

extern float Const_ExcMaxVoltage;
extern float Const_ExcCurrentBoost;
extern float Const_ExcPIDBoost;

extern float Const_TPR_DCLower;
extern float Const_TPR_DCUpper;
extern float Const_TPR_RectCurrLower;
extern float Const_TPR_RectCurrUpper;
extern float Const_TPR_VoutLower;
extern float Const_TPR_VoutUpper;
extern float Const_TPR_IGBTUnbLower;
extern float Const_TPR_IGBTUnbUpper;
extern float Const_TPR_IGBTOCPLower;
extern float Const_TPR_IGBTOCPUpper;
extern float Const_TPR_APHighLower;
extern float Const_TPR_APHighUpper;
extern float Const_TPR_APLowLower;
extern float Const_TPR_APLowUpper;
extern float Const_TPR_RPHighLower;
extern float Const_TPR_RPHighUpper;
extern float Const_TPR_RPLowLower;
extern float Const_TPR_RPLowUpper;
extern float Const_TPR_FreqLower;
extern float Const_TPR_FreqUpper;

extern float Const_UnderloadBoostPerVolt;
extern float Const_OverloadBoostPerVolt;

extern Uint16 Const_DCPRMinCycles;
extern Uint16 Const_DCPRMaxCycles;
extern Uint16 Const_DCPRReleaseCycles;
extern Uint16 Const_RectPRMinCycles;
extern Uint16 Const_RectPRMaxCycles;
extern Uint16 Const_RectPRReleaseCycles;
extern Uint16 Const_VoutPRMinCycles;
extern Uint16 Const_VoutPRMaxCycles;
extern Uint16 Const_VoutPRReleaseCycles;
extern Uint16 Const_IGBTUnbPRMinCycles;
extern Uint16 Const_IGBTUnbPRMaxCycles;
extern Uint16 Const_IGBTUnbPRReleaseCycles;
extern Uint16 Const_IGBTOCPPRMinCycles;
extern Uint16 Const_IGBTOCPPRMaxCycles;
extern Uint16 Const_IGBTOCPPRReleaseCycles;
extern Uint16 Const_APHighPRMinCycles;
extern Uint16 Const_APHighPRMaxCycles;
extern Uint16 Const_APHighPRReleaseCycles;
extern Uint16 Const_APLowPRMinCycles;
extern Uint16 Const_APLowPRMaxCycles;
extern Uint16 Const_APLowPRReleaseCycles;
extern Uint16 Const_RPHighPRMinCycles;
extern Uint16 Const_RPHighPRMaxCycles;
extern Uint16 Const_RPHighPRReleaseCycles;
extern Uint16 Const_RPLowPRMinCycles;
extern Uint16 Const_RPLowPRMaxCycles;
extern Uint16 Const_RPLowPRReleaseCycles;

extern Uint16 Const_FreqPRMinCycles;
extern Uint16 Const_FreqPRMaxCycles;
extern Uint16 Const_FreqPRReleaseCycles;

extern Uint16 Const_APOscPRMinCycles;
extern Uint16 Const_APOscPRMaxCycles;
extern Uint16 Const_APOscPRReleaseCycles;
extern float Const_TPR_APOscLower;
extern float Const_TPR_APOscUpper;

extern Uint16 Const_GFaultPRMinCycles;
extern Uint16 Const_GFaultPRMaxCycles;
extern Uint16 Const_GFaultPRReleaseCycles;

extern Uint16 Const_RecalcOperationValues;

extern Uint16 Const_ActivateLoadControl;
extern Uint16 Const_BoosterModeDisable;
extern Uint16 Const_BrakeModeDisable;
extern Uint16 Const_IsConnectedToResistiveLoad;
extern Uint16 Const_IsResistiveLoadConverter;
extern Uint16 Const_SyncLaunchLocked;
extern Uint16 Const_SyncShortLaunch;
extern Uint16 Const_SyncCyclesToRun;

extern float Const_MaxOutputPower;
extern float Const_MinOutputPower;
extern float Const_GeneratorToLoadMaxPower;
extern float Const_ResitiveLoadMaxPower;
extern float Const_MaxVoltageBoost;
extern float Const_SpeedLimitMaxPower;
extern float Const_SpeedLimitFreerun;
extern float Const_TurbineLimitLine_K;
extern float Const_TurbineLimitLine_B;
extern float Const_VoltageErrorUpperLimit;
extern float Const_VoltageErrorLowerLimit;
extern float Const_OutputVoltageSetpoint;
extern float Const_ReactivePowerBoost;
extern float Const_BaseFrequency;
extern float Const_FrequencyWindow;
extern float Const_ActiveSetpMax;
extern float Const_ActiveSetpMin;


extern float ChGain[20];
extern float ADCOffset;

extern Uint16 Const_TopACurrentLimit;
extern Uint16 Const_TopBCurrentLimit;
extern Uint16 Const_TopCCurrentLimit;
extern Uint16 Const_BotACurrentLimit;
extern Uint16 Const_BotBCurrentLimit;
extern Uint16 Const_BotCCurrentLimit;

extern Uint16 Booster_Voltage_Setpoint;
extern Uint16 Booster_Current_Limit;
extern Uint16 Brake_PWM_Width;

extern SecOrdFilter* OutputFrequencyFilter;
extern float OutputFrequencyFiltered;

extern Uint16 CPU_OverloadFlag;

extern Uint16 CurveSelect;

extern Uint16 MainCyclesCounter;
extern Uint16 CalcCycleCounter;
extern Uint32 FreeTimeCounter;
extern Uint32 WaitCyclesCounter;
extern Uint16 PrevMeasurmentCounterValue;

extern Uint32 LastMeasuredTimerValue;
extern Uint32 CalculationsTimerValue;
extern Int32 CalculationsDelay;

extern Uint32 InverterActivatedCounts;

extern float StartingVoltageMagnitude;
extern SecOrdFilter* UDCFilter;
extern SecOrdFilter* UDCFilterSlow;
extern float UDCOUTFiltered;
extern float UDCOUTFilteredSlow;
extern float BaseDCVoltage;
extern float StartingPWMAmp;

extern Uint16 TryToSyncCounter;
extern Uint16 SynchronizerActive;
extern Uint16 SyncLaunch;
extern Uint16 FreqUpdateCounter;
extern Uint16 Const_FreqUpdateDelay;
extern float PhaseMinValue;
extern float PhaseMaxValue;
extern float FreqMinValue;
extern float FreqMaxValue;
extern float PhaseChangeAmp;
extern float FreqChangeAmp;


extern float FVAL_InputFreq;
extern float FVAL_InputPhase;
extern float FVAL_InputSin;
extern float FVAL_InputSin_p2pidiv3;
extern float FVAL_InputSin_m2pidiv3;
extern float FVAL_InputCos;
extern float FVAL_InputCos_p2pidiv3;
extern float FVAL_InputCos_m2pidiv3;
extern float Const_sqrt3_div2;
extern float Const_2pi_div3;
extern float Const_2_div3;
extern float Const_1_div360;
extern float Const_pi;
extern float Const_2pi;
extern float Const_CycleTime;
//extern float Const_50MHzCountsForPeriod;

extern Uint16 Const_RMSCalcCount;


extern float Ptransform_d;
extern float Ptransform_q;
extern SecOrdFilter* dFilter;
extern SecOrdFilter* qFilter;
extern float FVAL_VoltageMagnitude;
extern float FVAL_VoltagePhase;
extern float FVAL_VoltagePhaseArray[MEASURMENTS_BUFFER_SIZE];

extern Int16 DelayedPhasePointer;
extern Int16 Const_DelayedPhaseCounts;
extern float FVAL_VoltageDelayedPhase;
extern float FVAL_VoltagePhaseChange;
extern float FVAL_MeasuredOutputFrequency;
extern float Const_DelayedPhaseFrequency;
extern float MeasuredOutputFrequency;
extern SecOrdFilter* FreqFilter;
extern float PIDActivePowerSetpoint;
extern float PIDOutputVoltageSetpoint;
extern float UDCErr;

extern volatile float Const_GeneratorMomentLimit; //MomentLimit N*m

extern Uint16 FlashStatus;

extern Uint16 SetTryToResetError;

extern float DCPower;
extern Uint16 DCPowerUint16;
extern float OutputRMSVoltage;


extern Uint16 RMSCyclesCount;
extern Uint16 RMS_Count_ValueNum;
extern Int32 MeanValues[20];
extern float MeanValues_Buffers[20];
extern float MeanValues_SummResults[20];

extern Uint32 MainPWM_AMP;
extern float MainPWM_AMP_F;
extern Uint32 MainPWM_OutputFrequency;
extern Uint32 MainPWM_PhaseShift;
extern Uint32 MainPWM_PhaseSet;
extern Uint32 ChopperPWM_Width;
extern Uint32 ExcitationPWM_Width;
extern float ExcitationPWM_Width_F;
extern Uint32 CoolerFanPWM_Width;
extern Uint32 MinimumCoolingPWMWidth;
extern Uint32 CommitDataValue;
extern Uint32 CommitCurrentsValue;

extern Uint32 EngineAMPSET;
extern Uint32 EnginePHASESET;
extern Uint32 EngineBuckPWM;
extern Uint32 EngineExcPWM;

extern Uint32 MTOC_cyclesCounter;
extern Uint32 CTOMtimeoutCounter;
extern Uint32 WorkingCycleTime_uS;
extern Uint32 Peak_WorkingCycleTime_uS;

extern Uint16 FPGA_Read_Regs[FPGA_INPUT_DATA_SIZE];
extern Uint16 FPGA_Write_Regs[FPGA_OUTPUT_DATA_SIZE];

extern Uint16 Data_FromM3[MTOC_DATA_TRANSMITT_COUNT];

extern Uint16 FPGA_Error_Latch_H;
extern Uint16 FPGA_Error_Latch_L;

extern Uint16 FPGA_C28_WD_VAL_L;
extern Uint16 FPGA_C28_WD_VAL_H;

extern Uint16 FPGA_M3_WD_VAL_L;
extern Uint16 FPGA_M3_WD_VAL_H;

extern Uint16 FPGA_Error_State_H;
extern Uint16 FPGA_Error_State_L;

extern Uint16 FPGA_EMIF_WD_Value;
extern Uint16 FPGA_EMIF_WD_Value_Prev;

extern Uint16 FPGA_Inputs_State_H;
extern Uint16 FPGA_Inputs_State_L;

extern volatile Uint16 FPGA_EXT_OUT;

extern Uint16 FPGA_OCP_Counter;

extern Uint16 BoosterInput1;
extern Uint16 BoosterInput2;
extern Uint16 BoosterInput3;
extern Uint16 BoosterInput4;
extern Uint16 BoosterInput5;
extern Uint16 BoosterInput6;
extern Uint16 BoosterInput7;
extern Uint16 BoosterInput8;
extern Uint16 BoosterInput9;
extern Uint16 BoosterInput10;
extern Uint16 BoosterInput11;
extern Uint16 BoosterInput12;

extern Uint16 FPGA_ResetValue;

extern Uint16 FPGA_EMIF_VALUES[10];
extern Uint16 EMIFrcounter;

extern Uint16 TransmittCounter;
extern Uint16 M3_TransmittCounter;

extern Uint16 TurnOnCommand;
extern Uint16 NewCommand;
extern Uint16 LastCommand;
extern Uint16 CurrentCommand;
extern Uint16 ExecutedCommand;
extern Uint16 CurrentAlg;
extern Uint32 AlgCounter;
extern Uint32 EmergencyCounter;
extern Uint16 BrakeLocker;

extern Uint16 SaveDataRequestFlag;

extern Uint32 ErrorFlags1;
extern Uint32 ErrorFlags2;
extern Uint32 ErrorFlags3;
extern Uint32 ErrorFlags4;

extern volatile Uint64 MeasurmentsTime;
extern volatile Uint16 MeasurmentsCounter;
extern volatile Uint16 AIN16_Values;
extern volatile Uint16 AIN15_Values;
extern volatile Uint16 AIN14_Values;
extern volatile Uint16 AIN13_Values;
extern volatile Uint16 AIN12_Values;
extern volatile Uint16 AIN11_Values;
extern volatile Uint16 AIN17_Values;
extern volatile Uint16 AIN18_Values;
extern volatile Uint16 AIN19_Values;
extern volatile Uint16 AIN20_Values;

extern volatile Uint16 AIN5_Values;
extern volatile Uint16 AIN6_Values;
extern volatile Uint16 AIN7_Values;
extern volatile Uint16 AIN8_Values;
extern volatile Uint16 AIN9_Values;
extern volatile Uint16 AIN10_Values;
extern volatile Uint16 AIN4_Values;
extern volatile Uint16 AIN3_Values;
extern volatile Uint16 AIN2_Values;
extern volatile Uint16 AIN1_Values;

extern volatile Uint16 IsBufferUpperHalf;
extern volatile Uint16 IsBufferLowerHalf;
extern volatile Uint16 BufferUpperHalfReady;
extern volatile Uint16 BufferLowerHalfReady;

extern volatile Uint16 ADC_Current_Value1;
extern volatile Uint16 ADC_Current_Value2;
extern volatile Uint16 ADC_Current_Value3;
extern volatile Uint16 ADC_Current_Value4;
extern volatile Uint16 ADC_Current_Value5;
extern volatile Uint16 ADC_Current_Value6;
extern volatile Uint16 ADC_Current_Value7;
extern volatile Uint16 ADC_Current_Value8;
extern volatile Uint16 ADC_Current_Value9;
extern volatile Uint16 ADC_Current_Value10;
extern volatile Uint16 ADC_Current_Value11;
extern volatile Uint16 ADC_Current_Value12;
extern volatile Uint16 ADC_Current_Value13;
extern volatile Uint16 ADC_Current_Value14;
extern volatile Uint16 ADC_Current_Value15;
extern volatile Uint16 ADC_Current_Value16;
extern volatile Uint16 ADC_Current_Value17;
extern volatile Uint16 ADC_Current_Value18;
extern volatile Uint16 ADC_Current_Value19;
extern volatile Uint16 ADC_Current_Value20;


extern volatile Uint16 LoopCount;
extern volatile Uint16 ConversionCount;

extern volatile Uint16 ChannelsTransferred;
extern volatile Uint16 *DMADest;
extern volatile Uint16 *DMASource;
extern volatile Uint16 DMADestOffset;
extern volatile Uint16* DataAddress[DMA_CHANNELS_TO_TRANSFER];
extern volatile Uint16* UpperDataAddress[DMA_CHANNELS_TO_TRANSFER];
extern volatile Uint64 TimerTicksCount; //Global ticks counter
extern volatile Uint16 TimerTicksDivideCounter;

extern volatile Uint32 CurrentDataOffset;
extern volatile Uint32 CurrentBufNum;
extern volatile Uint32 CurrentBufOffset;
extern volatile Uint16 DataPacksSent;
extern volatile Uint16 DataSentCounter;

extern volatile Uint16 CyclesCounter;


extern volatile Uint64 Time;
extern volatile Uint16 Time0, Time1, Time2, Time3;


extern float Booster_DCVoltage_OUT;
extern float Booster_DCVoltage_IN;
extern float Booster_ChopperCurrent;
extern float Booster_BrakeCurrent;
extern float Booster_ChopperSKiiPCurrent;
extern float Booster_ChopperSKiiPTemperature;
extern float Booster_BrakeSKiiPCurrent;
extern float Booster_BrakeSKiiPTemperature;
extern float Booster_ReGCurrentLimit;


extern float DCVoltage_IN;
extern float DCVoltage_OUT;
extern float PW_ZeroVRise;
extern float PW_MaxRiseLimit;
extern float PW_MaxLimit;
extern float VRegLimitMax;
extern float DCOutSetpoint;
extern float Reg_DCInpSetpoint;
extern float DCVoltageError;
extern float Alg_DCOutSetpoint;
extern float Alg_DCInpSetpoint;
//extern float ChopperCurrent1;
//extern float ChopperCurrent2;
extern float ChopperCurrentSetpoint;
extern float CurrentChoppError;
extern float DCVoltageChoppError;

extern float ExcitationCurrent;
extern float ExcitationCurrentFiltered;
extern float ExcitationSetpoint;
extern float ExcitationPWLimit;
extern float DCoutSetpoint;
extern float ActivePowerSetpoint;
extern float ActivePowerSetpointDist;
extern float RotSpeedSetpoint;
extern float ReactivePowerSetpoint;
//extern float InverterMaximumPower;

extern Uint16 SCADASystemState;
extern Uint16 SystemState;
extern Uint16 MainCycleMeasurmentsCounter;

extern Uint16 OffsetRecalculated;
extern Uint16 RecalcOffset;
extern short ChannelOffsets[20];
extern Int32 ChannelOffsetCalc[20];

extern float Current_U1;
extern float Current_V1;
extern float Current_W1;
extern float Voltage_1;
extern float Temp1_1;
extern float Temp1_2;
extern float Temp1_3;

extern float Current_U2;
extern float Current_V2;
extern float Current_W2;
extern float Voltage_2;
extern float Temp2_1;
extern float Temp2_2;
extern float Temp2_3;

extern float Current_U3;
extern float Current_V3;
extern float Current_W3;
extern float Voltage_3;
extern float Temp3_1;
extern float Temp3_2;
extern float Temp3_3;

extern float Gen_TempPh1;
extern float Gen_TempPh2;
extern float Gen_TempPh3;
extern float Gen_TempExc1;
extern float Gen_TempExc2;
extern float Gen_TempRearBearing;

extern float EXCTemp1;
extern float EXCTemp2;
extern float EXCTemp3;
extern float ChopperCurrent;
extern float ExcitationCurrent;
extern float ExcitationCurrent2;

extern float Rect1MeanCurrent;
extern float Rect2MeanCurrent;
extern float Rect3MeanCurrent;

extern float UDCMeanVoltage;
extern float IExcMeanCurrent;
extern float IExcMeanCurrentPrecise;
extern float ExcitationResistance;
extern float SKiiPC_MeanTemp;
extern float SKiiPB_MeanTemp;
extern float SKiiPA_MeanTemp;

extern float PhAIGBTCurrentRMS;
extern float PhBIGBTCurrentRMS;
extern float PhCIGBTCurrentRMS;

extern float NeutralIGBTCurrentRMS;
extern float NeutralCurrentRMS;

extern float PhACurrentRMS;
extern float PhBCurrentRMS;
extern float PhCCurrentRMS;

extern float PhAVoltRMS;
extern float PhBVoltRMS;
extern float PhCVoltRMS;
extern float PhaseAVoltage_Array[MEASURMENTS_BUFFER_SIZE];
extern float PhaseBVoltage_Array[MEASURMENTS_BUFFER_SIZE];
extern float PhaseCVoltage_Array[MEASURMENTS_BUFFER_SIZE];
//extern float IGBT_PhA_Currents[MEASURMENTS_BUFFER_SIZE];
//extern float IGBT_PhB_Currents[MEASURMENTS_BUFFER_SIZE];
//extern float IGBT_PhC_Currents[MEASURMENTS_BUFFER_SIZE];
extern float OutputFrequency;
extern float OutputPhaseSet;
extern float OutputPhase;
extern float MinOutputFrequency;
extern Uint16 IndexShiftCount;
extern float FrequencyMultiplier;
extern Uint16 ShiftedVoltageIndex;
extern float InstActivePower_A;
extern float InstActivePower_B;
extern float InstActivePower_C;
extern float InstActivePower;
extern float ActivePower;
extern float Unb_ActivePowerMax;
extern float Unb_ActivePowerMin;
extern SecOrdFilter* APLowpassFilter;
extern SecOrdFilter* APLowpassFilter2;
extern SecOrdFilter* APBandstopFilter;
extern SecOrdFilter* APBandstopFilter2;
extern float InstReactivePower_A;
extern float InstReactivePower_B;
extern float InstReactivePower_C;
extern float InstReactivePower;
extern float ReactivePower;
extern SecOrdFilter* RPLowpassFilter;
extern SecOrdFilter* RPLowpassFilter2;
extern SecOrdFilter* RPBandstopFilter;
extern SecOrdFilter* RPBandstopFilter2;


extern SecOrdFilter* GenPhaseRMSLowpassFilter;
extern float GenPhaseRMS;

extern SecOrdFilter* GenSpeedLowpassFilter;
extern SecOrdFilter* ExcCurrentFilter;
extern float GenSpeedFiltered;
extern float GenSpeedDoubleFiltered;
extern float MomentLimitedPower;
extern float TurbineLimitedPower;
extern float PowerDampingLimit;
extern float ZeroBaseSpeed;
extern float TurbinePower_K;
extern float TurbinePower_B;
extern float TurbineChar[20];
extern float CurrentBaseSpeed;
extern SecOrdFilter* IDCLowpassFilter;
extern float IDCFiltered;

extern SecOrdFilter* RegVoltageErrorFilter;
extern SecOrdFilter* RMSVoltageFilter;

extern Uint16 MasterCode;
extern Uint16 SystemManualControl;
extern Uint16 ExcitationManualControl;
extern Uint16 ChopperManualControl;
extern Uint16 InverterVoltageManualControl;
extern Uint16 InverterFrequencyManualControl;
extern Uint16 VentilationManualControl;
extern Uint16 Isolated_Grid_ControlMode;
extern Uint16 Rotation_Power_ControlMode;

extern Uint16 Isolated_Voltage_ControlMode; //OutputVoltageRegulation

extern Uint16 CurrentSysytemState;


extern float Reg_ExcSetpoint;
extern float Reg_GenSpeeed;
extern float Reg_IDC;

extern float Reg_DCVoltageSetpoint;
extern float Alg_MaxGeneratorControlledLoad;
extern float Alg_PowerDamping;
extern float Reg_VoltageBoost;
extern float Alg_BaseFrequency;
extern float Alg_Excitation_limit;

extern float Reg_APowerSetpoint;
extern float Reg_VoltageError;
extern float ReactivePowerSetpointReg;
extern float Reg_ReactivePowerBoost;
extern float Reg_ReactivePowerError;
extern float Reg_ReActivePowerPIDBoost;
extern float UnderloadBoost;
extern float OverloadBoost;
extern float Reg_ActivePowerPIDBoost;
extern float Reg_ActivePowerPIDBoost2;
extern float Reg_TotalActivePowerBoost;
extern float Reg_ActivePowerError;

extern Uint16 Reg_ReactiveDropCounterValue;
extern Uint16 Reg_ReactiveDropCounter;

extern float RotationFrequency;

extern float MeasuredRotFreq;
extern float Const_RotFreqMultiplier;
extern float Const_GenNumberOfPoles;
extern Uint16 GenVoltage_Unscaled;
extern Uint16 GenVoltageLow;
extern Uint16 GenVoltageHigh;
extern Uint32 TickCounts;
extern Uint32 TransitCounts;
extern Uint32 TransitTickCounts;
extern Uint32 SignChangeTickCounts;
extern Uint16 DetectionLevel;
extern Uint16 DetectionLevelHigh;
extern Uint16 DetectionLevelLow;
extern Uint16 RotSpeedMeasReset;
extern Uint16 RotSpeedMeasReady;
extern Uint16 TotalTransitCount;
extern Uint16 TransitLengthCounts;
extern Uint16 MeasurmentLengthCounts;

extern Uint16 ExcitationActivated;
extern Uint16 ChopperActivated;
extern Uint16 BrakeActivated;
extern Uint16 InverterActivated;
extern Uint16 BreakerActivated;

extern Uint16 CurrentLimitControlActivated;

extern FloatPID* ExcitationCurrentPID; //ПИД регулятор тока возбуждения
extern FloatBipolarPID* ChopperPID; //ПИД регулятор повышающего преобразователя
extern FloatBipolarPID* DCVoltagePID; //ПИД регулятор повышающего преобразователя

extern FloatPID* VoltagePID; //ПИД регулятор напряжения
extern FloatPID* ReactivePowerPID; //регулятор реактивной мощности

extern FloatPID* FreqToPowerPID; //регулятор активной мощности
extern FloatPID_D* FrequencyPID; //ПИД регулятор частоты
extern FloatCyclicPID* PhasePID; //ПИД регулятор частоты

extern FloatPID* RotationSpeedPID; //ПИД регулятор напряжения

extern float NTC_LUT[7];

extern Uint16 ADCPeakProt_Values_Min[20];
extern Uint16 ADCPeakProt_Values_Max[20];
extern Uint32 ADCPeakProt_ERR_Mask;
extern Uint32 ADCPeakProt_InverterPhAOCP;
extern Uint32 InverterPhAOCP_Flag;
extern Uint32 ADCPeakProt_InverterPhBOCP;
extern Uint32 InverterPhBOCP_Flag;
extern Uint32 ADCPeakProt_InverterPhCOCP;
extern Uint32 InverterPhCOCP_Flag;
extern Uint32 ADCPeakProt_ChopperOCP;
extern Uint32 ADCPeakProt_ExcitationOCP;
extern Uint32 ADCPeakProt_Flag;
extern Uint32 ADCPeakProt_ERR;
extern Uint32 ADCPeakProt_Now;
extern Uint32 ADCPeakProt_Counter;
extern Uint32 GlobalError;



#endif /* PCON_C28_H_ */
