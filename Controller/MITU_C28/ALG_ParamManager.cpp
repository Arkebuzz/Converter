/*
 * ALG_ParamManager.cpp
 *
 *  Created on: 14 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>

#define FLASH_DATA_START_ADDRESS 1
#define FLASH_OFFSET_DATA_START_ADDRESS 8193

typedef union {
  float f;
  struct {
    Uint16 Word1 : 16;
    Uint16 Word2 : 16;
  } parts;
} Float_cast;

typedef union {
  short value;
  struct {
    Uint16 Word1 : 16;
  } parts;
} Short_cast;

typedef union {
  Uint32 value;
  struct {
    Uint16 Word1 : 16;
    Uint16 Word2 : 16;
  } parts;
} Long_cast;


void Setups_Init(void)
{

	Uint16 UintSetupsCount=0;
	Uint16 FloatSetupsCount=0;
	int i;
	for (i=0;i<SETUP_UINT_DATA_COUNT;i++)	{UintSetups[i] = &Uint_DummyVariable;} //Init array with a dummy pointer
	for (i=0;i<SETUP_FLOAT_DATA_COUNT;i++)	{FloatSetups[i] = &Float_DummyVariable;} //Init array with a dummy pointer

	//UINT SETUPS---------------------------------------
	UintSetups[0] = & Const_BoosterModeDisable; UintSetupsCount++;
	UintSetups[1] = & Const_BrakeModeDisable; UintSetupsCount++;
	UintSetups[2] = & Const_SyncLaunchLocked; UintSetupsCount++;
	UintSetups[3] = & Const_SyncShortLaunch; UintSetupsCount++;
	UintSetups[4] = & Const_SyncCyclesToRun; UintSetupsCount++;
	UintSetups[5] = & Const_DCPRMinCycles; UintSetupsCount++;
	UintSetups[6] = & Const_DCPRMaxCycles; UintSetupsCount++;
	UintSetups[7] = & Const_DCPRReleaseCycles; UintSetupsCount++;
	UintSetups[8] = & Const_RectPRMinCycles; UintSetupsCount++;
	UintSetups[9] = & Const_RectPRMaxCycles; UintSetupsCount++;
	UintSetups[10] = & Const_RectPRReleaseCycles; UintSetupsCount++;
	UintSetups[11] = & Const_VoutPRMinCycles; UintSetupsCount++;
	UintSetups[12] = & Const_VoutPRMaxCycles; UintSetupsCount++;
	UintSetups[13] = & Const_VoutPRReleaseCycles; UintSetupsCount++;
	UintSetups[14] = & Const_IGBTUnbPRMinCycles; UintSetupsCount++;
	UintSetups[15] = & Const_IGBTUnbPRMaxCycles; UintSetupsCount++;
	UintSetups[16] = & Const_IGBTUnbPRReleaseCycles; UintSetupsCount++;
	UintSetups[17] = & Const_IGBTOCPPRMinCycles; UintSetupsCount++;
	UintSetups[18] = & Const_IGBTOCPPRMaxCycles; UintSetupsCount++;
	UintSetups[19] = & Const_IGBTOCPPRReleaseCycles; UintSetupsCount++;
	UintSetups[20] = & Const_APHighPRMinCycles; UintSetupsCount++;
	UintSetups[21] = & Const_APHighPRMaxCycles; UintSetupsCount++;
	UintSetups[22] = & Const_APHighPRReleaseCycles; UintSetupsCount++;
	UintSetups[23] = & Const_APLowPRMinCycles; UintSetupsCount++;
	UintSetups[24] = & Const_APLowPRMaxCycles; UintSetupsCount++;
	UintSetups[25] = & Const_APLowPRReleaseCycles; UintSetupsCount++;
	UintSetups[26] = & Const_RPHighPRMinCycles; UintSetupsCount++;
	UintSetups[27] = & Const_RPHighPRMaxCycles; UintSetupsCount++;
	UintSetups[28] = & Const_RPHighPRReleaseCycles; UintSetupsCount++;
	UintSetups[29] = & Const_RPLowPRMinCycles; UintSetupsCount++;
	UintSetups[30] = & Const_RPLowPRMaxCycles; UintSetupsCount++;
	UintSetups[31] = & Const_RPLowPRReleaseCycles; UintSetupsCount++;
	UintSetups[32] = & Const_FreqPRMinCycles; UintSetupsCount++;
	UintSetups[33] = & Const_FreqPRMaxCycles; UintSetupsCount++;
	UintSetups[34] = & Const_FreqPRReleaseCycles; UintSetupsCount++;

	//---------------------------------------------------

	//FLOAT SETUPS---------------------------------------
	FloatSetups[0] = & Const_ExcitationMax; FloatSetupsCount++;
	FloatSetups[1] = & EXCPID_P; FloatSetupsCount++;
	FloatSetups[2] = & EXCPID_I; FloatSetupsCount++;
	FloatSetups[3] = & DCVPID_P; FloatSetupsCount++;
	FloatSetups[4] = & DCVPID_I; FloatSetupsCount++;
	FloatSetups[5] = & DCVPID_POSG; FloatSetupsCount++;
	FloatSetups[6] = & DCVPID_NEGG; FloatSetupsCount++;
	FloatSetups[7] = & Const_MaxVoltageBoost; FloatSetupsCount++;

	FloatSetups[8] = & RPPID_P; FloatSetupsCount++;
	FloatSetups[9] = & RPPID_I; FloatSetupsCount++;
	FloatSetups[10] = & RPPID_G; FloatSetupsCount++;
	FloatSetups[11] = & Const_ReactivePowerBoost; FloatSetupsCount++;
	FloatSetups[12] = & Const_VoltageErrorUpperLimit; FloatSetupsCount++;
	FloatSetups[13] = & Const_VoltageErrorLowerLimit; FloatSetupsCount++;
	FloatSetups[14] = & Const_ReactivePowerMax; FloatSetupsCount++;
	FloatSetups[15] = & Const_ReactivePowerMin; FloatSetupsCount++;
	FloatSetups[16] = & Const_ReactivePowerLimiter; FloatSetupsCount++;

	FloatSetups[17] = & VPID_P; FloatSetupsCount++;
	FloatSetups[18] = & VPID_I; FloatSetupsCount++;
	FloatSetups[19] = & VPID_G; FloatSetupsCount++;


	FloatSetups[20] = & APPID_P; FloatSetupsCount++;
	FloatSetups[21] = & APPID_I; FloatSetupsCount++;
	FloatSetups[22] = & Const_FreqUpperLim; FloatSetupsCount++;
	FloatSetups[23] = & Const_FreqLowerLim; FloatSetupsCount++;
	FloatSetups[24] = & Const_BaseFrequency; FloatSetupsCount++;
	FloatSetups[25] = & Const_FrequencyWindow; FloatSetupsCount++;

	FloatSetups[26] = & FRPID_P; FloatSetupsCount++;
	FloatSetups[27] = & FRPID_I; FloatSetupsCount++;
	FloatSetups[28] = & FRPID_D; FloatSetupsCount++;
	FloatSetups[29] = & FRPID_G; FloatSetupsCount++;


	FloatSetups[30] = & PHPID_P; FloatSetupsCount++;
	FloatSetups[31] = & PHPID_I; FloatSetupsCount++;
	FloatSetups[32] = & PHPID_G; FloatSetupsCount++;

	FloatSetups[33] = & Const_OutputVoltageSetpoint; FloatSetupsCount++;
	FloatSetups[34] = & Const_MaxOutputPower; FloatSetupsCount++;
	FloatSetups[35] = & Const_MinOutputPower; FloatSetupsCount++;
	FloatSetups[36] = & Const_GeneratorToLoadMaxPower; FloatSetupsCount++;
	FloatSetups[37] = & Const_ResitiveLoadMaxPower; FloatSetupsCount++;
	FloatSetups[38] = & Const_SpeedLimitMaxPower; FloatSetupsCount++;
	FloatSetups[39] = & Const_SpeedLimitFreerun; FloatSetupsCount++;
	FloatSetups[40] = & Const_TurbineLimitLine_K; FloatSetupsCount++;
	FloatSetups[41] = & Const_TurbineLimitLine_B; FloatSetupsCount++;

	FloatSetups[42] = & Const_SyncMaxPhaseChange; FloatSetupsCount++;
	FloatSetups[43] = & Const_SyncMaxFreqChange; FloatSetupsCount++;
	FloatSetups[44] = & Const_SyncMinVoltageMag; FloatSetupsCount++;
	FloatSetups[45] = & Const_SyncStarPhaseLead; FloatSetupsCount++;

	FloatSetups[46] = & Prot_ExcCurrentMax; FloatSetupsCount++;
	FloatSetups[47] = & Prot_DCVoltageMax; FloatSetupsCount++;
	FloatSetups[48] = & Prot_IGBTTempMax; FloatSetupsCount++;
	FloatSetups[49] = & Prot_IGBTTempUnbalance; FloatSetupsCount++;
	FloatSetups[50] = & Prot_GenVoltageMax; FloatSetupsCount++;
	FloatSetups[51] = & Prot_GenSpeedMax; FloatSetupsCount++;
	FloatSetups[52] = & Prot_RectCurrUnbalance; FloatSetupsCount++;
	FloatSetups[53] = & Prot_OutpVoltageUnbalance; FloatSetupsCount++;
	FloatSetups[54] = & Prot_OutpCurrentUnbalance; FloatSetupsCount++;
	FloatSetups[55] = & Prot_IGBTCurrentUnbalance; FloatSetupsCount++;
	FloatSetups[56] = & Prot_OutpOverVoltage; FloatSetupsCount++;
	FloatSetups[57] = & Prot_OutpOverCurrent; FloatSetupsCount++;
	FloatSetups[58] = & Prot_APOverload; FloatSetupsCount++;
	FloatSetups[59] = & Prot_APNegative; FloatSetupsCount++;
	FloatSetups[60] = & Prot_RPOverload; FloatSetupsCount++;
	FloatSetups[61] = & Prot_DCVoltageLow; FloatSetupsCount++;
	FloatSetups[62] = & Prot_GenSpeedOPLow; FloatSetupsCount++;
	FloatSetups[63] = & Prot_GenSpeedOPHigh; FloatSetupsCount++;
	FloatSetups[64] = & Prot_OutpFrequencyHigh; FloatSetupsCount++;
	FloatSetups[65] = & Prot_OutpFrequencyLow; FloatSetupsCount++;
	FloatSetups[66] = & ProtRMS_RectCurrent; FloatSetupsCount++;
	FloatSetups[67] = & ProtRMS_OutpVoltage; FloatSetupsCount++;
	FloatSetups[68] = & ProtRMS_UDCVoltage; FloatSetupsCount++;
	FloatSetups[69] = & ProtRMS_NeutralCurrent; FloatSetupsCount++;
	FloatSetups[70] = & ProtRMS_NeutralIGBCurrent; FloatSetupsCount++;
	FloatSetups[71] = & ProtRMS_ExcCurrent; FloatSetupsCount++;
	FloatSetups[72] = & ProtRMS_IGBTTemp; FloatSetupsCount++;
	FloatSetups[73] = & ProtRMS_IGBTCurrent; FloatSetupsCount++;
	FloatSetups[74] = & ProtRMS_OutpCurrent; FloatSetupsCount++;

	FloatSetups[75] = & Const_ExcMaxVoltage; FloatSetupsCount++;
	FloatSetups[76] = & Const_ExcCurrentBoost; FloatSetupsCount++;
	FloatSetups[77] = & Const_ExcPIDBoost; FloatSetupsCount++;

	FloatSetups[78] = & Const_TPR_DCLower; FloatSetupsCount++;
	FloatSetups[79] = & Const_TPR_DCUpper; FloatSetupsCount++;
	FloatSetups[80] = & Const_TPR_RectCurrLower; FloatSetupsCount++;
	FloatSetups[81] = & Const_TPR_RectCurrUpper; FloatSetupsCount++;
	FloatSetups[82] = & Const_TPR_VoutLower; FloatSetupsCount++;
	FloatSetups[83] = & Const_TPR_VoutUpper; FloatSetupsCount++;
	FloatSetups[84] = & Const_TPR_IGBTUnbLower; FloatSetupsCount++;
	FloatSetups[85] = & Const_TPR_IGBTUnbUpper; FloatSetupsCount++;
	FloatSetups[86] = & Const_TPR_IGBTOCPLower; FloatSetupsCount++;
	FloatSetups[87] = & Const_TPR_IGBTOCPUpper; FloatSetupsCount++;
	FloatSetups[88] = & Const_TPR_APHighLower; FloatSetupsCount++;
	FloatSetups[89] = & Const_TPR_APHighUpper; FloatSetupsCount++;
	FloatSetups[90] = & Const_TPR_APLowLower; FloatSetupsCount++;
	FloatSetups[91] = & Const_TPR_APLowUpper; FloatSetupsCount++;
	FloatSetups[92] = & Const_TPR_RPHighLower; FloatSetupsCount++;
	FloatSetups[93] = & Const_TPR_RPHighUpper; FloatSetupsCount++;
	FloatSetups[94] = & Const_TPR_RPLowLower; FloatSetupsCount++;
	FloatSetups[95] = & Const_TPR_RPLowUpper; FloatSetupsCount++;
	FloatSetups[96] = & Const_TPR_FreqLower; FloatSetupsCount++;
	FloatSetups[97] = & Const_TPR_FreqUpper; FloatSetupsCount++;

	FloatSetups[98] = & Const_UnderloadBoostPerVolt; FloatSetupsCount++;
	FloatSetups[99] = & Const_OverloadBoostPerVolt; FloatSetupsCount++;

	//---------------------------------------------------
	Const_CurrentSetupVersion1 = UintSetupsCount;
	Const_CurrentSetupVersion2 = FloatSetupsCount;
	Const_CurrentSetupVersion3 = SETUP_UINT_DATA_COUNT;
	Const_CurrentSetupVersion4 = SETUP_FLOAT_DATA_COUNT;

	for (i=0;i<SETUP_UINT_DATA_COUNT;i++)
	{WriteTo_CTOM_MSGRAM(SETUP_DATA_START_ADDR+i,Setups_ReadUintParam(i));}

	for (i=0;i<SETUP_FLOAT_DATA_COUNT;i++)
	{
		float DataRead = Setups_ReadFloatParam(i);
		WriteTo_CTOM_MSGRAM_Float(SETUP_DATA_START_ADDR+SETUP_UINT_DATA_COUNT + i*2, DataRead);
	}


}


void OsciChannelsInit(void)
{
	Uint16 i;

	for (i=0;i<OSCI_SOURCES_COUNT;i++)	{OsciSources[i] = &Float_DummyVariable; OsciSourcesGain[i]=0; OsciSourcesOffset[i]=0;} //Init array with a dummy pointer
	for (i=0;i<10;i++) {SOC[i]=i;}

//Init service osci data-------------------------------------------------------------------------------------------------
	OsciSources[0] = & ActivePower; 					OsciSourcesGain[0]=0.001; 		OsciSourcesOffset[0]=0;
	OsciSources[1] = & ReactivePower; 					OsciSourcesGain[1]=0.001; 		OsciSourcesOffset[1]=0;
	OsciSources[2] = & OutputFrequency; 				OsciSourcesGain[2]=10000.0; 	OsciSourcesOffset[2]=-50.0;
	OsciSources[3] = & Booster_DCVoltage_IN;			OsciSourcesGain[3]=1; 			OsciSourcesOffset[3]=0;
	OsciSources[4] = & PIDActivePowerSetpoint; 			OsciSourcesGain[4]=0.001; 		OsciSourcesOffset[4]=0;
	OsciSources[5] = & MainPWM_AMP_F; 					OsciSourcesGain[5]=1; 			OsciSourcesOffset[5]=0;
	OsciSources[6] = & Booster_ChopperCurrent;			OsciSourcesGain[6]=1; 		    OsciSourcesOffset[6]=0;
	OsciSources[7] = & DCVoltagePID->CurrentOutput; 	OsciSourcesGain[7]=1000; 		OsciSourcesOffset[7]=0;
	OsciSources[8] = & ExcitationCurrentPID->CurrentOutput; OsciSourcesGain[8]=1; 		OsciSourcesOffset[8]=0;
	OsciSources[9] = & GenSpeedFiltered; 				OsciSourcesGain[9]=1; 			OsciSourcesOffset[9]=0;

	OsciSources[10] = & Alg_PowerDamping; 				OsciSourcesGain[10]=0.001; 		OsciSourcesOffset[10]=0;
	OsciSources[11] = & Reg_VoltageBoost; 				OsciSourcesGain[11]=1; 			OsciSourcesOffset[11]=0;
	OsciSources[12] = & DCOutSetpoint; 					OsciSourcesGain[12]=1; 			OsciSourcesOffset[12]=0;
	OsciSources[13] = & DCVoltageError; 				OsciSourcesGain[13]=1; 			OsciSourcesOffset[13]=0;
	OsciSources[14] = & ExcitationCurrentFiltered;	 	OsciSourcesGain[14]=1000;		OsciSourcesOffset[14]=0;
	OsciSources[15] = & ExcitationPWM_Width_F;			OsciSourcesGain[15]=1; 			OsciSourcesOffset[15]=0;
	OsciSources[16] = & Reg_APowerSetpoint; 			OsciSourcesGain[16]=0.001; 		OsciSourcesOffset[16]=0;
	OsciSources[17] = & NeutralIGBTCurrentRMS; 			OsciSourcesGain[17]=1; 			OsciSourcesOffset[17]=0;

	OsciSources[18] = & FrequencyPID->CurrentOutput; 	OsciSourcesGain[18]=10000.0; 	OsciSourcesOffset[18]=-60.0;
	OsciSources[19] = & Reg_APowerSetpoint; 			OsciSourcesGain[19]=0.001;	 	OsciSourcesOffset[19]=0;
	OsciSources[20] = & Reg_ActivePowerPIDBoost; 		OsciSourcesGain[20]=1; 			OsciSourcesOffset[20]=0;
	OsciSources[21] = & VoltagePID->CurrentOutput; 		OsciSourcesGain[21]=1; 			OsciSourcesOffset[21]=0;
	OsciSources[22] = & Reg_VoltageError; 				OsciSourcesGain[22]=1; 			OsciSourcesOffset[22]=0;
	OsciSources[23] = & Alg_DCOutSetpoint; 				OsciSourcesGain[23]=1; 			OsciSourcesOffset[23]=0;
	OsciSources[24] = & ReactivePowerPID->CurrentOutput;OsciSourcesGain[24]=0.001; 		OsciSourcesOffset[24]=0;
	OsciSources[25] = & ReactivePowerSetpointReg; 		OsciSourcesGain[25]=0.001; 		OsciSourcesOffset[25]=0;
	OsciSources[26] = & Reg_ReactivePowerBoost; 		OsciSourcesGain[26]=0.001;		OsciSourcesOffset[26]=0;

	OsciSources[27] = & FVAL_InputFreq; 				OsciSourcesGain[27]=10000.0; 	OsciSourcesOffset[27]=-50.0;
	OsciSources[28] = & FVAL_MeasuredOutputFrequency; 	OsciSourcesGain[28]=10000.0; 	OsciSourcesOffset[28]=-50.0;
	OsciSources[29] = & MeasuredOutputFrequency; 		OsciSourcesGain[29]=10000.0; 	OsciSourcesOffset[29]=-50.0;
	OsciSources[30] = & FVAL_VoltagePhase; 				OsciSourcesGain[30]=50; 		OsciSourcesOffset[30]=0;
	OsciSources[31] = & FVAL_VoltageMagnitude; 			OsciSourcesGain[31]=1; 			OsciSourcesOffset[31]=0;

	OsciSources[32] = & PhaseChangeAmp; 				OsciSourcesGain[32]=50; 		OsciSourcesOffset[32]=0;
	OsciSources[33] = & FreqChangeAmp; 					OsciSourcesGain[33]=10000.0; 	OsciSourcesOffset[33]=0;
	OsciSources[34] = & OutputPhaseSet; 				OsciSourcesGain[34]=50; 		OsciSourcesOffset[34]=0;
	OsciSources[35] = & StartingVoltageMagnitude; 		OsciSourcesGain[35]=1; 			OsciSourcesOffset[35]=0;
	OsciSources[36] = & StartingPWMAmp; 				OsciSourcesGain[36]=10000.0; 	OsciSourcesOffset[36]=0;

	OsciSources[37] = & ZeroBaseSpeed;					OsciSourcesGain[37]=1; 			OsciSourcesOffset[37]=0;
	OsciSources[38] = & MomentLimitedPower;				OsciSourcesGain[38]=0.001;	 	OsciSourcesOffset[38]=0;
	OsciSources[39] = & TurbineLimitedPower;			OsciSourcesGain[39]=0.001; 		OsciSourcesOffset[39]=0;

	OsciSources[40] = & PR_IGBTAOverCurr.ProtectionActive_f;	OsciSourcesGain[40]=1; 	    OsciSourcesOffset[40]=0;

	OsciSources[41] = & PhAIGBTCurrentRMS;				OsciSourcesGain[41]=1; 		OsciSourcesOffset[41]=0;
	OsciSources[42] = & DCVoltage_OUT;					OsciSourcesGain[42]=1; 		OsciSourcesOffset[42]=0;

	OsciSources[43] = & PhasePID->CurrentOutput; 				OsciSourcesGain[43]=50; 			OsciSourcesOffset[43]=0;
	OsciSources[44] = & FreqToPowerPID->CurrentOutput; 			OsciSourcesGain[44]=0.001; 		OsciSourcesOffset[44]=0;

	OsciSources[45] = & Booster_DCVoltage_OUT;					OsciSourcesGain[45]=1; 		OsciSourcesOffset[45]=0;
	OsciSources[46] = & Booster_BrakeCurrent;					OsciSourcesGain[46]=1; 		OsciSourcesOffset[46]=0;
	OsciSources[47] = & Booster_ChopperSKiiPCurrent;			OsciSourcesGain[47]=1; 		OsciSourcesOffset[47]=0;
	OsciSources[48] = & Booster_ChopperSKiiPTemperature;		OsciSourcesGain[48]=1; 		OsciSourcesOffset[48]=0;
	OsciSources[49] = & Booster_BrakeSKiiPCurrent;				OsciSourcesGain[49]=1; 		OsciSourcesOffset[49]=0;
	OsciSources[50] = & Booster_BrakeSKiiPTemperature;			OsciSourcesGain[50]=1; 		OsciSourcesOffset[50]=0;
	OsciSources[51] = & Booster_ReGCurrentLimit;				OsciSourcesGain[51]=1; 		OsciSourcesOffset[51]=0;



//-----------------------------------------------------------------------------------------------------------------------
}






Uint16 ReadDataFromFlash(void)
{
	int i;
	Uint32 read_pointer;
	Uint16 ReadSuccess=1;
	Uint16 LED=0;
	Uint64 Checksumm = 0;
	Float_cast FloatUnion;
	for (i=0;i<10000;i++) {i=i+1;}


	//Read flash header
	read_pointer=FLASH_DATA_START_ADDRESS;
	Uint16 DummyRead = SyncReadWordFromFlash(read_pointer);read_pointer++; //UintSetupsCount
	for (i=0;i<10000;i++) {i=i+1;} //Wait for flash to init

	Uint16 FlashVersion1 = SyncReadWordFromFlash(read_pointer);read_pointer++; //UintSetupsCount
	Uint16 FlashVersion2 = SyncReadWordFromFlash(read_pointer);read_pointer++; //FloatSetupsCount
	Uint16 FlashVersion3 = SyncReadWordFromFlash(read_pointer);read_pointer++; //SETUP_UINT_DATA_COUNT
	Uint16 FlashVersion4 = SyncReadWordFromFlash(read_pointer);read_pointer++; //SETUP_FLOAT_DATA_COUNT

	GpioG1DataRegs.GPADAT.bit.GPIO4 = 0; // LED Blink // Turn off LED

	if (FlashVersion1!=Const_CurrentSetupVersion1) {ReadSuccess=0;}
	else if (FlashVersion2!=Const_CurrentSetupVersion2) { ReadSuccess=0;}
	else if (FlashVersion3!=Const_CurrentSetupVersion3) { ReadSuccess=0;}
	else if (FlashVersion4!=Const_CurrentSetupVersion4) { ReadSuccess=0;}
	else
	{
		//Read and verify Uint data
		for (i=0;i<SETUP_UINT_DATA_COUNT;i++)
		{
			Uint16 Value1 = SyncReadWordFromFlash(read_pointer); read_pointer++;
			Uint16 Value2 = SyncReadWordFromFlash(read_pointer); read_pointer++;
			Checksumm = Checksumm + Value1;
			if (Value1==Value2) {Setups_WriteUintParam(i,Value1);}
			else {ReadSuccess=0;}
		}

		//Read and verify float data
		for (i=0;i<SETUP_FLOAT_DATA_COUNT;i++)
		{
			if (LED==0) {GpioG1DataRegs.GPADAT.bit.GPIO4 = 1; LED=1;}
			else {GpioG1DataRegs.GPADAT.bit.GPIO4 = 0; LED=0;}
			Uint16 FValue1 = SyncReadWordFromFlash(read_pointer);read_pointer++;
			Uint16 FValue2 = SyncReadWordFromFlash(read_pointer);read_pointer++;
			Uint16 FValue3 = SyncReadWordFromFlash(read_pointer);read_pointer++;
			Uint16 FValue4 = SyncReadWordFromFlash(read_pointer);read_pointer++;
			Checksumm = Checksumm + FValue1 + FValue2;
			if (FValue1==FValue3 && FValue2==FValue4)
				{
				FloatUnion.parts.Word1 = FValue1;
				FloatUnion.parts.Word2 = FValue2;
				float ValueToWrite = FloatUnion.f;
				Setups_WriteFloatParam(i,ValueToWrite);
				}
			else {ReadSuccess=0;}
		}

		//Read and verify checksum
		Uint16 Checksum4read = SyncReadWordFromFlash(read_pointer);read_pointer++;
		Uint16 Checksum3read = SyncReadWordFromFlash(read_pointer);read_pointer++;
		Uint16 Checksum2read = SyncReadWordFromFlash(read_pointer);read_pointer++;
		Uint16 Checksum1read = SyncReadWordFromFlash(read_pointer);read_pointer++;

		Uint16 Checksum1 = (Uint16)((Checksumm&0xFFFF000000000000>>48)&0x000000000000FFFF);
		Uint16 Checksum2 = (Uint16)((Checksumm&0x0000FFFF00000000>>32)&0x000000000000FFFF);
		Uint16 Checksum3 = (Uint16)((Checksumm&0x00000000FFFF0000>>16)&0x000000000000FFFF);
		Uint16 Checksum4 = (Uint16)(Checksumm&0x000000000000FFFF&0x000000000000FFFF);

		if (Checksum1!=Checksum1read || Checksum2!=Checksum2read || Checksum3!=Checksum3read || Checksum4read!=Checksum4)
		{ ReadSuccess=0;}

	}

	reader_exit:
	GpioG1DataRegs.GPADAT.bit.GPIO4 = 0; // LED Blink // Turn off LED
	return (ReadSuccess);
}

void SaveDataToFlash(void)
{
	/*int i;
	Uint32 write_pointer;
	Uint16 LED=0;
	Uint64 Checksumm = 0;
	Float_cast FloatUnion;

	for (i=0;i<1000;i++) {i=i+1;}
	FlashReader->SetRewriteFlash();

	write_pointer = FLASH_DATA_START_ADDRESS;
	SyncWriteWordToFlash(write_pointer, 1); write_pointer++;
	for (i=0;i<1000;i++) {i=i+1;} //Wait for flash to init

	SyncWriteWordToFlash(write_pointer, Const_CurrentSetupVersion1); write_pointer++;
	SyncWriteWordToFlash(write_pointer, Const_CurrentSetupVersion2); write_pointer++;
	SyncWriteWordToFlash(write_pointer, Const_CurrentSetupVersion3); write_pointer++;
	SyncWriteWordToFlash(write_pointer, Const_CurrentSetupVersion4); write_pointer++;

	for (i=0;i<SETUP_UINT_DATA_COUNT;i++)
	{
		if (LED==0) {GpioG1DataRegs.GPADAT.bit.GPIO4 = 1; LED=1;}
		else {GpioG1DataRegs.GPADAT.bit.GPIO4 = 0; LED=0;}
		Uint16 WordToWrite = Setups_ReadUintParam(i);
		SyncWriteWordToFlash(write_pointer, WordToWrite); write_pointer++;
		SyncWriteWordToFlash(write_pointer, WordToWrite); write_pointer++;
		Checksumm = Checksumm + WordToWrite;
	}

	for (i=0;i<SETUP_FLOAT_DATA_COUNT;i++)
	{
		if (LED==0) {GpioG1DataRegs.GPADAT.bit.GPIO4 = 1; LED=1;}
		else {GpioG1DataRegs.GPADAT.bit.GPIO4 = 0; LED=0;}
		FloatUnion.f = Setups_ReadFloatParam(i);
		Uint16 DataHighWord = FloatUnion.parts.Word1;
		Uint16 DataLowWord = FloatUnion.parts.Word2;
		SyncWriteWordToFlash(write_pointer, DataHighWord); write_pointer++;
		SyncWriteWordToFlash(write_pointer, DataLowWord); write_pointer++;
		SyncWriteWordToFlash(write_pointer, DataHighWord); write_pointer++;
		SyncWriteWordToFlash(write_pointer, DataLowWord); write_pointer++;
		Checksumm = Checksumm + DataHighWord + DataLowWord;
	}

	Uint16 Checksum1 = (Uint16)((Checksumm&0xFFFF000000000000>>48)&0x000000000000FFFF);
	Uint16 Checksum2 = (Uint16)((Checksumm&0x0000FFFF00000000>>32)&0x000000000000FFFF);
	Uint16 Checksum3 = (Uint16)((Checksumm&0x00000000FFFF0000>>16)&0x000000000000FFFF);
	Uint16 Checksum4 = (Uint16)(Checksumm&0x000000000000FFFF&0x000000000000FFFF);

	SyncWriteWordToFlash(write_pointer, Checksum4); write_pointer++;
	SyncWriteWordToFlash(write_pointer, Checksum3); write_pointer++;
	SyncWriteWordToFlash(write_pointer, Checksum2); write_pointer++;
	SyncWriteWordToFlash(write_pointer, Checksum1); write_pointer++;
    */

}


void SaveOffsetsToFlash(void)
{
	int i;
	Uint32 write_pointer;
	//Uint16 LED=0;
	//Uint64 Checksumm = 0;
	Short_cast ShortUnion;
	//Long_cast Uint32Union;
	Uint32 Checksum=0;

	GpioG1DataRegs.GPADAT.bit.GPIO4 = 1;
	write_pointer = FLASH_OFFSET_DATA_START_ADDRESS;
	SyncWriteWordToFlash(write_pointer, 1); write_pointer++; //Dummy word to make sure block is cleared
	for (i=0;i<1000;i++) {i=i+1;} //Wait for flash to init and clear block
	for (i=0;i<20;i++)
	{
		ShortUnion.value = ChannelOffsets[i];
		Uint16 WordToWrite = ShortUnion.parts.Word1;
		SyncWriteWordToFlash(write_pointer, WordToWrite); write_pointer++;
		SyncWriteWordToFlash(write_pointer, WordToWrite); write_pointer++;
		Checksum = Checksum + WordToWrite;
	}
	//Uint32Union.value = Checksum;
	//Uint16 WordToWrite = Uint32Union.parts.Word1;
	//SyncWriteWordToFlash(write_pointer, WordToWrite); write_pointer++;
	//WordToWrite = Uint32Union.parts.Word2;
	//SyncWriteWordToFlash(write_pointer, WordToWrite); write_pointer++;
	//GpioG1DataRegs.GPADAT.bit.GPIO4 = 0;
}

Uint16 ReadOffsetsFromFlash(void)
{
	int i;
	Uint32 read_pointer;
	Uint16 ReadSuccess=1;
	Short_cast ShortUnion;

	GpioG1DataRegs.GPADAT.bit.GPIO4 = 1;

	read_pointer=FLASH_OFFSET_DATA_START_ADDRESS;
	Uint16 DummyRead = SyncReadWordFromFlash(read_pointer);read_pointer++; //Dummy word to make sure operations start
	for (i=0;i<1000;i++) {i=i+1;} //Wait for flash to init
	for (i=0;i<20;i++)
		{
		Uint16 ReadValue1 = SyncReadWordFromFlash(read_pointer);read_pointer++;
		Uint16 ReadValue2 = SyncReadWordFromFlash(read_pointer);read_pointer++;
		if (ReadValue1==ReadValue2)
			{
			ShortUnion.parts.Word1 = ReadValue1;
				if (ShortUnion.value>=-50 && ShortUnion.value<=50)
				{
				ChannelOffsets[i] = ShortUnion.value;
				}
				else {ReadSuccess=0;}
			}
		else {ReadSuccess=0;}
		}

	GpioG1DataRegs.GPADAT.bit.GPIO4 = 0;
	return(ReadSuccess);
}


void EraseFlashData(void)
{
	/*int i;
	Uint32 write_pointer;
	for (i=0;i<1000;i++) {i=i+1;}
	FlashReader->SetRewriteFlash();
	write_pointer = FLASH_DATA_START_ADDRESS;
	SyncWriteWordToFlash(write_pointer, 0); write_pointer++;
	SyncWriteWordToFlash(write_pointer, 0); write_pointer++;
	SyncWriteWordToFlash(write_pointer, 0); write_pointer++;
	SyncWriteWordToFlash(write_pointer, 0); write_pointer++;
	SyncWriteWordToFlash(write_pointer, 0); write_pointer++;*/

}


Uint16 SyncReadWordFromFlash(Uint32 Addr)
{
	/*Uint16 i =0;
	for (i=0;i<1000;i++) {i=i+1;}
	FlashReader->SPIFlashReaderInit(MAIN_CYCLE_US);
	FlashReader->InitReadWordFromFlash(Addr);
	while (FlashReader->SPIFlashStep()!=1) {while (CpuTimer1Regs.TCR.bit.TIF==0) {}	CpuTimer1Regs.TCR.bit.TIF = 1;}
	return (FlashReader->GetReadValue());*/
}

void SyncWriteWordToFlash(Uint32 Addr, Uint16 Data)
{
	/*Uint16 i = 0;
	for (i=0;i<1000;i++) {i=i+1;}
	FlashReader->SPIFlashReaderInit(MAIN_CYCLE_US);
	FlashReader->InitWriteWordToFlash(Addr,Data);
	while (FlashReader->SPIFlashStep()!=1) {while (CpuTimer1Regs.TCR.bit.TIF==0) {}	CpuTimer1Regs.TCR.bit.TIF = 1;}*/
}

void Setups_WriteUintParam(Uint16 ID, Uint16 Value)
{
	if (ID<SETUP_UINT_DATA_COUNT)
		{
		*(UintSetups[ID])=Value;
		WriteTo_CTOM_MSGRAM(SETUP_DATA_START_ADDR+ID,Value);
		}
}


Uint16 Setups_ReadUintParam(Uint16 ID)
{
	Uint16 ReturnVal=0xFFFF;
	if (ID<SETUP_UINT_DATA_COUNT)
		{
		ReturnVal = *(UintSetups[ID]);
		}
	return (ReturnVal);
}

void Setups_WriteFloatParam(Uint16 ID, float Value)
{
	if (ID<SETUP_FLOAT_DATA_COUNT)
		{
		*(FloatSetups[ID])=Value;
		WriteTo_CTOM_MSGRAM_Float(SETUP_DATA_START_ADDR+SETUP_UINT_DATA_COUNT+ID*2,Value);
		}
}


float Setups_ReadFloatParam(Uint16 ID)
{
	float ReturnVal = 0;
	if (ID<SETUP_FLOAT_DATA_COUNT)
		{
		ReturnVal = *(FloatSetups[ID]);
		}
	return (ReturnVal);
}
