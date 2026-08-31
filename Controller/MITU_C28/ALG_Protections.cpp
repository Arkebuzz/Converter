/*
 * ALG_Protections.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>

void ErrorSet(Uint32 ErrNum)
	{


	if (GlobalError==0) {GlobalError = ErrNum;}
	GpioG1DataRegs.GPACLEAR.bit.GPIO7 = 1; //Remove system OK flag to FPGA
	GpioG1DataRegs.GPACLEAR.bit.GPIO11 = 1; //LED OK off
	ActivateInverter(0); //Remove ActivateInverter flag to FPGA
	ActivateChopper(0); //Remove ActivateChopper flag to FPGA
	if (ErrNum==0 || ErrNum==3 || ErrNum==4 || ErrNum==5 || ErrNum==11 || ErrNum==12 || ErrNum==18)
	{
	ActivateExcitation(0);
	BrakeLocker = 1;
	}
	CurrentAlg = 0;
	if (ErrNum==96)	{ErrorFlags4 = ADCPeakProt_ERR;}
	else if (ErrNum==64) {ErrorFlags3 |= (((Uint32)FPGA_Error_Latch_H)<<16) | (Uint32)FPGA_Error_Latch_L;}
	else if (ErrNum<64 && ErrNum>=32) {ErrorFlags2 |= ((Uint32)0x1)<<(ErrNum-32);}
	else if (ErrNum<32)  {ErrorFlags1 |= ((Uint32)0x1)<<(ErrNum);}
	else {ErrorFlags1|=0x1;}



	}

void ErrorReset(void)
	{
	ActivateInverter(0);
	ActivateChopper(0);
	ActivateExcitation(0);
	ActivateBrake(0);
	BrakeLocker = 0;
	EmergencyCounter = 0;
	ErrorRemove();
	ErrorFlags1=0; ErrorFlags2=0; ErrorFlags3=0; ErrorFlags4=0;
	GlobalError=0;
	CommitDataValue=43690;
	WriteWordTo_FPGA(50,2014);
	WriteWordTo_FPGA(100,(unsigned short)(CommitDataValue));
	SetTryToResetError =1;
	PR_DCOvervoltage.TimedProtectionReset();
	PR_RectUnbalance.TimedProtectionReset();
	PR_IGBTCurrUnbalance.TimedProtectionReset();
	PR_IGBTAOverCurr.TimedProtectionReset();
	PR_IGBTBOverCurr.TimedProtectionReset();
	PR_IGBTCOverCurr.TimedProtectionReset();
	PR_OutpVoltageUnbalance.TimedProtectionReset();
	PR_NegativeAPower.TimedProtectionReset();
	PR_NegativeRPower.TimedProtectionReset();
	PR_APOverload.TimedProtectionReset();
	PR_RPOverload.TimedProtectionReset();
	PR_OutputFrequency.TimedProtectionReset();
	PR_ActivePowerRegulation.TimedProtectionReset();

	PR_OutpPhAOverCurr.TimedProtectionReset();
	PR_OutpPhBOverCurr.TimedProtectionReset();
	PR_OutpPhCOverCurr.TimedProtectionReset();
	}

void FastProtections(void)
{
	if ( (FPGA_Error_Latch_H!=0 || FPGA_Error_Latch_L!=0) && (SetTryToResetError==0)) {ErrorSet(ERR_FPGA_FLAGS_PROTECTIONS);}
}

void ProtectionsCheck(void)
{
	//Обработка защит---------------------------------------------------------------------------------------
	FastProtections(); //Защиты по флагам FPGA
	if (ExcitationCurrent>Prot_ExcCurrentMax) {ErrorSet(ERR_EXCITATION_OVERCURRENT);}
	if (fabs(SEG_IEXC2)>Prot_ExcCurrentMax) {ErrorSet(ERR_EXCITATION_OVERCURRENT);}
	if (DCVoltage_OUT>Prot_DCVoltageMax) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}
	if (fabs(SEG_I1_UDC)>Prot_DCVoltageMax) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}
	if (PR_DCOvervoltage.TimedProtectionStep(DCVoltage_OUT)!=0) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}

	//if (SEG_I1_SKIPH2>Prot_IGBTTempMax) {ErrorSet(ERR_PHA_OVERTEMP);}
	//if (SEG_I1_SKIPH3>Prot_IGBTTempMax) {ErrorSet(ERR_PHB_OVERTEMP);}
	//if (SEG_I1_SKIPH1>Prot_IGBTTempMax) {ErrorSet(ERR_PHC_OVERTEMP);}
    //if (SEG_I2_SKIPH2>Prot_IGBTTempMax) {ErrorSet(ERR_PHA_OVERTEMP);}
    //if (SEG_I2_SKIPH3>Prot_IGBTTempMax) {ErrorSet(ERR_PHB_OVERTEMP);}
    //if (SEG_I2_SKIPH1>Prot_IGBTTempMax) {ErrorSet(ERR_PHC_OVERTEMP);}
    //if (SEG_I3_SKIPH2>Prot_IGBTTempMax) {ErrorSet(ERR_PHA_OVERTEMP);}
    //if (SEG_I3_SKIPH3>Prot_IGBTTempMax) {ErrorSet(ERR_PHB_OVERTEMP);}
    //if (SEG_I3_SKIPH1>Prot_IGBTTempMax) {ErrorSet(ERR_PHC_OVERTEMP);}

	//if ((fabs(SEG_I1_SKIPH2 - SEG_I1_SKIPH3) > Prot_IGBTTempUnbalance) || (fabs(SEG_I1_SKIPH3 - SEG_I1_SKIPH1) > Prot_IGBTTempUnbalance) || (fabs(SEG_I1_SKIPH2 - SEG_I1_SKIPH1) > Prot_IGBTTempUnbalance))
	//{ErrorSet(ERR_IGBT_TEMP_UNBALANCE);}
    //if ((fabs(SEG_I2_SKIPH2 - SEG_I2_SKIPH3) > Prot_IGBTTempUnbalance) || (fabs(SEG_I2_SKIPH3 - SEG_I2_SKIPH1) > Prot_IGBTTempUnbalance) || (fabs(SEG_I2_SKIPH2 - SEG_I2_SKIPH1) > Prot_IGBTTempUnbalance))
    //{ErrorSet(ERR_IGBT_TEMP_UNBALANCE);}
    //if ((fabs(SEG_I3_SKIPH2 - SEG_I3_SKIPH3) > Prot_IGBTTempUnbalance) || (fabs(SEG_I3_SKIPH3 - SEG_I3_SKIPH1) > Prot_IGBTTempUnbalance) || (fabs(SEG_I3_SKIPH2 - SEG_I3_SKIPH1) > Prot_IGBTTempUnbalance))
    //{ErrorSet(ERR_IGBT_TEMP_UNBALANCE);}

	//if (GenPhaseVoltage>Prot_GenVoltageMax || GenPhaseVoltage<-1*Prot_GenVoltageMax) {ErrorSet(ERR_GENPHASE_OVERVOLTAGE);}
	//if (GenSpeedFiltered>Prot_GenSpeedMax) {ErrorSet(ERR_GENSPEED_OPERATION_OVERRANGE);}

	//float RectUnbalance1 = fabs(Rect1Current - Rect2Current);
	//float RectUnbalance2 = fabs(Rect2Current - Rect3Current);
	//float RectUnbalance3 = fabs(Rect3Current - Rect1Current);
	//if (RectUnbalance2>RectUnbalance1) {RectUnbalance1 = RectUnbalance2;}
	//if (RectUnbalance3>RectUnbalance1) {RectUnbalance1 = RectUnbalance3;}
	//if (RectUnbalance1>Prot_RectCurrUnbalance) {ErrorSet(ERR_RECTCURRENT_UNBALANCE);}
	//if (PR_RectUnbalance.TimedProtectionStep(RectUnbalance1)!=0) {ErrorSet(ERR_RECTCURRENT_UNBALANCE);}

	//float VoltUnbalance1 = fabs(PhAVoltRMS - PhBVoltRMS);
	//float VoltUnbalance2 = fabs(PhBVoltRMS - PhCVoltRMS);
	//float VoltUnbalance3 = fabs(PhCVoltRMS - PhAVoltRMS);
	//if (VoltUnbalance2>VoltUnbalance1) {VoltUnbalance1 = VoltUnbalance2;}
	//if (VoltUnbalance3>VoltUnbalance1) {VoltUnbalance1 = VoltUnbalance3;}
	//if (PR_OutpVoltageUnbalance.TimedProtectionStep(VoltUnbalance1)!=0) {ErrorSet(ERR_VOLTAGE_UNBALANCE);}

	//float CurrUnbalance1 = fabs(PhAIGBTCurrentRMS - PhBIGBTCurrentRMS);
	//float CurrUnbalance2 = fabs(PhBIGBTCurrentRMS - PhCIGBTCurrentRMS);
	//float CurrUnbalance3 = fabs(PhCIGBTCurrentRMS - PhAIGBTCurrentRMS);
	//if (CurrUnbalance2>CurrUnbalance1) {CurrUnbalance1 = CurrUnbalance2;}
	//if (CurrUnbalance3>CurrUnbalance1) {CurrUnbalance1 = CurrUnbalance3;}
	//if (PR_IGBTCurrUnbalance.TimedProtectionStep(CurrUnbalance1)!=0) {ErrorSet(ERR_IGBT_CURRENT_UNBALANCE);}

	if (PR_IGBTAOverCurr.TimedProtectionStep(PhA1IGBTCurrentRMS)!=0) {ErrorSet(ERR_RMSINVPHA_OVERCURRENT);}
	if (PR_IGBTBOverCurr.TimedProtectionStep(PhB1IGBTCurrentRMS)!=0) {ErrorSet(ERR_RMSINVPHB_OVERCURRENT);}
	if (PR_IGBTCOverCurr.TimedProtectionStep(PhC1IGBTCurrentRMS)!=0) {ErrorSet(ERR_RMSINVPHC_OVERCURRENT);}

	if (PR_OutpPhAOverCurr.TimedProtectionStep(PhACurrentRMS)!=0) {ErrorSet(ERR_RMSPHA_OVERCURRENT);}
	if (PR_OutpPhBOverCurr.TimedProtectionStep(PhBCurrentRMS)!=0) {ErrorSet(ERR_RMSPHB_OVERCURRENT);}
	if (PR_OutpPhCOverCurr.TimedProtectionStep(PhCCurrentRMS)!=0) {ErrorSet(ERR_RMSPHC_OVERCURRENT);}

	//if (PR_APOverload.TimedProtectionStep(ActivePower)!=0) {ErrorSet(ERR_OUTPUT_OVERLOAD);}
	//if (PR_NegativeAPower.TimedProtectionStep(-1.0*ActivePower)!=0) {ErrorSet(ERR_OUTPUT_NEGATIVE_POWER);}
	//if (PR_RPOverload.TimedProtectionStep(ReactivePower)!=0){ErrorSet(ERR_OUTPUT_REACTIVE_OVERLOAD);}
	//if (PR_NegativeRPower.TimedProtectionStep(-1*ReactivePower)!=0) {ErrorSet(ERR_OUTPUT_REACTIVE_OVERLOAD);}

	//if (ActivePower>Unb_ActivePowerMax) {Unb_ActivePowerMax = ActivePower;} else {Unb_ActivePowerMax = Unb_ActivePowerMax - 50;}
	//if (ActivePower<Unb_ActivePowerMin) {Unb_ActivePowerMin = ActivePower;} else {Unb_ActivePowerMin = Unb_ActivePowerMin + 50;}

	//float ActivePowerUnbalance = fabs(Unb_ActivePowerMax - Unb_ActivePowerMin);
	//float FrequencyError = fabs(Const_BaseFrequency - OutputFrequency);

	if (Const_IsResistiveLoadConverter==0)
		{
		//float GroundFaultError = 2.0-(float)(GetDINValue(10));
		//if (PR_GroundFault.TimedProtectionStep(GroundFaultError)!=0) {ErrorSet(ERR_GROUND_FAULT_PROTECTION);}
		}
	else
		{
		//float GroundFaultError = 2.0-(float)(GetDINValue(10));
		//if (InverterActivatedCounts<3300) {GroundFaultError=0;}
		//if (PR_GroundFault.TimedProtectionStep(GroundFaultError)!=0) {ErrorSet(ERR_CONVERTER_COOLING_FAULT);}
		}

	//Защиты при работающем преобразователе
	if (InverterActivated!=0)
	{
		//Мгновенные защиты
		if (Const_SyncShortLaunch!=0)
		{if (InverterActivatedCounts>Const_SyncCyclesToRun) {ErrorSet(ERR_SHORT_SYNC_STOPPED);}}
		//if (VoltUnbalance1>Prot_OutpVoltageUnbalance) {ErrorSet(ERR_VOLTAGE_UNBALANCE);}
		//if (CurrUnbalance1>Prot_IGBTCurrentUnbalance) {ErrorSet(ERR_IGBT_CURRENT_UNBALANCE);}
		//if (ActivePower>Prot_APOverload) {ErrorSet(ERR_OUTPUT_OVERLOAD);}
		//if (ActivePower<Prot_APNegative) {ErrorSet(ERR_OUTPUT_NEGATIVE_POWER);}
		//if (ReactivePower>Prot_RPOverload) {ErrorSet(ERR_OUTPUT_REACTIVE_OVERLOAD);}
		//if (ReactivePower<-1*Prot_RPOverload) {ErrorSet(ERR_OUTPUT_REACTIVE_OVERLOAD);}

		if (SystemManualControl==0)
			{
			//if (DCVoltage_OUT<Prot_DCVoltageLow)  {ErrorSet(ERR_DCOUT_OPERATION_UNDERVOLTAGE);}
			//if (Const_BoosterModeDisable==0) {Prot_GenSpeedOPLow=1000;}
			//else {Prot_GenSpeedOPLow=2000;}
			//if (GenSpeedFiltered<Prot_GenSpeedOPLow || GenSpeedFiltered>Prot_GenSpeedOPHigh) {ErrorSet(ERR_GENSPEED_OPERATION_OVERRANGE);}
			//if (OutputFrequency>Prot_OutpFrequencyHigh || OutputFrequency<Prot_OutpFrequencyLow) {ErrorSet(ERR_OUTPUT_FREQENCY_OVERRANGE);}
			}
	}
	else
	{
		Unb_ActivePowerMax=0;
		Unb_ActivePowerMin=0;
		//FrequencyError=0;
		//ActivePowerUnbalance=0;
	}
	//if (PR_OutputFrequency.TimedProtectionStep(FrequencyError)!=0) {ErrorSet(ERR_OUTPUT_FREQ_OVERRANGE);}
	//if (PR_ActivePowerRegulation.TimedProtectionStep(ActivePowerUnbalance)!=0) {ErrorSet(ERR_ACTIVE_POWER_REGULATION);}

}

