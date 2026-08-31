/*
 * ALG_MainCycleFunctions.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>

void CalculateValues(void)
{
    //SEG DATA-------------
    SEG_EXC_UIN  =((float)BoosterInput1-2048.0)*0.729830759876179;
    SEG_U_MAIN   =((float)BoosterInput2-2048.0)*0.729830759876179;
    SEG_U_R  =((float)BoosterInput3-2048.0)*0.729830759876179;
    SEG_I_IN     =((float)BoosterInput4-2048.0)*0.0243276919958726;
    SEG_IEXC1    =((float)BoosterInput5-2048.0)*0.0243276919958726;
    SEG_IEXC2    =((float)BoosterInput6-2048.0)*0.0243276919958726;
    SEG_EXC_TEMP     = -1.3289*((float)(BoosterInput8&0x00FF))+251.34;

    SEG_RECT_UG1     =0;
    SEG_RECT_UG2     =0;
    SEG_RECT_UDC     =((float)CellsInput[0]-2048.0)*0.729830759876179;
    SEG_RECT_UDCP    =SEG_RECT_UDC/2;
    SEG_RECT_UDCN    =SEG_RECT_UDC/2;
    SEG_RECT_RI1     =((float)CellsInput[1]-2048.0)*0.243276919958726;
    SEG_RECT_RI2     =((float)CellsInput[2]-2048.0)*0.243276919958726;
    SEG_RECT_RI3     =((float)CellsInput[3]-2048.0)*0.243276919958726;
    SEG_RECT_RI4     =((float)CellsInput[4]-2048.0)*0.243276919958726;
    SEG_RECT_RI5     =((float)CellsInput[5]-2048.0)*0.243276919958726;
    SEG_RECT_RI6     =((float)CellsInput[6]-2048.0)*0.243276919958726;
    SEG_RECT_TU1     =-1.3289*((float)((CellsInput[7]>>8)&0x00FF))+251.34;
    SEG_RECT_TU2     =-1.3289*((float)(CellsInput[7]&0x00FF))+251.34;
    SEG_RECT_TC1     =-1.3289*((float)((CellsInput[8]>>8)&0x00FF))+251.34;

    SEG_I1_IPH1  =((float)CellsInput[12]-2048.0)*1.52048074974204;
    SEG_I1_IPH2  =((float)CellsInput[13]-2048.0)*1.52048074974204;
    SEG_I1_IPH3  =((float)CellsInput[14]-2048.0)*1.52048074974204;
    SEG_I1_SKIPH1    =((float)CellsInput[15]-2048.0)*2.19548726275327;
    SEG_I1_SKIPH2    =((float)CellsInput[16]-2048.0)*2.19548726275327;
    SEG_I1_SKIPH3    =((float)CellsInput[17]-2048.0)*2.19548726275327;
    SEG_I1_SKIDISCH  =0;
    SEG_I1_UDC   = ((float)CellsInput[18])*0.435965401785714;
    SEG_I1_TSK1  = 30.0+0.5859375*((float)(CellsInput[21]&0x00FF));
    SEG_I1_TSK2  = 30.0+0.5859375*((float)((CellsInput[22]>>8)&0x00FF));
    SEG_I1_TSK3  = 30.0+0.5859375*((float)(CellsInput[22]&0x00FF));
    SEG_I1_TSKDISCH  = 30.0;
    SEG_I1_TLF   = -1.3289*((float)((CellsInput[19]>>8)&0x00FF))+251.34;
    SEG_I1_TC1   = -1.3289*((float)(CellsInput[19]&0x00FF))+251.34;
    SEG_I1_TC2   = SEG_I1_TC1;

    SEG_I2_IPH1  =((float)CellsInput[24]-2048.0)*1.52048074974204;
    SEG_I2_IPH2  =((float)CellsInput[25]-2048.0)*1.52048074974204;
    SEG_I2_IPH3  =((float)CellsInput[26]-2048.0)*1.52048074974204;
    SEG_I2_SKIPH1    =((float)CellsInput[27]-2048.0)*2.19548726275327;
    SEG_I2_SKIPH2    =((float)CellsInput[28]-2048.0)*2.19548726275327;
    SEG_I2_SKIPH3    =((float)CellsInput[29]-2048.0)*2.19548726275327;
    SEG_I2_SKIDISCH  = 0;
    SEG_I2_UDC   =((float)CellsInput[30])*0.435965401785714;
    SEG_I2_TSK1  = 30.0+0.5859375*((float)(CellsInput[33]&0x00FF));
    SEG_I2_TSK2  = 30.0+0.5859375*((float)((CellsInput[34]>>8)&0x00FF));
    SEG_I2_TSK3  = 30.0+0.5859375*((float)(CellsInput[34]&0x00FF));
    SEG_I2_TSKDISCH  = 30.0;
    SEG_I2_TLF   =-1.3289*((float)((CellsInput[31]>>8)&0x00FF))+251.34;
    SEG_I2_TC1   =-1.3289*((float)(CellsInput[31]&0x00FF))+251.34;
    SEG_I2_TC2   =SEG_I2_TC1;

    SEG_I3_IPH1  =((float)CellsInput[36]-2048.0)*1.52048074974204;
    SEG_I3_IPH2  =((float)CellsInput[37]-2048.0)*1.52048074974204;
    SEG_I3_IPH3  =((float)CellsInput[38]-2048.0)*1.52048074974204;
    SEG_I3_SKIPH1    =((float)CellsInput[39]-2048.0)*2.19548726275327;
    SEG_I3_SKIPH2    =((float)CellsInput[40]-2048.0)*2.19548726275327;
    SEG_I3_SKIPH3    =((float)CellsInput[41]-2048.0)*2.19548726275327;
    SEG_I3_SKIDISCH  = 0;
    SEG_I3_UDC   =((float)CellsInput[42])*0.435965401785714;
    SEG_I3_TSK1  = 30.0+0.5859375*((float)(CellsInput[45]&0x00FF));
    SEG_I3_TSK2  = 30.0+0.5859375*((float)((CellsInput[46]>>8)&0x00FF));
    SEG_I3_TSK3  = 30.0+0.5859375*((float)(CellsInput[46]&0x00FF));
    SEG_I3_TSKDISCH  = 30.0;
    SEG_I3_TLF   =-1.3289*((float)((CellsInput[43]>>8)&0x00FF))+251.34;
    SEG_I3_TC1   =-1.3289*((float)(CellsInput[43]&0x00FF))+251.34;
    SEG_I3_TC2   =SEG_I3_TC1;


    SEG_F_UA     =((float)CellsInput[48]-2048.0)*0.729830759876179;
    SEG_F_UB     =((float)CellsInput[49]-2048.0)*0.729830759876179;
    SEG_F_UC     =((float)CellsInput[50]-2048.0)*0.729830759876179;
    SEG_F_UN     =((float)CellsInput[51]-2048.0)*0.729830759876179;
    SEG_F_UGRID  = 0;
    SEG_F_IA     =((float)CellsInput[52]-2048.0)*3.25520833333333;
    SEG_F_IB     =((float)CellsInput[53]-2048.0)*3.25520833333333;
    SEG_F_IC     =((float)CellsInput[54]-2048.0)*3.25520833333333;
    SEG_F_TC1    =-1.3289*((float)((CellsInput[55]>>8)&0x00FF))+251.34;
    SEG_F_TC2    =-1.3289*((float)(CellsInput[55]&0x00FF))+251.34;
    SEG_F_TC3    =-1.3289*((float)((CellsInput[56]>>8)&0x00FF))+251.34;

    SEG_TEMPCELL_EXC     =-1.1978*((float)((BoosterInput8>>8)&0x00FF))+231.07;
    SEG_TEMPCELL_RECT    =-1.1978*((float)(CellsInput[8]&0x00FF))+231.07;
    SEG_TEMPCELL_INV1    = -1.1978*((float)((CellsInput[20]>>8)&0x00FF))+231.07;
    SEG_TEMPCELL_INV2    = -1.1978*((float)((CellsInput[32]>>8)&0x00FF))+231.07;
    SEG_TEMPCELL_INV3    = -1.1978*((float)((CellsInput[44]>>8)&0x00FF))+231.07;
    SEG_TEMPCELL_FILT    =-1.1978*((float)(CellsInput[56]&0x00FF))+231.07;


    SEG_DCS1     = BoosterInput10;
    SEG_DCS2     = BoosterInput11;
    SEG_DCS3     = CellsInput[9];
    SEG_DCS4     = CellsInput[10];

    //SEG DATA-------------

	// Values calculation --------------------------------------------------------------------------
	ExcitationCurrent = fabs(SEG_IEXC1);//fabs(((float)AIN10_Values-2048.0)*ChGain[9]);

	//DCVoltage_IN = fabs(((float)AIN7_Values-2048.0)*0.828810728);
	DCVoltage_OUT = fabs(SEG_RECT_UDC);//fabs(((float)AIN7_Values-2048.0)*ChGain[6]);

	// Gathering time shift since last ADC interrupt for phase syncronization----------------
		if (SyncLaunch == 1) {if (GlobalError==0) {ActivateInverter(1);SyncLaunch=0;ActivateSynchronizer(0);}} //Sync launch inverter with predefined volt\phase
		CalculationsTimerValue = CpuTimer1Regs.TIM.all;
		CalculationsDelay = (Int32)LastMeasuredTimerValue - (Int32)CalculationsTimerValue;
		if (CalculationsDelay<0) {CalculationsDelay = MAIN_CYCLE_US + CalculationsDelay;}

		//if (CalculationsDelay>(ADC_SAMPLE_CYCLE_SHIFT+5)) {CpuTimer2Regs.PRD.all = ADC_SAMPLE_CYCLE_US;}
		//else if (CalculationsDelay<(ADC_SAMPLE_CYCLE_SHIFT-5)) {CpuTimer2Regs.PRD.all = ADC_SAMPLE_CYCLE_US-2;}
		//else {CpuTimer2Regs.PRD.all = ADC_SAMPLE_CYCLE_US-1;}

		if (CalculationsDelay>(ADC_SAMPLE_CYCLE_SHIFT+5)) {CpuTimer2Regs.TPR.bit.TDDR = C28_FREQ;}
		else if (CalculationsDelay<(ADC_SAMPLE_CYCLE_SHIFT-5)) {CpuTimer2Regs.TPR.bit.TDDR = C28_FREQ-2;}
		else {CpuTimer2Regs.TPR.bit.TDDR = C28_FREQ-1;}
	// ------------------------------------------------------------------------

	PhaseAVoltage = SEG_F_UA;//((float)AIN1_Values-2048.0)*ChGain[0];
	PhaseBVoltage = SEG_F_UB;//((float)AIN2_Values-2048.0)*ChGain[1];
	PhaseCVoltage = SEG_F_UC;//((float)AIN3_Values-2048.0)*ChGain[2];

	PhaseACurrent = SEG_F_IA;//((float)AIN20_Values-2048.0)*ChGain[19] ;
	PhaseBCurrent = SEG_F_IB;//((float)AIN18_Values-2048.0)*ChGain[17] ;
	PhaseCCurrent = SEG_F_IC;//((float)AIN16_Values-2048.0)*ChGain[15] ;

	PhaseATemp = 0;//fabs(((float)AIN13_Values-2048.0)*ChGain[12] ) + 20;
	PhaseBTemp = 0;//fabs(((float)AIN12_Values-2048.0)*ChGain[11] ) + 20;
	PhaseCTemp = 0;//fabs(((float)AIN11_Values-2048.0)*ChGain[10] ) + 20;
	//PhaseChopperTemp = ((float)AIN11_Values-2048.0)*0.056282419 + 20;

	GenPhaseVoltage = 0;//((float)AIN9_Values-2048.0)*ChGain[8] ;


	Rect1Current = fabs(SEG_RECT_RI4);//fabs(((float)AIN4_Values-2048.0)*ChGain[3]);
	Rect2Current = fabs(SEG_RECT_RI5);//fabs(((float)AIN5_Values-2048.0)*ChGain[4]);
	Rect3Current = fabs(SEG_RECT_RI6);//fabs(((float)AIN6_Values-2048.0)*ChGain[5]);

	//ChopperCurrent1 = fabs(SEG_I_IN);
	//ChopperCurrent2 = fabs(((float)AIN20_Values-2048.0)*0);

	PhaseAVoltage_Array[MainCycleMeasurmentsCounter] = PhaseAVoltage;
	PhaseBVoltage_Array[MainCycleMeasurmentsCounter] = PhaseBVoltage;
	PhaseCVoltage_Array[MainCycleMeasurmentsCounter] = PhaseCVoltage;

	UDCFilter->Input(DCVoltage_OUT);
	UDCOUTFiltered = UDCFilter->CurrentOutput;
	UDCFilterSlow->Input(UDCOUTFiltered);
	UDCOUTFilteredSlow = UDCFilterSlow->CurrentOutput;

	Reg_IDC = Rect1Current+Rect2Current+Rect3Current;
	IDCLowpassFilter->Input(Reg_IDC);
	IDCFiltered = IDCLowpassFilter->CurrentOutput;
	DCPower = Booster_DCVoltage_IN * IDCFiltered;
	RMSVoltageFilter->Input((PhAVoltRMS + PhBVoltRMS + PhCVoltRMS)/3);
	OutputRMSVoltage = RMSVoltageFilter->CurrentOutput;
	OutputFrequencyFilter->Input(OutputFrequency);
	OutputFrequencyFiltered = OutputFrequencyFilter->CurrentOutput;
	ExcCurrentFilter->Input(ExcitationCurrent);
	ExcitationCurrentFiltered = ExcCurrentFilter->CurrentOutput;

	//DCPowerUint16 = (DCPower/10.0);
	// ----------------------------------------------------------------------------------------------------
}


void CalculatePower(void)
{
	//Расчет мощностей -------------------------------------------------------------------------------------
	if (OutputFrequency>MinOutputFrequency) {IndexShiftCount = (Uint16)(FrequencyMultiplier/OutputFrequency);}
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
	ReactivePower = RPLowpassFilter2->CurrentOutput;
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

	RotationFrequency = MeasuredRotFreq;
	if (RotationFrequency<0) {RotationFrequency = 0;}
	if (RotationFrequency>5000) {RotationFrequency = 5000;}
	GenSpeedLowpassFilter->Input(RotationFrequency);
	GenSpeedFiltered = GenSpeedLowpassFilter->CurrentOutput;
	// ----------------------------------------------------------------------------------------------------
}
