/*
 * ALG_MainCycleFunctions.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>

float CalculateNTC(Uint16 value)
{
    bool not_found = true;
    float result = fabs((float)value - 256.0);
    int index = 1;
    if (result>=NTC_LUT[6]) {result = 175.0;}
    else
    {
        for (int i=1; i<7 && not_found ; i++)
        {
            if (result < NTC_LUT[i]) {index=i-1; not_found=false;}
        }
        result = (float)index*25.0 + (result-NTC_LUT[index])*25.0/(NTC_LUT[index+1]-NTC_LUT[index]) + 5.0;
    }
    return result;
}


void CalculateValues(void)
{
	// Values calculation --------------------------------------------------------------------------
	ChopperCurrent = fabs((((float)DMABufFPGA1[18])-1706.66)*0.0390625);
    ExcitationCurrent = fabs((((float)DMABufFPGA1[19])-1706.66)*0.0390625);
	ExcitationCurrent2 = fabs((((float)DMABufFPGA1[20])-1706.66)*0.0390625);
	DCVoltage_IN = fabs(((float)DMABufFPGA1[7]-2070.0)*1.271565755);
	DCVoltage_OUT = fabs(((float)DMABufFPGA1[8]-2070.0)*1.271565755);
	EXCTemp1 = fabs(((float)DMABufFPGA1[21]-2048.0)*0.08477105);
	EXCTemp2 = fabs(((float)DMABufFPGA1[22]-2048.0)*0.08477105);
	EXCTemp3 = fabs(((float)DMABufFPGA1[23]-2048.0)*0.08477105);

	Current_U1 = fabs(((float)DMABufFPGA1[24]-1706.66)*0.366210938);
	Current_V1 = fabs(((float)DMABufFPGA1[25]-1706.66)*0.366210938);
	Current_W1 = fabs(((float)DMABufFPGA1[26]-1706.66)*0.366210938);
	Voltage_1 = fabs(((float)DMABufFPGA1[27]-2070.0)*1.271565755);
	Temp1_1 = CalculateNTC(DMABufFPGA1[28]);
	Temp1_2 = CalculateNTC(DMABufFPGA1[29]);
	Temp1_3 = CalculateNTC(DMABufFPGA1[30]);

	Current_U2 = fabs(((float)DMABufFPGA1[32]-1706.66)*0.366210938);
	Current_V2 = fabs(((float)DMABufFPGA1[33]-1706.66)*0.366210938);
	Current_W2 = fabs(((float)DMABufFPGA1[34]-1706.66)*0.366210938);
	Voltage_2 = fabs(((float)DMABufFPGA1[35]-2070.0)*1.271565755);
	Temp2_1 = CalculateNTC(DMABufFPGA1[36]);
	Temp2_2 = CalculateNTC(DMABufFPGA1[37]);
	Temp2_3 = CalculateNTC(DMABufFPGA1[38]);

	Current_U3 = fabs(((float)DMABufFPGA1[40]-1706.66)*0.366210938);
    Current_V3 = fabs(((float)DMABufFPGA1[41]-1706.66)*0.366210938);
    Current_W3 = fabs(((float)DMABufFPGA1[42]-1706.66)*0.366210938);
    Voltage_3 = fabs(((float)DMABufFPGA1[43]-2070.0)*1.271565755);
    Temp3_1 = CalculateNTC(DMABufFPGA1[44]);
    Temp3_2 = CalculateNTC(DMABufFPGA1[45]);
    Temp3_3 = CalculateNTC(DMABufFPGA1[46]);

    Gen_TempPh1 = (((float)DMABufFPGA1[47]-2048.0)*-0.414413452) + 335.92;
    Gen_TempPh2 = (((float)DMABufFPGA1[16]-2048.0)*-0.414413452) + 335.92;
    Gen_TempPh3 = (((float)DMABufFPGA1[17]-2048.0)*-0.414413452) + 335.92;
    Gen_TempExc1 = (((float)DMABufFPGA1[48]-2048.0)*-0.414413452) + 335.92;
    Gen_TempExc2 = (((float)DMABufFPGA1[49]-2048.0)*-0.414413452) + 335.92;
    Gen_TempRearBearing = (((float)DMABufFPGA1[6]-2048.0)*-0.414413452) + 335.92;

	// Gathering time shift since last ADC interrupt for phase syncronization----------------
		//if (SyncLaunch == 1) {if (GlobalError==0) {ActivateInverter(1);SyncLaunch=0;ActivateSynchronizer(0);}} //Sync launch inverter with predefined volt\phase
		CalculationsTimerValue = CpuTimer1Regs.TIM.all;
		CalculationsDelay = (Int32)LastMeasuredTimerValue - (Int32)CalculationsTimerValue;
		if (CalculationsDelay<0) {CalculationsDelay = MAIN_CYCLE_US + CalculationsDelay;}

		if (CalculationsDelay>(ADC_SAMPLE_CYCLE_SHIFT+5)) {CpuTimer2Regs.TPR.bit.TDDR = C28_FREQ;}
		else if (CalculationsDelay<(ADC_SAMPLE_CYCLE_SHIFT-5)) {CpuTimer2Regs.TPR.bit.TDDR = C28_FREQ-2;}
		else {CpuTimer2Regs.TPR.bit.TDDR = C28_FREQ-1;}
	// ------------------------------------------------------------------------

	PhaseAVoltage_Array[MainCycleMeasurmentsCounter] = 0;
	PhaseBVoltage_Array[MainCycleMeasurmentsCounter] = 0;
	PhaseCVoltage_Array[MainCycleMeasurmentsCounter] = 0;

	UDCFilter->Input(DCVoltage_OUT);
	UDCOUTFiltered = UDCFilter->CurrentOutput;
	UDCFilterSlow->Input(UDCOUTFiltered);
	UDCOUTFilteredSlow = UDCFilterSlow->CurrentOutput;

	DCPower = 0;

	ExcCurrentFilter->Input(ExcitationCurrent);
	ExcitationCurrentFiltered = ExcCurrentFilter->CurrentOutput;

	// ----------------------------------------------------------------------------------------------------
}


void CalculatePower(void)
{
	//Расчет мощностей -------------------------------------------------------------------------------------
	/*if (OutputFrequency>MinOutputFrequency) {IndexShiftCount = (Uint16)(FrequencyMultiplier/OutputFrequency);}
	else {IndexShiftCount = MEASURMENTS_BUFFER_SIZE-1;}
	ShiftedVoltageIndex = (MEASURMENTS_BUFFER_SIZE + MainCycleMeasurmentsCounter - IndexShiftCount);
	if (ShiftedVoltageIndex>=MEASURMENTS_BUFFER_SIZE) {ShiftedVoltageIndex = ShiftedVoltageIndex - MEASURMENTS_BUFFER_SIZE;}

	InstActivePower_A = PhaseAVoltage*PhaseACurrent;
	InstActivePower_B = PhaseBVoltage*PhaseBCurrent;
	InstActivePower_C = PhaseCVoltage*PhaseCCurrent;
	InstActivePower = InstActivePower_A + InstActivePower_B + InstActivePower_C;
	APLowpassFilter->Input(InstActivePower);
	APBandstopFilter->Input(APLowpassFilter->CurrentOutput);
	APBandstopFilter2->Input(APBandstopFilter->CurrentOutput);
	APLowpassFilter2->Input(APBandstopFilter2->CurrentOutput);
	ActivePower = APLowpassFilter2->CurrentOutput;

	InstReactivePower_A = PhaseAVoltage_Array[ShiftedVoltageIndex]*PhaseACurrent;
	InstReactivePower_B = PhaseBVoltage_Array[ShiftedVoltageIndex]*PhaseBCurrent;
	InstReactivePower_C = PhaseCVoltage_Array[ShiftedVoltageIndex]*PhaseCCurrent;
	InstReactivePower = InstReactivePower_A + InstReactivePower_B + InstReactivePower_C;
	RPLowpassFilter->Input(InstReactivePower);
	RPBandstopFilter->Input(RPLowpassFilter->CurrentOutput);
	RPBandstopFilter2->Input(RPBandstopFilter->CurrentOutput);
	RPLowpassFilter2->Input(RPBandstopFilter2->CurrentOutput);
	ReactivePower = RPLowpassFilter2->CurrentOutput;*/
	// ----------------------------------------------------------------------------------------------------
}

void RotationSpeedMeasurments(void)
{
	// Детектор частоты вращения
	DetectionLevel = (Uint16)((GenPhaseRMSLowpassFilter->CurrentOutput - 2048.0)*0.6);
	if (DetectionLevel<10) {DetectionLevel=10;} else if (DetectionLevel>140) {DetectionLevel = 140;}
	DetectionLevelHigh = 2048 + DetectionLevel; DetectionLevelLow = 2048 - DetectionLevel;

	if (RotSpeedMeasReady==1)
	{
		if (TransitLengthCounts>=120)
		{
			MeasuredRotFreq = Const_RotFreqMultiplier/((float)TransitLengthCounts);
			RotSpeedMeasReset = 1;
		}
		else if (TotalTransitCount>=40 && MeasurmentLengthCounts>1)
		{
			MeasuredRotFreq = Const_RotFreqMultiplier / (((float)MeasurmentLengthCounts)/((float)TotalTransitCount));
			RotSpeedMeasReset = 1;
		}
		RotSpeedMeasReady=0;
	}

	RotationFrequency = 0; //MeasuredRotFreq
	if (RotationFrequency<0) {RotationFrequency = 0;}
	if (RotationFrequency>5000) {RotationFrequency = 5000;}
	GenSpeedLowpassFilter->Input(RotationFrequency);
	GenSpeedFiltered = GenSpeedLowpassFilter->CurrentOutput;
	// ----------------------------------------------------------------------------------------------------
}
