/*
 * DataExchange.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>


void SignalProcessing(void)
{
Uint16 i;
float CalculatedResult = 0;

if (RMSCyclesCount >= Const_RMSCalcCount)
	{
		for (i=0;i<20;i++)
			{
			MeanValues_SummResults[i] = MeanValues_Buffers[i];
			MeanValues_Buffers[i]=0;
			}
		RMSCyclesCount=0;
	}

MeanValues_Buffers[0] = MeanValues_Buffers[0] + ChopperCurrent;
MeanValues_Buffers[1] = MeanValues_Buffers[1] + ExcitationCurrent;
MeanValues_Buffers[2] = MeanValues_Buffers[2] + ExcitationCurrent2;
MeanValues_Buffers[3] = MeanValues_Buffers[3] + DCVoltage_IN;
MeanValues_Buffers[4] = MeanValues_Buffers[4] + DCVoltage_OUT;
MeanValues_Buffers[5] = MeanValues_Buffers[5] + fabs(Current_U1);
MeanValues_Buffers[6] = MeanValues_Buffers[6] + fabs(Current_V1);
MeanValues_Buffers[7] = MeanValues_Buffers[7] + fabs(Current_W1);
MeanValues_Buffers[8] = MeanValues_Buffers[8] + Voltage_1;
MeanValues_Buffers[9] = MeanValues_Buffers[9] + fabs(Current_U2);
MeanValues_Buffers[10] = MeanValues_Buffers[10] + fabs(Current_V2);
MeanValues_Buffers[11] = MeanValues_Buffers[11] + fabs(Current_W2);
MeanValues_Buffers[12] = MeanValues_Buffers[12] + Voltage_2;
MeanValues_Buffers[13] = MeanValues_Buffers[13] + fabs(Current_U3);
MeanValues_Buffers[14] = MeanValues_Buffers[14] + fabs(Current_V3);
MeanValues_Buffers[15] = MeanValues_Buffers[15] + fabs(Current_W3);
MeanValues_Buffers[16] = MeanValues_Buffers[16] + Voltage_3;
MeanValues_Buffers[17] = MeanValues_Buffers[17] + Temp1_1;
MeanValues_Buffers[18] = MeanValues_Buffers[18] + Temp2_1;
MeanValues_Buffers[19] = MeanValues_Buffers[19] + Temp3_1;

RMSCyclesCount++;

if (RMS_Count_ValueNum>=20) {RMS_Count_ValueNum=0;}


CalculatedResult = MeanValues_SummResults[RMS_Count_ValueNum]/(float)Const_RMSCalcCount;
if (false)
			{
			float SQRTResult = sqrt(CalculatedResult);
			MeanValues[RMS_Count_ValueNum] = (short)(SQRTResult);
			if (RMS_Count_ValueNum == 8)
				{
				GenPhaseRMS = SQRTResult;
				}
			}
else
			{
			MeanValues[RMS_Count_ValueNum] = (short)CalculatedResult;
			}

//if (MeanValues_Buffers[3]>1000) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}
//if (MeanValues_Buffers[4]>1000) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}
//if (MeanValues_Buffers[8]>1000) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}
//if (MeanValues_Buffers[12]>1000) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}
//if (MeanValues_Buffers[16]>1000) {ErrorSet(ERR_DCOUT_OVERVOLTAGE);}

/*
switch (RMS_Count_ValueNum) //Защиты по RMS и средним значениям
	{
	case 0:  PhAVoltRMS = MeanValues[0]; if (PhAVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHA_OVERVOLTAGE);} break;
	case 1:  PhBVoltRMS = MeanValues[1]; if (PhBVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHB_OVERVOLTAGE);} break;
	case 2:  PhCVoltRMS = MeanValues[2]; if (PhCVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHC_OVERVOLTAGE);} break;

	case 3: Rect1MeanCurrent = fabs(MeanValues[3]);	if (Rect1MeanCurrent>ProtRMS_RectCurrent) {ErrorSet(ERR_MEANRECT1_OVERCURRENT);} break;
	case 4: Rect2MeanCurrent = fabs(MeanValues[4]);	if (Rect2MeanCurrent>ProtRMS_RectCurrent) {ErrorSet(ERR_MEANRECT2_OVERCURRENT);} break;
	case 5: Rect3MeanCurrent = fabs(MeanValues[5]);	if (Rect3MeanCurrent>ProtRMS_RectCurrent) {ErrorSet(ERR_MEANRECT3_OVERCURRENT);} break;

	case 6: UDCMeanVoltage = fabs(MeanValues[6]);	if (UDCMeanVoltage>ProtRMS_UDCVoltage ) {ErrorSet(ERR_MEANUDCOUT_OVERVOLTAGE);} break;

	case 7: NeutralCurrentRMS = MeanValues[7];	 if (NeutralCurrentRMS>ProtRMS_NeutralCurrent) {ErrorSet(ERR_RMSIGEN_OVERCURRENT);} break;

	case 8: NeutralIGBTCurrentRMS = MeanValues[8]; if (NeutralIGBTCurrentRMS>ProtRMS_NeutralIGBCurrent) {ErrorSet(ERR_RMSUGEN_OVERVOLTAGE);} break;

	case 9: IExcMeanCurrent = fabs(MeanValues[9]);if (IExcMeanCurrent>ProtRMS_ExcCurrent) {ErrorSet(ERR_MEANIEXC_OVERCURRENT);} break;

	case 10: SKiiPC_MeanTemp = fabs(MeanValues[10]);	if (SKiiPC_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHC_OVERTEMP);} break;
	case 11: SKiiPB_MeanTemp = fabs(MeanValues[11]);	if (SKiiPB_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHB_OVERTEMP);} break;
	case 12: SKiiPA_MeanTemp = fabs(MeanValues[12]);	if (SKiiPA_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHA_OVERTEMP);} break;

	case 14: PhCIGBTCurrentRMS = MeanValues[14];	if (PhCIGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHC_OVERCURRENT);}  break;
	case 15: PhCCurrentRMS = MeanValues[15]; if (PhCCurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHC_OVERCURRENT);}  break;
	case 16: PhBIGBTCurrentRMS = MeanValues[16]; if (PhBIGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHB_OVERCURRENT);}  break;
	case 17: PhBCurrentRMS = MeanValues[17]; if (PhBCurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHB_OVERCURRENT);}  break;
	case 18: PhAIGBTCurrentRMS = MeanValues[18]; if (PhAIGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHA_OVERCURRENT);}  break;
	case 19: PhACurrentRMS = MeanValues[19]; if (PhACurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHA_OVERCURRENT);}  break;
	default: break;
	}*/

RMS_Count_ValueNum++;

GenPhaseRMSLowpassFilter->Input(GenPhaseRMS);
}



void MeanOffsetCalc(void)
{

	ChannelOffsetCalc[0] = ChannelOffsetCalc[0] + AIN1_Values;
	ChannelOffsetCalc[1] = ChannelOffsetCalc[1] + AIN2_Values;
	ChannelOffsetCalc[2] = ChannelOffsetCalc[2] + AIN3_Values;
	ChannelOffsetCalc[3] = ChannelOffsetCalc[3] + AIN4_Values;
	ChannelOffsetCalc[4] = ChannelOffsetCalc[4] + AIN5_Values;
	ChannelOffsetCalc[5] = ChannelOffsetCalc[5] + AIN6_Values;
	ChannelOffsetCalc[6] = ChannelOffsetCalc[6] + AIN7_Values;
	ChannelOffsetCalc[7] = ChannelOffsetCalc[7] + AIN8_Values;
	ChannelOffsetCalc[8] = ChannelOffsetCalc[8] + AIN9_Values;
	ChannelOffsetCalc[9] = ChannelOffsetCalc[9] + AIN10_Values;
	ChannelOffsetCalc[10] = ChannelOffsetCalc[10] + AIN11_Values;
	ChannelOffsetCalc[11] = ChannelOffsetCalc[11] + AIN12_Values;
	ChannelOffsetCalc[12] = ChannelOffsetCalc[12] + AIN13_Values;
	ChannelOffsetCalc[13] = ChannelOffsetCalc[13] + AIN14_Values;
	ChannelOffsetCalc[14] = ChannelOffsetCalc[14] + AIN15_Values;
	ChannelOffsetCalc[15] = ChannelOffsetCalc[15] + AIN16_Values;
	ChannelOffsetCalc[16] = ChannelOffsetCalc[16] + AIN17_Values;
	ChannelOffsetCalc[17] = ChannelOffsetCalc[17] + AIN18_Values;
	ChannelOffsetCalc[18] = ChannelOffsetCalc[18] + AIN19_Values;
	ChannelOffsetCalc[19] = ChannelOffsetCalc[19] + AIN20_Values;

	if (RecalcOffset>5001)
		{
		for (int ChNum=0; ChNum<20; ChNum++)
			{
				ChannelOffsetCalc[ChNum] = ChannelOffsetCalc[ChNum]/5000;
				ChannelOffsetCalc[ChNum] = 2048 - ChannelOffsetCalc[ChNum];
				if (ChannelOffsetCalc[ChNum]>50) ChannelOffsetCalc[ChNum]=50;
				if (ChannelOffsetCalc[ChNum]<-50) ChannelOffsetCalc[ChNum]=-50;
				ChannelOffsets[ChNum] = ChannelOffsetCalc[ChNum];
			}
			ChannelOffsets[10]=0;ChannelOffsets[11]=0;ChannelOffsets[12]=0;
			RecalcOffset=0;
			SaveOffsetsToFlash();
		}
}
