/*
 * ALG_Regulation.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>

void ExcitationRegulation(void)
{
	//Расчет ограничений на возбуждение
	float MaximumExcitationCurrent = Const_ExcitationMax;//28-6*Reg_GenSpeeed/1000; //(10 + 4 + 7*(10-4))/2 - (10-4)*Reg_GenSpeeed/1000.0; //Excitation limitation to avoid overvoltage, 10A at 3000, 4A at 4000
	if (MaximumExcitationCurrent>Const_ExcitationMax) {MaximumExcitationCurrent=Const_ExcitationMax;}
	if (MaximumExcitationCurrent<1) {MaximumExcitationCurrent=1;}


	//Расчет уставки напряжения шины постоянного тока
	Reg_DCVoltageSetpoint = 200;
	if (ChopperManualControl==1) {DCOutSetpoint = (float)((unsigned short)Data_FromM3[11])*900.0/65535.0;} //Ручное управление напряжением DC
	else {DCOutSetpoint = Reg_DCVoltageSetpoint;}
	float BoosterSetpoint = DCOutSetpoint+20.0;
	if (BoosterSetpoint<0) {BoosterSetpoint = 0;}
	if (BoosterSetpoint>900) {BoosterSetpoint = 900;}
	if (Const_BoosterModeDisable==0){Booster_Voltage_Setpoint = (unsigned short)(BoosterSetpoint);}
	else {Booster_Voltage_Setpoint=0;}

	// Регулятор возбуждения -----------------------------------------------------------------------------
	// Регулятор напряжения на шине постоянного тока -----------------------------------------------------
	if (ExcitationActivated!=0)
	{
		DCVoltagePID->SetProp(DCVPID_P);

		if (SystemManualControl==1) {Reg_DCInpSetpoint = DCOutSetpoint; }
		else {Reg_DCInpSetpoint = Alg_DCInpSetpoint;}
		DCVoltageError = DCOutSetpoint - UDCOUTFiltered;
		DCVoltagePID->PIDStep(DCVoltageError);
		DCVoltagePID->SetUpperIntegratorLimit(Alg_Excitation_limit);
		DCVoltagePID->SetUpperOutputLimit(Alg_Excitation_limit);

		// Регулятор тока возбуждения --------------------------------------------------------------------------
		ExcitationPWLimit = 2400.0;
		if (ExcitationManualControl==1) {ExcitationSetpoint = (float)((unsigned short)Data_FromM3[10])*40.0/65535.0;}
		else {
			 Reg_ExcSetpoint = DCVoltagePID->CurrentOutput;
			 if (Reg_ExcSetpoint>MaximumExcitationCurrent) {Reg_ExcSetpoint=MaximumExcitationCurrent;}
			 if (Reg_ExcSetpoint<1.0) {Reg_ExcSetpoint=1.0;}
			 ExcitationSetpoint = Reg_ExcSetpoint;
			 }
		ExcitationCurrentPID->SetProp(EXCPID_P); ExcitationCurrentPID->SetInt(EXCPID_I);
		ExcitationCurrentPID->SetUpperIntegratorLimit(ExcitationPWLimit);
		ExcitationCurrentPID->SetUpperOutputLimit(ExcitationPWLimit);
		ExcitationCurrentPID->PIDStep(ExcitationSetpoint - ExcitationCurrentFiltered);
	}
	else
		{
		ExcitationCurrentPID->ResetData(0,0);
		DCVoltagePID->ResetData(0,0);
		DCVoltagePID->SetInt(DCVPID_I*0.01);
		}

	if (ExcitationCurrentPID->CurrentOutput<100) {ExcitationPWM_Width = 0;}
	else if (ExcitationCurrentPID->CurrentOutput>2480) {ExcitationPWM_Width = 2400;}
	else {ExcitationPWM_Width = (Uint16)(ExcitationCurrentPID->CurrentOutput);}
	// ----------------------------------------------------------------------------------------------------
}


void DCBoostRegulation(void)
{
	// Регулятор напряжения шины инвертора-----------------------------------------------------------------
	    	/*
	    	if (ChopperManualControl==1) {	DCOutSetpoint = (float)((unsigned short)Data_FromM3[11])*550.0/65535.0;}
	    	    	else {DCOutSetpoint = 450;}


	    	if (DCVoltage_OUT>10) {PW_ZeroVRise = 1.0-DCVoltage_IN/DCVoltage_OUT;} else  {PW_ZeroVRise = 0;}
	    	if (PW_ZeroVRise<0) {PW_ZeroVRise=0;}
	    	else if (PW_ZeroVRise>0.9) {PW_ZeroVRise=0.9;}
	    	if (DCVoltage_IN>10) {PW_MaxRiseLimit = 225.0/DCVoltage_IN;} else {PW_MaxRiseLimit = 1.0;}
	    	if (PW_MaxRiseLimit<0) {PW_MaxRiseLimit=0;} else if (PW_MaxRiseLimit>0.9) {PW_MaxRiseLimit=0.9;}

	    	if (PW_MaxRiseLimit<(PW_ZeroVRise+0.2)) {PW_MaxLimit = PW_MaxRiseLimit; } else {PW_MaxLimit = (PW_ZeroVRise+0.2);}
	    	if (PW_MaxLimit<0) {PW_MaxLimit=0;} else if (PW_MaxLimit>0.9) {PW_MaxLimit=0.9;}


	    	if (InverterActivatedCounts>10000)
	    	    	{
	    	    		if (ActivePower>4000)
	    	    		{
	    	    			if (ActivePower>8000)
	    	    				{
	    	    				if (ActivePower>12000)
	    							{
	    	    						ChopperPID->SetProp(0.0002); //0.002 - default
	    	    					    ChopperPID->SetInt(0.0002);  //0.008 - default
	    							}
	    	    				else if (ActivePower<10000)
	    	    					{
	    	    					ChopperPID->SetProp(0.0002); //0.002 - default
	    	    					ChopperPID->SetInt(0.0002);  //0.008 - default
	    	    					}
	    	    				}
	    	    			else if (ActivePower<6000)
	    						{
	    						ChopperPID->SetProp(0.0004); //0.002 - default
	    						ChopperPID->SetInt(0.0004);  //0.008 - default
	    						}
	    	    		}
	    	    		else if (ActivePower<2000)
	    	    		{
	    	    			ChopperPID->SetProp(0.002); //0.002 - default
	    	    			ChopperPID->SetInt(0.008);  //0.008 - default
	    	    		}
	    	    	} else {ChopperPID->SetProp(0.002);ChopperPID->SetInt(0.008);}

	    	    	if (ChopperActivated==1) {
	    	    		if (ChopperCurrent1 > 200 || ChopperCurrent2>200){
	    					ChopperPID->SetUpperIntegratorLimit(PW_ZeroVRise);
	    					ChopperPID->SetUpperOutputLimit(PW_ZeroVRise);
	    					ChopperPID->PIDStep(-20);
	    					ChopperPWM_Width = ChopperPID->CurrentOutput*10000.0;}
	    				else {
	    					ChopperPID->SetUpperIntegratorLimit(PW_MaxLimit);
	    					ChopperPID->SetUpperOutputLimit(PW_MaxLimit);
	    					float DCVoltageChoppError = DCOutSetpoint - DCVoltage_OUT;
	    					if (DCVoltageChoppError>50) {DCVoltageChoppError=50;}
	    					ChopperPID->PIDStep(DCVoltageChoppError);
	    					ChopperPWM_Width = ChopperPID->CurrentOutput*10000.0;}}
	    	    	else {
	    	    		ChopperPID->ResetData(0,0);
	    	    		ChopperPWM_Width = 0;}
	    	    */
	    	// ----------------------------------------------------------------------------------------------------
}

void RotationSpeedRegulation(void)
{
	// Регулятор частоты вращения генератора --------------------------------------------------------------
	//GenSpeedRegLowpassFilter->Input(GenSpeedFiltered);
	//GenSpeedDoubleFiltered = GenSpeedRegLowpassFilter->CurrentOutput;
	/*if (Rotation_Power_ControlMode==1)
		{
		RotationSpeedPID->ResetData(0,0);
		ActivePowerSetpointDist = ((float)((short)Data_FromM3[14]))*10.0;
		if (ActivePowerSetpointDist>550000) ActivePowerSetpointDist=550000;
		if (ActivePowerSetpointDist<0) ActivePowerSetpointDist=0;
		if (ActivePowerSetpoint<(ActivePowerSetpointDist-10)) {ActivePowerSetpoint = ActivePowerSetpoint + 2;}
		if (ActivePowerSetpoint>(ActivePowerSetpointDist+10)) {ActivePowerSetpoint = ActivePowerSetpoint - 2;}
		}
	else
		{
		RotSpeedSetpoint = ((float)((short)Data_FromM3[15]));

		if (RotSpeedSetpoint<500) {RotSpeedSetpoint=500;}
		if (RotSpeedSetpoint>900) {RotSpeedSetpoint=900;}
		if (InverterActivated!=0)
			{
				float SpeedError = GenSpeedFiltered-RotSpeedSetpoint;
				float SpeedLimit = -50.0*(ExcitationCurrent - 9);
				if (SpeedLimit<0) {SpeedError = SpeedLimit;}
				if (SpeedError>100) {SpeedError=100;}
				if (SpeedError<-100) {SpeedError=-100;}
				RotationSpeedPID->PIDStep(SpeedError);
				ActivePowerSetpointDist = RotationSpeedPID->CurrentOutput;
				if (ActivePowerSetpointDist>550000) ActivePowerSetpointDist=550000;
				if (ActivePowerSetpointDist<10000) ActivePowerSetpointDist=10000;
				if (ActivePowerSetpoint<(ActivePowerSetpointDist-20)) {ActivePowerSetpoint = ActivePowerSetpoint + 8;}
				if (ActivePowerSetpoint>(ActivePowerSetpointDist+20)) {ActivePowerSetpoint = ActivePowerSetpoint - 8;}
			}
			else
			{
				RotationSpeedPID->ResetData(0,0);
				ActivePowerSetpoint=0;
			}
		}
	if (ActivePowerSetpoint>InverterMaximumPower) {ActivePowerSetpoint = InverterMaximumPower;}*/
	// ----------------------------------------------------------------------------------------------------
}


void UnitLoadRegulation(void)
{
	if (Const_IsResistiveLoadConverter==0)
	{
		if (Const_IsConnectedToResistiveLoad==0)
			{
				Reg_APowerSetpoint = (Const_BaseFrequency + Const_FrequencyWindow - OutputFrequencyFiltered)*(Const_MaxOutputPower-Alg_PowerDamping)/Const_FrequencyWindow;
			}
		else
			{
				if (Const_ActivateLoadControl==0) {Alg_BaseFrequency=Const_BaseFrequency + Const_FrequencyWindow;	}
				else {Alg_BaseFrequency= Const_BaseFrequency + Const_FrequencyWindow*0.5;}
				Reg_APowerSetpoint = (Alg_BaseFrequency - OutputFrequencyFiltered)*(Const_MaxOutputPower-Alg_PowerDamping)/Const_FrequencyWindow;
			}

		if (Reg_APowerSetpoint > (Const_MaxOutputPower)) {Reg_APowerSetpoint = (Const_MaxOutputPower);}
		if (Reg_APowerSetpoint < (Const_MinOutputPower)) {Reg_APowerSetpoint = (Const_MinOutputPower);}

		if (InverterActivated!=0)
			{
			FreqToPowerPID->SetUpperIntegratorLimit(Const_MaxOutputPower-Alg_PowerDamping);
			FreqToPowerPID->SetUpperOutputLimit(Const_MaxOutputPower-Alg_PowerDamping);
			PIDActivePowerSetpoint = FreqToPowerPID->PIDStep(Reg_APowerSetpoint - ActivePower);
			}
		else
			{
			FreqToPowerPID->ResetData(0,10000);
			}
	}


}

void ActivePowerRegulation(void)
{
	// Регулятор активной мощности (частоты) инвертора-----------------------------------------------------------------
	if (InverterFrequencyManualControl==1)
	{float TargetFrequency = 49.0+(((float)Data_FromM3[2])*2.0/65535);  OutputFrequency = TargetFrequency; MainPWM_PhaseShift=0; }
	else   	{
				if (Const_IsResistiveLoadConverter==1)
				{
					OutputFrequency=50.0;
					MainPWM_PhaseShift=0;
					MainPWM_PhaseSet=0;
				}
				else
				{
					//Регулятор P
					if (InverterActivated!=0)
					{
						if (SystemManualControl==0)
						{
							//Underload regulation boost during negative power--------------
							UnderloadBoost = UDCOUTFiltered - NOMINAL_DC_VOLTAGE;
							UnderloadBoost = UnderloadBoost - Const_MaxVoltageBoost;
							if (UnderloadBoost<0) {UnderloadBoost=0;}
							if (UnderloadBoost>200) {UnderloadBoost=200;}
							UnderloadBoost = UnderloadBoost*Const_UnderloadBoostPerVolt;
							if (ActivePower>10000) {UnderloadBoost=0;}

							//Overload regulation boost ---------------------------------
							OverloadBoost = NOMINAL_DC_VOLTAGE - UDCOUTFiltered;
							OverloadBoost = OverloadBoost - Const_MaxVoltageBoost;
							if (OverloadBoost<0) {OverloadBoost=0;}
							if (OverloadBoost>200) {OverloadBoost=200;}
							OverloadBoost = OverloadBoost*Const_OverloadBoostPerVolt;
							if (ActivePower<50000) {OverloadBoost=0;}
						} else {UnderloadBoost=0;OverloadBoost=0;}

						//Regulation boost calculations
						PIDActivePowerSetpoint = PIDActivePowerSetpoint + UnderloadBoost - OverloadBoost;
						Reg_ActivePowerError = fabs(PIDActivePowerSetpoint - ActivePower);

						if (Reg_ActivePowerError>20000)
						    			{
						    			if (Reg_ActivePowerError>40000)
						    				{
						    					if (Reg_ActivePowerError>60000)
						    					{
						    						if (Reg_ActivePowerError>100000)
						    						{
						    							if (Reg_ActivePowerError>200000) {Reg_ActivePowerPIDBoost = 4;} //Maximal
						    							else if (Reg_ActivePowerError<150000) {Reg_ActivePowerPIDBoost = 2;}
						    						}
						    						else if (Reg_ActivePowerError<80000) {Reg_ActivePowerPIDBoost = 1;}
						    					}
						    					else if (Reg_ActivePowerError<50000)	{Reg_ActivePowerPIDBoost = 0.5;}
						    				}
						    			else if (Reg_ActivePowerError<30000) {Reg_ActivePowerPIDBoost = 0.25;}
						    			}
						else if (Reg_ActivePowerError<10000) {Reg_ActivePowerPIDBoost = 0.1;} //Minimal

						if (Reg_ActivePowerError>5000)
										{
										if (Reg_ActivePowerError>10000)
											{
												if (Reg_ActivePowerError>20000)
												{
													if (Reg_ActivePowerError>40000) {Reg_ActivePowerPIDBoost2 = 0.1;} //Maximal
													else if (Reg_ActivePowerError<30000) {Reg_ActivePowerPIDBoost2 = 0.15;}
												}
												else if (Reg_ActivePowerError<15000)	{Reg_ActivePowerPIDBoost2 = 0.25;}
											}
										else if (Reg_ActivePowerError<7500) {Reg_ActivePowerPIDBoost2 = 0.5;}
										}
						else if (Reg_ActivePowerError<2500) {Reg_ActivePowerPIDBoost2 = 1;} //Minimal

						Reg_TotalActivePowerBoost = 1;

						if (UnderloadBoost>0 || OverloadBoost>0) {Reg_TotalActivePowerBoost=Reg_TotalActivePowerBoost*1;}

						PhasePID->SetProp(PHPID_P*Reg_ActivePowerPIDBoost2);
						PhasePID->SetInt(PHPID_I*Reg_ActivePowerPIDBoost2);
						FrequencyPID->SetProp(FRPID_P*Reg_ActivePowerPIDBoost);
						FrequencyPID->SetInt(FRPID_I*Reg_ActivePowerPIDBoost);

						// Regulation ------------------------------------------------

						FrequencyPID->PIDStep(PIDActivePowerSetpoint - ActivePower);
						OutputFrequency = FrequencyPID->CurrentOutput;
						if (OutputFrequency<48.0) {OutputFrequency=48.0;}
						if (OutputFrequency>52.0) {OutputFrequency=52.0;}
						PhasePID->PIDStep(PIDActivePowerSetpoint - ActivePower);
						float OutputFrequencyTicks = 50000000.0/OutputFrequency;
						MainPWM_PhaseShift = PhasePID->CurrentOutput* Const_1_div360 * OutputFrequencyTicks;
					}
					else
					{
						FrequencyPID->ResetData(OutputFrequency,OutputFrequency);
						if (OutputFrequency<48.0) {OutputFrequency=48.0;}
						if (OutputFrequency>52.0) {OutputFrequency=52.0;}
						PhasePID->ResetData(0,0);
						float OutputFrequencyTicks = 50000000.0/OutputFrequency;
						MainPWM_PhaseShift = PhasePID->CurrentOutput* Const_1_div360 * OutputFrequencyTicks;
					}
				}
			}

	if (OutputFrequency>52)  {MainPWM_OutputFrequency= (Uint32)(50000000.0/52.0);}
	else if (OutputFrequency<48) {MainPWM_OutputFrequency= (Uint32)(50000000.0/48.0);}
	else {MainPWM_OutputFrequency= (Uint32)(50000000.0/OutputFrequency);}
	// ----------------------------------------------------------------------------------------------------
}

void ReactivePowerRegulation(void)
{
	// Регулятор реактивной мощности (напряжения) инвертора-----------------------------------------------------------------
	if (InverterVoltageManualControl==1)  {MainPWM_AMP=Data_FromM3[1];VoltagePID->ResetData(0,0);ReactivePowerPID->ResetData(0,0);}
	else {
			if (Const_IsResistiveLoadConverter==1) //Регулятор конвертера нагрузки
			{
				Reg_VoltageError = UDCOUTFiltered - (Alg_DCOutSetpoint+25);
				if (Reg_VoltageError>Const_VoltageErrorUpperLimit) {Reg_VoltageError=Const_VoltageErrorUpperLimit;}
				if (Reg_VoltageError<Const_VoltageErrorLowerLimit) {Reg_VoltageError=Const_VoltageErrorLowerLimit;}

				if (InverterActivated!=0)
				{
					ReactivePowerPID->PIDStep(Reg_VoltageError); //Power demand regulator
					ReactivePowerSetpointReg = ReactivePowerPID->CurrentOutput;
					//ReactivePowerSetpointReg = ReactivePowerSetpointReg - Alg_PowerDamping;
					if (ReactivePowerSetpointReg<0) {ReactivePowerSetpointReg = 0;}
					if (ReactivePowerSetpointReg>Const_ResitiveLoadMaxPower) {ReactivePowerSetpointReg = Const_ResitiveLoadMaxPower;}
					VoltagePID->PIDStep(ReactivePowerSetpointReg - DCPower); //PWM Amp (output voltage) regulator
					MainPWM_AMP = (Uint16)(VoltagePID->CurrentOutput*10000.0);
				}
				else
				{
					ReactivePowerPID->ResetData(0,0);
					VoltagePID->ResetData(0,0);
					MainPWM_AMP=0;
				}
			}
			else
			{
				//Регулятор Q -->> StartingPWMAmp = 2*StartingVoltageMagnitude/DCOutSetpoint;
				//RegVoltageErrorFilter->Input(Const_OutputVoltageSetpoint - OutputRMSVoltage);
				float CounterValue = (1000000 - ReactivePower)*0.01;
				if (CounterValue<200) {CounterValue=200;}
				if (CounterValue>32000) {CounterValue = 32000;}
				Reg_ReactiveDropCounterValue = (Uint16)CounterValue;
				Reg_VoltageError = Const_OutputVoltageSetpoint - OutputRMSVoltage;//RegVoltageErrorFilter->CurrentOutput;
				if (Reg_ReactiveDropCounter>0) {Reg_ReactiveDropCounter--;}
				else {Reg_VoltageError = Reg_VoltageError - 1; Reg_ReactiveDropCounter = Reg_ReactiveDropCounterValue;}
				if (Reg_VoltageError>Const_VoltageErrorUpperLimit) {Reg_VoltageError=Const_VoltageErrorUpperLimit;}
				if (Reg_VoltageError<Const_VoltageErrorLowerLimit) {Reg_VoltageError=Const_VoltageErrorLowerLimit;}

				if (InverterActivated!=0)
					{
						ReactivePowerPID->PIDStep(Reg_VoltageError);
						ReactivePowerSetpointReg = ReactivePowerPID->CurrentOutput;
						Reg_ReactivePowerBoost = Reg_VoltageError*Const_ReactivePowerBoost;
						ReactivePowerSetpointReg = ReactivePowerSetpointReg + Reg_ReactivePowerBoost;

						//Voltage regulator boost calculations
						Reg_ReactivePowerError = fabs(ReactivePowerSetpoint - ReactivePower);
						if (Reg_ReactivePowerError>50000)
										{
										if (Reg_ReactivePowerError>100000)
											{
												if (Reg_ReactivePowerError>150000) {Reg_ReActivePowerPIDBoost = 1;} //Maximal
												else if (Reg_ReactivePowerError<125000)	{Reg_ReActivePowerPIDBoost = 0.75;}
											}
										else if (Reg_ReactivePowerError<75000) {Reg_ReActivePowerPIDBoost = 0.5;}
										}
						else if (Reg_ReactivePowerError<25000) {Reg_ReActivePowerPIDBoost = 0.25;} //Minimal
						VoltagePID->SetProp(VPID_P*Reg_ReActivePowerPIDBoost);
						VoltagePID->SetInt(VPID_I*Reg_ReActivePowerPIDBoost);

						//Regulation
						ReactivePowerSetpoint = ReactivePowerSetpointReg;
						VoltagePID->PIDStep(ReactivePowerSetpoint - ReactivePower);
						float PWMAMP_Reg = (VoltagePID->CurrentOutput);
						//if (UDCOUTFilteredSlow>1)	{PWMAMP_Reg = PWMAMP_Reg * DCOutSetpoint/UDCOUTFilteredSlow;}
						//else {PWMAMP_Reg = 1;}
						if (PWMAMP_Reg>1) {PWMAMP_Reg=1;}
						if (PWMAMP_Reg<0) {PWMAMP_Reg=0;}
						MainPWM_AMP = (Uint16)(PWMAMP_Reg*10000.0);
					}
					else
					{
						ReactivePowerSetpoint = 0;
						ReactivePowerSetpointReg = 0;
						ReactivePowerPID->ResetData(0,4000);
						float PWMAMP_Reg = StartingPWMAmp;
						if (UDCOUTFilteredSlow>1)	{PWMAMP_Reg = PWMAMP_Reg * DCOutSetpoint/UDCOUTFilteredSlow;}
						else {PWMAMP_Reg = 1;}
						if (PWMAMP_Reg>1) {PWMAMP_Reg=1;}
						if (PWMAMP_Reg<0) {PWMAMP_Reg=0;}
						VoltagePID->ResetData(0,PWMAMP_Reg);
						MainPWM_AMP = PWMAMP_Reg;
					}
			}
		 }
	// ----------------------------------------------------------------------------------------------------
}

void StateRecognition(void)
{
	if (ExcitationActivated!=0) {SystemState|=0x0001;} else {SystemState&=0xFFFE;}
	if (ChopperActivated!=0) {SystemState|=0x0002;} else {SystemState&=0xFFFD;}
	if (InverterActivated!=0) {SystemState|=0x0004;}  else {SystemState&=0xFFFB;}

	if (SystemManualControl==0) {SystemState&=0xFFF7;} else {SystemState|=0x0008;}
	if (ExcitationManualControl==0) {SystemState&=0xFFEF;} else {SystemState|=0x0010;}
	if (ChopperManualControl==0) {SystemState&=0xFFDF;} else {SystemState|=0x0020;}
	if (InverterVoltageManualControl==0) {SystemState&=0xFFBF;} else {SystemState|=0x0040;}
	if (InverterFrequencyManualControl==0) {SystemState&=0xFF7F;} else {SystemState|=0x0080;}
	if (VentilationManualControl==0) {SystemState&=0xFEFF;} else {SystemState|=0x0100;}
	if (Isolated_Grid_ControlMode==0) {SystemState&=0xFDFF;} else {SystemState|=0x0200;}
	if (Rotation_Power_ControlMode==0) {SystemState&=0xFBFF;} else {SystemState|=0x0400;}

	if (CurrentSysytemState==0) {SystemState&=0xE7FF;}
	else if (CurrentSysytemState==1) {SystemState&=0xE7FF;SystemState|=0x0800;}
	else if (CurrentSysytemState==2) {SystemState&=0xE7FF;SystemState|=0x1000;}
	else if (CurrentSysytemState==3) {SystemState&=0xE7FF;SystemState|=0x1800;}
}

void CoolingControl(void)
{
	//Cooler fan PWM control
	if (ExcitationActivated==0 && ChopperActivated==0 && InverterActivated==0) {CoolerFanPWM_Width=0;}
	else {CoolerFanPWM_Width=5000;}
	if (Data_FromM3[5] > CoolerFanPWM_Width) {CoolerFanPWM_Width=Data_FromM3[5];}
	//if (ExcitationActivated!=0) {CoolerFanPWM_Width=1000;SystemState|=0x0001;} else {SystemState&=0xFFFE;}
	//if (ChopperActivated!=0) {CoolerFanPWM_Width=5000;SystemState|=0x0002;} else {SystemState&=0xFFFD;}
	//if (InverterActivated!=0) {CoolerFanPWM_Width=10000;SystemState|=0x0004;}  else {SystemState&=0xFFFB;}
	//if (Data_FromM3[5] > CoolerFanPWM_Width) {CoolerFanPWM_Width=Data_FromM3[5];}
	//if (MinimumCoolingPWMWidth > CoolerFanPWM_Width) {CoolerFanPWM_Width=MinimumCoolingPWMWidth;}
}

