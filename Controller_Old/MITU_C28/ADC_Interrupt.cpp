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

	if (TimerTicksDivideCounter == ADC_SAMPLE_DATA_DIVIDER)
	{
		ADC_Current_Value1 = DMABufFPGA1[18]; //Chopp
		ADC_Current_Value2 = DMABufFPGA1[19]; //Exc1
		ADC_Current_Value3 = DMABufFPGA1[20]; //Exc2
		ADC_Current_Value4 = DMABufFPGA1[7]; //ExcVin
		ADC_Current_Value5 = DMABufFPGA1[8]; //ExcVout
		ADC_Current_Value6 = DMABufFPGA1[24]; //INV1 Current1
		ADC_Current_Value7 = DMABufFPGA1[25]; //INV1 Current2
		ADC_Current_Value8 = DMABufFPGA1[26]; //INV1 Current3
		ADC_Current_Value9 = DMABufFPGA1[27]; //INV1 Voltage
		ADC_Current_Value10 =  DMABufFPGA1[32]; //INV2 Current1
		ADC_Current_Value11 =  DMABufFPGA1[33]; //INV2 Current2
		ADC_Current_Value12 =  DMABufFPGA1[34]; //INV2 Current3
		ADC_Current_Value13 =  DMABufFPGA1[35]; //INV2 Voltage
		ADC_Current_Value14 = DMABufFPGA1[40]; //INV3 Current1
		ADC_Current_Value15 = DMABufFPGA1[41]; //INV3 Current2
		ADC_Current_Value16 = DMABufFPGA1[42]; //INV3 Current3
		ADC_Current_Value17 = DMABufFPGA1[43]; //INV3 Voltage
		ADC_Current_Value18 = DMABufFPGA1[5]; //PhU1PWM
		ADC_Current_Value19 = DMABufFPGA1[12]; //Phase_H
		ADC_Current_Value20 = DMABufFPGA1[13]; //Phase_L

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
	/*AIN1_Values  = ADC_Current_Value1+ChannelOffsets[0];  if (ADC_Current_Value1>ADCPeakProt_Values_Max[ADCPeakProt_Counter] || ADC_Current_Value1 < ADCPeakProt_Values_Min[ADCPeakProt_Counter]) {ADCPeakProt_Now = ADCPeakProt_Now|ADCPeakProt_Flag;} ADCPeakProt_Flag=ADCPeakProt_Flag<<1; ADCPeakProt_Counter++;
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
    */
	ADCPeakProt_ERR =	0;//ADCPeakProt_Now & ADCPeakProt_ERR_Mask;
	if (ADCPeakProt_ERR!=0)	{ErrorSet(96);} //Set C28 error to FPGA
	TimerTicksDivideCounter=0;
	}

	TimerTicksDivideCounter++;

    CpuTimer2Regs.TCR.bit.TIF = 1; //Reset ADC Timer interrupt flag
    Adc1Regs.ADCINTFLGCLR.bit.ADCINT2 = 1;  //Clear ADCINT2 flag reinitialize for next SOC
    Adc2Regs.ADCINTFLGCLR.bit.ADCINT1 = 1;  //Clear ADCINT1 flag reinitialize for next SOC
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge interrupt to PIE
    return;
}



