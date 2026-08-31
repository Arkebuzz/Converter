/*
 * ADC_Interrupt.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */
#include <GlobalData.h>
volatile Uint16 PHACurrent;
volatile Uint16 PHBCurrent;
volatile Uint16 PHCCurrent;

interrupt void  adc1_isr(void)
{

	//ADC_Current_Value15 = Adc1Result.ADCRESULT1; //SKiiP PhC current
	//ADC_Current_Value17 = Adc1Result.ADCRESULT6; //SKiiP PhB current
	//ADC_Current_Value19 = Adc1Result.ADCRESULT8; //SKiiP PhA current
	//PHACurrent = ADC_Current_Value19+ChannelOffsets[18]; //SKiiP PhA current
	//PHBCurrent = ADC_Current_Value17+ChannelOffsets[16]; //SKiiP PhB current
	//PHCCurrent = ADC_Current_Value15+ChannelOffsets[14]; //SKiiP PhC current


    ADC_Current_Value15 = 2048; //SKiiP PhC current
    ADC_Current_Value17 = 2048; //SKiiP PhB current
    ADC_Current_Value19 = 2048; //SKiiP PhA current
    PHACurrent = 0;//ADC_Current_Value19+ChannelOffsets[18]; //SKiiP PhA current
    PHBCurrent = 0;//ADC_Current_Value17+ChannelOffsets[16]; //SKiiP PhB current
    PHCCurrent = 0;//ADC_Current_Value15+ChannelOffsets[14]; //SKiiP PhC current
	WriteWordTo_FPGA(70,0); //SKiiP PhA current
	WriteWordTo_FPGA(71,0); //SKiiP PhB current
	WriteWordTo_FPGA(72,0); //SKiiP PhC current
	WriteWordTo_FPGA(74,(unsigned short)(CommitCurrentsValue)); //Data transfer commit


	if (TimerTicksDivideCounter == ADC_SAMPLE_DATA_DIVIDER)
	{
		/*ADC_Current_Value1 = Adc2Result.ADCRESULT9;
		ADC_Current_Value2 = Adc2Result.ADCRESULT8;
		ADC_Current_Value3 = Adc2Result.ADCRESULT7;
		ADC_Current_Value4 = Adc2Result.ADCRESULT6;
		ADC_Current_Value5 = Adc2Result.ADCRESULT0;
		ADC_Current_Value6 = Adc2Result.ADCRESULT1;
		ADC_Current_Value7 = Adc2Result.ADCRESULT2;
		ADC_Current_Value8 = Adc2Result.ADCRESULT3;
		ADC_Current_Value9 = Adc2Result.ADCRESULT4;
		ADC_Current_Value10 = Adc2Result.ADCRESULT5;
		ADC_Current_Value11 = Adc1Result.ADCRESULT5;
		ADC_Current_Value12 = Adc1Result.ADCRESULT4;
		ADC_Current_Value13 = Adc1Result.ADCRESULT3;
		ADC_Current_Value14 = Adc1Result.ADCRESULT2;
		ADC_Current_Value16 = Adc1Result.ADCRESULT0;
		ADC_Current_Value18 = Adc1Result.ADCRESULT7;
		ADC_Current_Value20 = Adc1Result.ADCRESULT9;*/

        ADC_Current_Value1 = 2048;
        ADC_Current_Value2 =  2048;
        ADC_Current_Value3 =  2048;
        ADC_Current_Value4 =  2048;
        ADC_Current_Value5 =  2048;
        ADC_Current_Value6 =  2048;
        ADC_Current_Value7 =  2048;
        ADC_Current_Value8 =  2048;
        ADC_Current_Value9 =  2048;
        ADC_Current_Value10 =  2048;
        ADC_Current_Value11 =  2048;
        ADC_Current_Value12 =  2048;
        ADC_Current_Value13 =  2048;
        ADC_Current_Value14 =  2048;
        ADC_Current_Value16 =  2048;
        ADC_Current_Value18 =  2048;
        ADC_Current_Value20 =  2048;

	ADCPeakProt_Flag = 1;
	ADCPeakProt_Now = 0;
	ADCPeakProt_Counter = 0;
	TimerTicksCount++;
	LastMeasuredTimerValue = CpuTimer1Regs.TIM.all;

	 // Circular buffer counter reload
	    if(MeasurmentsCounter == (MEASURMENTS_BUFFER_SIZE - 1))
	    {
	    	MeasurmentsCounter = 0;
	    }
	    else MeasurmentsCounter++;

	MeasurmentsTime = TimerTicksCount;

	AIN1_Values  = ADC_Current_Value1+ChannelOffsets[0];  if (ADC_Current_Value1>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value1 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN2_Values  = ADC_Current_Value2+ChannelOffsets[1];  if (ADC_Current_Value2>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value2 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN3_Values  = ADC_Current_Value3+ChannelOffsets[2];  if (ADC_Current_Value3>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value3 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN4_Values  = ADC_Current_Value4+ChannelOffsets[3];  if (ADC_Current_Value4>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value4 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN5_Values  = ADC_Current_Value5+ChannelOffsets[4];  if (ADC_Current_Value5>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value5 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN6_Values  = ADC_Current_Value6+ChannelOffsets[5];  if (ADC_Current_Value6>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value6 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN7_Values  = ADC_Current_Value7+ChannelOffsets[6];  if (ADC_Current_Value7>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value7 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN8_Values  = ADC_Current_Value8+ChannelOffsets[7];  if (ADC_Current_Value8>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value8 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN9_Values  = ADC_Current_Value9+ChannelOffsets[8];  if (ADC_Current_Value9>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value9 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN10_Values  = ADC_Current_Value10+ChannelOffsets[9]; if (ADC_Current_Value10>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value10 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;

	AIN11_Values  = ADC_Current_Value11+ChannelOffsets[10]; if (ADC_Current_Value11>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value11 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN12_Values  = ADC_Current_Value12+ChannelOffsets[11]; if (ADC_Current_Value12>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value12 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN13_Values  = ADC_Current_Value13+ChannelOffsets[12]; if (ADC_Current_Value13>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value13 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN14_Values  = ADC_Current_Value14+ChannelOffsets[13]; if (ADC_Current_Value14>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value14 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN15_Values  = ADC_Current_Value15+ChannelOffsets[14]; if (ADC_Current_Value15>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value15 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN16_Values  = ADC_Current_Value16+ChannelOffsets[15]; if (ADC_Current_Value16>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value16 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN17_Values  = ADC_Current_Value17+ChannelOffsets[16]; if (ADC_Current_Value17>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value17 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN18_Values  = ADC_Current_Value18+ChannelOffsets[17]; if (ADC_Current_Value18>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value18 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN19_Values  = ADC_Current_Value19+ChannelOffsets[18]; if (ADC_Current_Value19>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value19 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
	AIN20_Values  = ADC_Current_Value20+ChannelOffsets[19]; if (ADC_Current_Value20>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value20 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;


	/*AIN1_Values = 0;
	AIN2_Values = 0;
	AIN3_Values = 0;
	AIN4_Values = 0;
	AIN5_Values = 0;
	AIN6_Values = 0;
	AIN7_Values = 0;
	AIN8_Values = 0;
	AIN9_Values = 0;
	AIN10_Values = 0;
	AIN11_Values = 0;
	AIN12_Values = 0;
	AIN13_Values = 0;
	AIN14_Values = 0;
	AIN15_Values = 0;
	AIN16_Values = 0;
	AIN17_Values = 0;
	AIN18_Values = 0;
	AIN19_Values = 0;
	AIN20_Values = 0;*/



	ADCPeakProt_Now = 0;

	ADCPeakProt_ERR =	0;
	//if (ADCPeakProt_ERR!=0)	{ErrorSet(96);} //Set C28 error to FPGA
	TimerTicksDivideCounter=0;


	//-------------START GEN SPEED DETECTOR-------------------------------------
		/*TickCounts++;
		TransitTickCounts++;
		SignChangeTickCounts++;
		GenVoltage_Unscaled = AIN9_Values ;
		if (TickCounts>10000)
		{MeasuredRotFreq = 0; RotSpeedMeasReset = 1; TickCounts=0; TransitTickCounts=0; SignChangeTickCounts=0; TransitCounts=0;}

		if ( (GenVoltage_Unscaled>(DetectionLevelHigh)) && (GenVoltageLow==1) && (SignChangeTickCounts>1))
		{
			TransitCounts++;
			if (RotSpeedMeasReset==1)
			{RotSpeedMeasReset=0; TickCounts=0;TransitCounts=0;}
			if (RotSpeedMeasReady==0)
				{
				TotalTransitCount = TransitCounts;
				TransitLengthCounts = TransitTickCounts;
				MeasurmentLengthCounts = TickCounts;
				RotSpeedMeasReady=1;
				}
			GenVoltageLow=0;
			GenVoltageHigh=1;
			TransitTickCounts=0;
			SignChangeTickCounts=0;
		}
		else if ( (GenVoltage_Unscaled < (DetectionLevelLow)) && (GenVoltageHigh==1) && (SignChangeTickCounts>1))
		{
			GenVoltageLow=1;
			GenVoltageHigh=0;
			SignChangeTickCounts=0;
		} */
	//-------------END GEN SPEED DETECTOR-------------------------------------

	}

	TimerTicksDivideCounter++;

    CpuTimer2Regs.TCR.bit.TIF = 1; //Reset ADC Timer interrupt flag
    Adc1Regs.ADCINTFLGCLR.bit.ADCINT2 = 1;  //Clear ADCINT2 flag reinitialize for next SOC
    Adc2Regs.ADCINTFLGCLR.bit.ADCINT1 = 1;  //Clear ADCINT1 flag reinitialize for next SOC
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge interrupt to PIE
    return;
}



