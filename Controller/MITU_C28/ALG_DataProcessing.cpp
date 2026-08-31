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

MeanValues_Buffers[0] = MeanValues_Buffers[0] + SEG_I1_IPH2*SEG_I1_IPH2;
MeanValues_Buffers[1] = MeanValues_Buffers[1] + SEG_I1_IPH3*SEG_I1_IPH3;
MeanValues_Buffers[2] = MeanValues_Buffers[2] + SEG_I1_IPH1*SEG_I1_IPH1;
MeanValues_Buffers[3] = MeanValues_Buffers[3] + SEG_I2_IPH2*SEG_I2_IPH2;
MeanValues_Buffers[4] = MeanValues_Buffers[4] + SEG_I2_IPH3*SEG_I2_IPH3;
MeanValues_Buffers[5] = MeanValues_Buffers[5] + SEG_I2_IPH1*SEG_I2_IPH1;
MeanValues_Buffers[6] = MeanValues_Buffers[6] + SEG_I3_IPH2*SEG_I3_IPH2;
MeanValues_Buffers[7] = MeanValues_Buffers[7] + SEG_I3_IPH3*SEG_I3_IPH3;
MeanValues_Buffers[8] = MeanValues_Buffers[8] + SEG_I3_IPH2*SEG_I3_IPH2;
MeanValues_Buffers[9] = MeanValues_Buffers[9] + SEG_F_UA*SEG_F_UA;
MeanValues_Buffers[10] = MeanValues_Buffers[10] + SEG_F_UB*SEG_F_UB;
MeanValues_Buffers[11] = MeanValues_Buffers[11] + SEG_F_UC*SEG_F_UC;
MeanValues_Buffers[12] = MeanValues_Buffers[12] + SEG_F_IA*SEG_F_IA;
MeanValues_Buffers[13] = MeanValues_Buffers[13] + SEG_F_IB*SEG_F_IB;
MeanValues_Buffers[14] = MeanValues_Buffers[14] + SEG_F_IC*SEG_F_IC;
MeanValues_Buffers[15] = MeanValues_Buffers[15] + SEG_I2_UDC*SEG_I2_UDC;
MeanValues_Buffers[16] = MeanValues_Buffers[16] + SEG_IEXC1*SEG_IEXC1;
MeanValues_Buffers[17] = MeanValues_Buffers[17] + SEG_RECT_TU1*SEG_RECT_TU1;
MeanValues_Buffers[18] = MeanValues_Buffers[18] + SEG_RECT_TU2*SEG_RECT_TU2;
MeanValues_Buffers[19] = MeanValues_Buffers[19] + SEG_EXC_TEMP*SEG_EXC_TEMP;

RMSCyclesCount++;

if (RMS_Count_ValueNum>=20) {RMS_Count_ValueNum=0;}

CalculatedResult = MeanValues_SummResults[RMS_Count_ValueNum]/(float)Const_RMSCalcCount;
float SQRTResult = sqrt(CalculatedResult);
MeanValues[RMS_Count_ValueNum] = (short)(SQRTResult);
GenPhaseRMS = 0;


switch (RMS_Count_ValueNum) //Защиты по RMS и средним значениям
	{
    case 0: PhA1IGBTCurrentRMS = MeanValues[0]; if (PhA1IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHA_OVERCURRENT);}  break;
    case 1: PhB1IGBTCurrentRMS = MeanValues[1]; if (PhB1IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHB_OVERCURRENT);}  break;
    case 2: PhC1IGBTCurrentRMS = MeanValues[2]; if (PhC1IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHC_OVERCURRENT);}  break;
    case 3: PhA2IGBTCurrentRMS = MeanValues[3]; if (PhA2IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHA_OVERCURRENT);}  break;
    case 4: PhB2IGBTCurrentRMS = MeanValues[4]; if (PhB2IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHB_OVERCURRENT);}  break;
    case 5: PhC2IGBTCurrentRMS = MeanValues[5]; if (PhC2IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHC_OVERCURRENT);}  break;
    case 6: PhA3IGBTCurrentRMS = MeanValues[6]; if (PhA3IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHA_OVERCURRENT);}  break;
    case 7: PhB3IGBTCurrentRMS = MeanValues[7]; if (PhB3IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHB_OVERCURRENT);}  break;
    case 8: PhC3IGBTCurrentRMS = MeanValues[8]; if (PhC3IGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHC_OVERCURRENT);}  break;
    case 9:  PhAVoltRMS = MeanValues[9]; if (PhAVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHA_OVERVOLTAGE);} break;
    case 10:  PhBVoltRMS = MeanValues[10]; if (PhBVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHB_OVERVOLTAGE);} break;
    case 11:  PhCVoltRMS = MeanValues[11]; if (PhCVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHC_OVERVOLTAGE);} break;
    case 12: PhACurrentRMS = MeanValues[12]; if (PhACurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHA_OVERCURRENT);}  break;
    case 13: PhBCurrentRMS = MeanValues[13]; if (PhBCurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHB_OVERCURRENT);}  break;
    case 14: PhCCurrentRMS = MeanValues[14]; if (PhCCurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHC_OVERCURRENT);}  break;
    case 15: UDCMeanVoltage = MeanValues[15]; if (UDCMeanVoltage>ProtRMS_UDCVoltage ) {ErrorSet(ERR_MEANUDCOUT_OVERVOLTAGE);} break;
    case 16: IExcMeanCurrent = MeanValues[16]; if (IExcMeanCurrent>ProtRMS_ExcCurrent) {ErrorSet(ERR_MEANIEXC_OVERCURRENT);} break;
    case 17: SKiiPA_MeanTemp = MeanValues[17]; if (SKiiPA_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHA_OVERTEMP);} break;
    case 18: SKiiPB_MeanTemp = MeanValues[18]; if (SKiiPB_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHB_OVERTEMP);} break;
    case 19: SKiiPC_MeanTemp = MeanValues[19]; if (SKiiPC_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHC_OVERTEMP);} break;
    default: break;

    /*case 0:  PhAVoltRMS = (((float)MeanValues[0])-2048)*ChGain[0]; if (PhAVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHA_OVERVOLTAGE);} break;
	case 1:  PhBVoltRMS = (((float)MeanValues[1])-2048)*ChGain[1]; if (PhBVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHB_OVERVOLTAGE);} break;
	case 2:  PhCVoltRMS = (((float)MeanValues[2])-2048)*ChGain[2]; if (PhCVoltRMS>ProtRMS_OutpVoltage) {ErrorSet(ERR_RMSPHC_OVERVOLTAGE);} break;

	case 3: Rect1MeanCurrent = fabs(((float)MeanValues[3]-2048.0)*ChGain[3]);	if (Rect1MeanCurrent>ProtRMS_RectCurrent) {ErrorSet(ERR_MEANRECT1_OVERCURRENT);} break;
	case 4: Rect2MeanCurrent = fabs(((float)MeanValues[4]-2048.0)*ChGain[4]);	if (Rect2MeanCurrent>ProtRMS_RectCurrent) {ErrorSet(ERR_MEANRECT2_OVERCURRENT);} break;
	case 5: Rect3MeanCurrent = fabs(((float)MeanValues[5]-2048.0)*ChGain[5]);	if (Rect3MeanCurrent>ProtRMS_RectCurrent) {ErrorSet(ERR_MEANRECT3_OVERCURRENT);} break;

	case 6: UDCMeanVoltage = fabs(((float)MeanValues[6]-2048.0)*ChGain[6]);	if (UDCMeanVoltage>ProtRMS_UDCVoltage ) {ErrorSet(ERR_MEANUDCOUT_OVERVOLTAGE);} break;

	case 7: GenPhaseRMSCurrent = (((float)MeanValues[7])-2048)*ChGain[7];	 if (GenPhaseRMSCurrent>ProtRMS_GenPhCurrent) {ErrorSet(ERR_RMSIGEN_OVERCURRENT);} break;

	case 8: GenPhaseRMSVoltage = (((float)MeanValues[8])-2048)*ChGain[8]; if (GenPhaseRMSVoltage>ProtRMS_GenPhVoltage) {ErrorSet(ERR_RMSUGEN_OVERVOLTAGE);} break;

	case 9: IExcMeanCurrent = fabs(((float)MeanValues[9])-2048)*ChGain[9];if (IExcMeanCurrent>ProtRMS_ExcCurrent) {ErrorSet(ERR_MEANIEXC_OVERCURRENT);} break;

	case 10: SKiiPC_MeanTemp = fabs(((float)MeanValues[10])-2048)*ChGain[10];	if (SKiiPC_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHC_OVERTEMP);} break;
	case 11: SKiiPB_MeanTemp = fabs(((float)MeanValues[11])-2048)*ChGain[11];	if (SKiiPB_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHB_OVERTEMP);} break;
	case 12: SKiiPA_MeanTemp = fabs(((float)MeanValues[12])-2048)*ChGain[12];	if (SKiiPA_MeanTemp>ProtRMS_IGBTTemp) {ErrorSet(ERR_MEANPHA_OVERTEMP);} break;

	case 14: PhCIGBTCurrentRMS = (((float)MeanValues[14])-2048)*ChGain[14];	if (PhCIGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHC_OVERCURRENT);}  break;
	case 15: PhCCurrentRMS = (((float)MeanValues[15])-2048)*ChGain[15]; if (PhCCurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHC_OVERCURRENT);}  break;
	case 16: PhBIGBTCurrentRMS = (((float)MeanValues[16])-2048)*ChGain[16]; if (PhBIGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHB_OVERCURRENT);}  break;
	case 17: PhBCurrentRMS = (((float)MeanValues[17])-2048)*ChGain[17]; if (PhBCurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHB_OVERCURRENT);}  break;
	case 18: PhAIGBTCurrentRMS = (((float)MeanValues[18])-2048)*ChGain[18]; if (PhAIGBTCurrentRMS>ProtRMS_IGBTCurrent) {ErrorSet(ERR_RMSINVPHA_OVERCURRENT);}  break;
	case 19: PhACurrentRMS = (((float)MeanValues[19])-2048)*ChGain[19]; if (PhACurrentRMS>ProtRMS_OutpCurrent) {ErrorSet(ERR_RMSPHA_OVERCURRENT);}  break;
	default: break; */
	}




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
