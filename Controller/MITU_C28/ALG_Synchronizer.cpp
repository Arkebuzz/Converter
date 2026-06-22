/*
 * ALG_Synchronizer.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */


#include <GlobalData.h>

void ActivateSynchronizer(Uint16 Activate)
{
	if (Activate!=0)
	{
		SynchronizerActive = 1;
		FreqUpdateCounter = 0;
		PhaseMinValue = FVAL_VoltagePhase;
		PhaseMaxValue = FVAL_VoltagePhase;
		FreqMinValue = MeasuredOutputFrequency;
		FreqMaxValue = MeasuredOutputFrequency;
		FVAL_InputFreq = 60.0;
	}
	else
	{
		SynchronizerActive = 0;
		FreqUpdateCounter = 0;
		PhaseMinValue = 0;
		PhaseMaxValue = 0;
		FreqMinValue = 0;
		FreqMaxValue = 0;
		FVAL_InputFreq = 60.0;
		SyncLaunch=0;
	}
}

void CalcFundamentalValues(void)
{
	/*FVAL_InputPhase = FVAL_InputPhase + Const_CycleTime*Const_2pi*FVAL_InputFreq;
	if (FVAL_InputPhase>Const_2pi) {FVAL_InputPhase = FVAL_InputPhase - Const_2pi;}
	FVAL_InputSin = sin(FVAL_InputPhase);
	FVAL_InputSin_p2pidiv3 = sin(FVAL_InputPhase + Const_2pi_div3);
	FVAL_InputSin_m2pidiv3 = sin(FVAL_InputPhase - Const_2pi_div3);
	FVAL_InputCos =cos(FVAL_InputPhase);
	FVAL_InputCos_p2pidiv3 = cos(FVAL_InputPhase + Const_2pi_div3);
	FVAL_InputCos_m2pidiv3 = cos(FVAL_InputPhase - Const_2pi_div3);
	Ptransform_d = Const_2_div3*(PhaseAVoltage*FVAL_InputSin + PhaseBVoltage*FVAL_InputSin_m2pidiv3 + PhaseCVoltage*FVAL_InputSin_p2pidiv3);
	Ptransform_q = Const_2_div3*(PhaseAVoltage*FVAL_InputCos + PhaseBVoltage*FVAL_InputCos_m2pidiv3 + PhaseCVoltage*FVAL_InputCos_p2pidiv3);
	dFilter->Input(Ptransform_d); Ptransform_d = dFilter->CurrentOutput;
	qFilter->Input(Ptransform_q); Ptransform_q = qFilter->CurrentOutput;
	FVAL_VoltageMagnitude = sqrt(Ptransform_d*Ptransform_d + Ptransform_q*Ptransform_q);
	FVAL_VoltagePhase = atan2(Ptransform_q,Ptransform_d)*180.0/Const_pi;
	FVAL_VoltagePhaseArray[MainCycleMeasurmentsCounter] = FVAL_VoltagePhase;
	DelayedPhasePointer = MainCycleMeasurmentsCounter - Const_DelayedPhaseCounts;
	if (DelayedPhasePointer<0) {DelayedPhasePointer = DelayedPhasePointer + MEASURMENTS_BUFFER_SIZE;}
	FVAL_VoltageDelayedPhase = FVAL_VoltagePhaseArray[DelayedPhasePointer];
	FVAL_VoltagePhaseChange = FVAL_VoltagePhase - FVAL_VoltageDelayedPhase;
	if (FVAL_VoltagePhaseChange<0) {FVAL_VoltagePhaseChange = FVAL_VoltagePhaseChange + 360;}
	if (FVAL_VoltagePhaseChange>=180) {FVAL_VoltagePhaseChange = FVAL_VoltagePhaseChange - 360;}
	FVAL_MeasuredOutputFrequency = FVAL_VoltagePhaseChange * Const_DelayedPhaseFrequency / 360 + FVAL_InputFreq;
	FreqFilter->Input(FVAL_MeasuredOutputFrequency);
	MeasuredOutputFrequency = FreqFilter->CurrentOutput;*/
}

void SynchronizerOperation(void)
{
	// Синхронизация с сетевым напряжением  -----------------------------------------------------------------------------
	if (SynchronizerActive==1)
	{
		//ActivateSyncout(0); //TEMP!!

		//Подгонка частоты
		if (FreqUpdateCounter>Const_FreqUpdateDelay || Const_IsResistiveLoadConverter==1)
			{
			FreqUpdateCounter=0;
			PhaseChangeAmp = fabs(PhaseMaxValue - PhaseMinValue);
			FreqChangeAmp = fabs(FreqMaxValue-FreqMinValue);
			//Проверка рассогласования угла и частоты
			if (FreqChangeAmp<Const_SyncMaxFreqChange
					&& PhaseChangeAmp<Const_SyncMaxPhaseChange
					&& FVAL_VoltageMagnitude>Const_SyncMinVoltageMag
					&& Const_IsResistiveLoadConverter==0
					&& FVAL_InputFreq>Const_FreqLowerLim
					&& FVAL_InputFreq<Const_FreqUpperLim) //Разрешение включения на сеть
				{
				OutputFrequency = FVAL_InputFreq;
				OutputPhaseSet = FVAL_VoltagePhase + FVAL_InputPhase*180.0/Const_pi + 5.4 + 0.9 + Const_SyncStarPhaseLead; //50uS ADCdelay=0.9 deg, 300uS cycle = 5.4 deg, starts at 9.2 deg
				if (OutputPhaseSet<0) {OutputPhaseSet = OutputPhaseSet+360;}
				if (OutputPhaseSet>360) {OutputPhaseSet = OutputPhaseSet-360;}
				float OutputFrequencyTicks = 50000000.0/OutputFrequency;
				MainPWM_PhaseSet = OutputFrequencyTicks*(OutputPhaseSet/360.0);
				//if (OutputPhaseSet<3) {ActivateSyncout(1);} //TEMP!!
				StartingVoltageMagnitude = FVAL_VoltageMagnitude+10;
				if (UDCOUTFiltered>1)	{StartingPWMAmp = 2*StartingVoltageMagnitude/UDCOUTFiltered;}
				else {StartingPWMAmp=1;}
				if (StartingPWMAmp>1) {StartingPWMAmp=1;}
				if (StartingPWMAmp<0) {StartingPWMAmp=0;}
				// Синхронный запуск на следующем цикле
				if (Const_SyncLaunchLocked==0)
					{
					Alg_PowerDamping = Const_MaxOutputPower-20000;
					SyncLaunch=1;
					SynchronizerActive=0;
					}
				}
			else if (FVAL_VoltageMagnitude<30 || Const_IsResistiveLoadConverter==1) //Разрешение включения на изолированную нагрузку
				{
				OutputFrequency = Const_BaseFrequency;
				OutputPhaseSet = 0;
				float OutputFrequencyTicks = 50000000.0/OutputFrequency;
				MainPWM_PhaseSet = OutputFrequencyTicks*(OutputPhaseSet/360.0);
				if (Const_IsResistiveLoadConverter==0){StartingVoltageMagnitude = FVAL_VoltageMagnitude;}
				else {StartingVoltageMagnitude = 0;}
				if (DCOutSetpoint>1)	{StartingPWMAmp = 2*StartingVoltageMagnitude/DCOutSetpoint;}
				else {StartingPWMAmp=1;}
				if (StartingPWMAmp>1) {StartingPWMAmp=1;}
				if (StartingPWMAmp<0) {StartingPWMAmp=0;}
				// Синхронный запуск на следующем цикле
				if (Const_SyncLaunchLocked==0)
					{
					Alg_PowerDamping = Const_MaxOutputPower-20000;
					SyncLaunch=1;
					SynchronizerActive=0;
					}
				}
			else //Подстройка частоты
				{
				PhaseMinValue = FVAL_VoltagePhase;
				PhaseMaxValue = FVAL_VoltagePhase;
				FreqMinValue = MeasuredOutputFrequency;
				FreqMaxValue = MeasuredOutputFrequency;
				FVAL_InputFreq = MeasuredOutputFrequency;
				}
			}
		else
			{
			FreqUpdateCounter++;
			if (PhaseMinValue>FVAL_VoltagePhase) {PhaseMinValue = FVAL_VoltagePhase;}
			if (PhaseMaxValue<FVAL_VoltagePhase) {PhaseMaxValue = FVAL_VoltagePhase;}
			if (FreqMinValue>MeasuredOutputFrequency) {FreqMinValue = MeasuredOutputFrequency;}
			if (FreqMaxValue<MeasuredOutputFrequency) {FreqMaxValue = MeasuredOutputFrequency;}
			OutputFrequency = FVAL_InputFreq;
			if (OutputFrequency<58.0) {OutputFrequency=58.0;}
			if (OutputFrequency>62.0) {OutputFrequency=62.0;}
			float OutputPhaseSet = FVAL_VoltagePhase + FVAL_InputPhase*180.0/Const_pi + 5.4 + 0.9 + Const_SyncStarPhaseLead; //50uS ADCdelay=0.9 deg, 300uS cycle = 5.4 deg, starts at 9.2 deg
			if (OutputPhaseSet<0) {OutputPhaseSet = OutputPhaseSet+360;}
			if (OutputPhaseSet>360) {OutputPhaseSet = OutputPhaseSet-360;}
			//if (OutputPhaseSet<3) {ActivateSyncout(1);} //TEMP!!
			float OutputFrequencyTicks = 50000000.0/OutputFrequency;
			MainPWM_PhaseSet = OutputFrequencyTicks*(OutputPhaseSet/360.0);
			StartingVoltageMagnitude = FVAL_VoltageMagnitude;
			if (DCOutSetpoint>1)	{StartingPWMAmp = 2*StartingVoltageMagnitude/DCOutSetpoint;}
			else {StartingPWMAmp=1;}
			if (StartingPWMAmp>1) {StartingPWMAmp=1;}
			if (StartingPWMAmp<0) {StartingPWMAmp=0;}
			}
	}

	// ----------------------------------------------------------------------------------------------------

}
