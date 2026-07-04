/*
 * ALG_DataExchange.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>


void DMADataTransferSet()
{
	MainPWM_AMP_F = (float)MainPWM_AMP;
	ExcitationPWM_Width_F = (float) ExcitationPWM_Width;


	if (DataPacksSent >= DATA_PACKS_TOTAL || CurrentBufNum>=7)
	{
			 if (CurrentBufNum==0) {CtoMIpcRegs.CTOMIPCSET.bit.IPC10 = 1;DataSentCounter++;}
		else if (CurrentBufNum==1) {CtoMIpcRegs.CTOMIPCSET.bit.IPC11 = 1;DataSentCounter++;}
		else if (CurrentBufNum==2) {CtoMIpcRegs.CTOMIPCSET.bit.IPC12 = 1;DataSentCounter++;}
		else if (CurrentBufNum==3) {CtoMIpcRegs.CTOMIPCSET.bit.IPC13 = 1;DataSentCounter++;}
		else if (CurrentBufNum==4) {CtoMIpcRegs.CTOMIPCSET.bit.IPC14 = 1;DataSentCounter++;}
		else if (CurrentBufNum==5) {CtoMIpcRegs.CTOMIPCSET.bit.IPC15 = 1;DataSentCounter++;}
		else if (CurrentBufNum==6) {CtoMIpcRegs.CTOMIPCSET.bit.IPC16 = 1;DataSentCounter++;}


		if 		(CtoMIpcRegs.CTOMIPCFLG.bit.IPC10 == 0) {CurrentBufNum=0;CurrentDataOffset = 0;DataPacksSent=0;}
		else if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC11 == 0) {CurrentBufNum=1;CurrentDataOffset = 0;DataPacksSent=0;}
		else if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC12 == 0) {CurrentBufNum=2;CurrentDataOffset = 0;DataPacksSent=0;}
		else if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC13 == 0) {CurrentBufNum=3;CurrentDataOffset = 0;DataPacksSent=0;}
		else if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC14 == 0) {CurrentBufNum=4;CurrentDataOffset = 0;DataPacksSent=0;}
		else if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC15 == 0) {CurrentBufNum=5;CurrentDataOffset = 0;DataPacksSent=0;}
		else if (CtoMIpcRegs.CTOMIPCFLG.bit.IPC16 == 0) {CurrentBufNum=6;CurrentDataOffset = 0;DataPacksSent=0;}
		else {CurrentBufNum = 10;}
	}


	if (CurrentBufNum<7 && DataPacksSent<DATA_PACKS_TOTAL)
	{
	CurrentBufOffset = CurrentBufNum*SHMEM_BUFFER_SIZE;

	if (CurrentDataOffset==0)
	{
		DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  0xFFFF; CurrentDataOffset++;
		Time = MeasurmentsTime;
		Time0 = (Uint16) ((Time&0x000000000000FFFF)>>0);
		Time1 = (Uint16) ((Time&0x00000000FFFF0000)>>16);
		Time2 = (Uint16) ((Time&0x0000FFFF00000000)>>32);
		Time3 = (Uint16) ((Time&0xFFFF000000000000)>>48);
		DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)Time3; CurrentDataOffset++;
		DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)Time2; CurrentDataOffset++;
		DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)Time1; CurrentDataOffset++;
		DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)Time0; CurrentDataOffset++;
		DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (Uint16) DMA_CHANNELS_TO_TRANSFER; CurrentDataOffset++;
	}

	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value1; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value2; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value3; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value4; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value5; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value6; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value7; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value8; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value9; CurrentDataOffset++;

	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value10; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value11; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value12; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value13; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value14; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value15; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value16; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value17; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value18; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value19; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  ADC_Current_Value20; CurrentDataOffset++;


	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[0]])+OsciSourcesOffset[SOC[0]])*OsciSourcesGain[SOC[0]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[1]])+OsciSourcesOffset[SOC[1]])*OsciSourcesGain[SOC[1]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[2]])+OsciSourcesOffset[SOC[2]])*OsciSourcesGain[SOC[2]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[3]])+OsciSourcesOffset[SOC[3]])*OsciSourcesGain[SOC[3]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[4]])+OsciSourcesOffset[SOC[4]])*OsciSourcesGain[SOC[4]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[5]])+OsciSourcesOffset[SOC[5]])*OsciSourcesGain[SOC[5]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[6]])+OsciSourcesOffset[SOC[6]])*OsciSourcesGain[SOC[6]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[7]])+OsciSourcesOffset[SOC[7]])*OsciSourcesGain[SOC[7]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[8]])+OsciSourcesOffset[SOC[8]])*OsciSourcesGain[SOC[8]]);CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(((*OsciSources[SOC[9]])+OsciSourcesOffset[SOC[9]])*OsciSourcesGain[SOC[9]]);CurrentDataOffset++;

	/*
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ActivePower*0.1); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ReactivePower*0.1); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)((OutputFrequency-50.0)*10000.0); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(PhasePID->CurrentOutput*50); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(PIDActivePowerSetpoint*0.1); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(VoltagePID->CurrentOutput*10000); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(MainPWM_AMP); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ChopperPWM_Width); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ExcitationPWM_Width); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(GenSpeedDoubleFiltered*10); CurrentDataOffset++;
	 */

	//Для тестирования СВ
	/*
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(Reg_GenSpeeed*0.1); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(Reg_IDC*0.1); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)((OutputFrequency-50.0)*10000.0); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(PhasePID->CurrentOutput*50); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(IDCFiltered*0.1); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(VoltagePID->CurrentOutput*10000); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ExcitationPWLimit); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ExcitationCurrentPID->CurrentOutput); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ExcitationPWM_Width); CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  (short)(ExcitationSetpoint*1000.0); CurrentDataOffset++;
	*/
	DataPacksSent++;
	}

}

void FPGA_Read_Init (void)
{
	SetEPIRead0Size(0x2);
	SetEPIRead0Adress(0x4000000000);
	SetEPIRead0Count(4);
}

void FPGA_Get_Data (void)
{
	FPGA_Error_Latch_H = (Uint16) ReadWordFromEPIFIFO();
	FPGA_Error_Latch_L = (Uint16) ReadWordFromEPIFIFO();
	FPGA_Error_State_H = (Uint16) ReadWordFromEPIFIFO();
	FPGA_Error_State_L = (Uint16) ReadWordFromEPIFIFO();
}

void FPGA_Read_Data (void)
{
	FPGA_Error_Latch_H = DMABufFPGA1[0];
	FPGA_Error_Latch_L = DMABufFPGA1[1];
	FPGA_Error_State_H = DMABufFPGA1[2];
	FPGA_Error_State_L = DMABufFPGA1[3];
	FPGA_EMIF_WD_Value = DMABufFPGA1[4];
	FPGA_Inputs_State_H = DMABufFPGA1[5];
	FPGA_Inputs_State_L = DMABufFPGA1[6];
	FPGA_OCP_Counter  = DMABufFPGA1[11];

}

Uint16 GetDINValue(Uint16 InputNum)
{

	Uint16 InputMask = 1;
	if (InputNum<=15)
	{
		InputMask = (InputMask<<InputNum);
		if ((FPGA_Inputs_State_L&InputMask)!=0) {return 1;}
		else {return 0;}
	}
	else if (InputNum<=30)
	{
		InputNum = InputNum-16;
		InputMask = (InputMask<<InputNum);
		if ((FPGA_Inputs_State_H&InputMask)!=0) {return 1;}
		else {return 0;}
	}
	else {return 0;}

}

void FPGA_Write_Exitation(void)
{
      EngineExcPWM = ExcitationPWM_Width;
	  WriteWordTo_FPGA(62,0);
	  WriteWordTo_FPGA(63,(unsigned short)(EngineExcPWM&0x0000FFFF));
}

void FPGA_Write_Phase_Freq (void)
{

	WriteWordTo_FPGA(56,(unsigned short)(Data_FromM3[12]));
	WriteWordTo_FPGA(57,(unsigned short)(Data_FromM3[13]));

}

void FPGA_Write_Amp (void)
{
    EngineAMPSET=Data_FromM3[1];
	WriteWordTo_FPGA(52,0);
	WriteWordTo_FPGA(53,(unsigned short)(EngineAMPSET&0x0000FFFF));

}

void FPGA_Write_Booster (void)
{
    EngineBuckPWM = 2400;
    WriteWordTo_FPGA(60,0);
    WriteWordTo_FPGA(61,(unsigned short)(EngineBuckPWM&0x0000FFFF));
}


void FPGA_EXTOUT_SetBit(Uint16 bitnum)
{
    Uint16 Mask = 1;
    if (bitnum<16)
    {
        Mask = Mask<<bitnum;
        FPGA_EXT_OUT = FPGA_EXT_OUT|Mask;
    }
}

void FPGA_EXTOUT_ClearBit(Uint16 bitnum)
{
    Uint16 Mask = 1;
    if (bitnum<16)
    {
        Mask = Mask<<bitnum;
        Mask = ~ Mask;
        FPGA_EXT_OUT = FPGA_EXT_OUT&Mask;
    }
}

void FPGA_Write_Aux (void)
{
	WriteWordTo_FPGA(64,0);
	WriteWordTo_FPGA(65,(unsigned short)(CoolerFanPWM_Width&0x0000FFFF));
}


void FPGA_Write_Data (void)
{
    BoosterInput1 = ReadWordFrom_FPGA(18);

	CommitDataValue=43690;
	CommitCurrentsValue=21845;
	FPGA_EMIF_WD_Value = DMABufFPGA1[4];
	//if (FPGA_EMIF_WD_Value_Prev==FPGA_EMIF_WD_Value) {ErrorSet(ERR_DMA_OVERRIDE);}
	FPGA_EMIF_WD_Value_Prev = FPGA_EMIF_WD_Value;

	if (FPGA_EMIF_WD_Value!=0xFFFF)
	{
		WriteWordTo_FPGA(51,FPGA_EMIF_WD_Value+1);
	}
	else
	{
		FPGA_EMIF_WD_Value = 1;
		WriteWordTo_FPGA(51,FPGA_EMIF_WD_Value);
	}

	WriteWordTo_FPGA(100,(unsigned short)(CommitDataValue));

	        /*if (CurrentLimitControlActivated!=0)
	        {
	        	float CurrentSetpoint = (float)((unsigned short)Data_FromM3[17])*500.0/65535.0;
	        	float UpperLimit = 2048.0 + CurrentSetpoint/1.2507204149;
	        	float LowerLimit = 2048.0 - CurrentSetpoint/1.2507204149;
	        	Const_TopACurrentLimit = (Uint16)(UpperLimit);
	        	Const_TopBCurrentLimit = (Uint16)(UpperLimit);
	        	Const_TopCCurrentLimit = (Uint16)(UpperLimit);
	        	Const_BotACurrentLimit = (Uint16)(LowerLimit);
	        	Const_BotBCurrentLimit = (Uint16)(LowerLimit);
	        	Const_BotCCurrentLimit = (Uint16)(LowerLimit);
	        }*/
}


void FPGA_Write_DMA (void)
{

}


void M3_Read_Data(void) {
	int k;
	for (k = 0; k < 4; k++) {
		if (M3_TransmittCounter >= MTOC_DATA_TRANSMITT_COUNT) {
			M3_TransmittCounter = 0;
		}
		else {
			M3_TransmittCounter++;
		}
		Data_FromM3[M3_TransmittCounter] = ReadFrom_MTOC_MSGRAM(M3_TransmittCounter);
	}
}

void M3_Get_Setups(void)
{
	if (CtoMIpcRegs.MTOCIPCSTS.bit.IPC8 == 1) //Write Uint param
	{
		Uint16 DataID = ReadFrom_MTOC_MSGRAM(SETUP_DATA_START_ADDR);
		Uint16 Data_Value = ReadFrom_MTOC_MSGRAM(SETUP_DATA_START_ADDR+1);
		if (DataID < SETUP_UINT_DATA_COUNT)
			{
			//Save Data to setup parameter
			Setups_WriteUintParam(DataID, Data_Value);
			}
		CtoMIpcRegs.MTOCIPCACK.bit.IPC8 = 1;
	}

	if (CtoMIpcRegs.MTOCIPCSTS.bit.IPC9 == 1) //Write float param
	{
		Uint16 DataID = ReadFrom_MTOC_MSGRAM(SETUP_DATA_START_ADDR);
		float FloatData = ReadFrom_MTOC_MSGRAM_Float(SETUP_DATA_START_ADDR+2);
		if (DataID < SETUP_FLOAT_DATA_COUNT)
			{
			//Save Data to setup parameter
			Setups_WriteFloatParam(DataID, FloatData);
			}
		CtoMIpcRegs.MTOCIPCACK.bit.IPC9 = 1;
	}

	if (CtoMIpcRegs.MTOCIPCSTS.bit.IPC6 == 1) //Get osci channel data
	{
		Uint16 DataID = ReadFrom_MTOC_MSGRAM(OSCI_SETUP_DATA_START_ADDR);
		float OsciGain = 0;
		float OsciOffset = 0;
		if (DataID<(20+OSCI_SOURCES_COUNT))
		{
			if (DataID<20) {OsciGain = ChGain[DataID]; OsciOffset = ADCOffset; }
			else {DataID=DataID-20;OsciGain = OsciSourcesGain[DataID]; OsciOffset = OsciSourcesOffset[DataID];}
		}
		WriteTo_CTOM_MSGRAM_Float(OSCI_SETUP_DATA_START_ADDR,OsciGain);
		WriteTo_CTOM_MSGRAM_Float(OSCI_SETUP_DATA_START_ADDR+4,OsciOffset);
		CtoMIpcRegs.MTOCIPCACK.bit.IPC6 = 1;
	}

	if (CtoMIpcRegs.MTOCIPCSTS.bit.IPC7 == 1) //Set service osci channel number
	{
		Uint16 DataID = ReadFrom_MTOC_MSGRAM(OSCI_SETUP_DATA_START_ADDR);
		Uint16 NumToSet = ReadFrom_MTOC_MSGRAM(OSCI_SETUP_DATA_START_ADDR+2);
		if (DataID<10 && NumToSet<OSCI_SOURCES_COUNT)
		{
			SOC[DataID] = NumToSet;
		}
		CtoMIpcRegs.MTOCIPCACK.bit.IPC7 = 1;
	}

}

void M3_Write_Data (Uint32 CyclesCounter)
{
	int k;
	for (k=0;k<4;k++)
	{
		if (TransmittCounter >= CTOM_DATA_TRANSMITT_COUNT)
		{
			TransmittCounter = 0;
		}

			Uint16 Addr_Offset = TransmittCounter % CTOM_DATA_TRANSMITT_COUNT;
			switch (Addr_Offset)
				{
				// SCADA DATA ------------------------------------------------------------
				case 0: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_Error_Latch_H)); break;
				case 1: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_Error_Latch_L)); break;
				case 2: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_Error_State_H)); break;
				case 3: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_Error_State_L)); break;
				case 4: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_Inputs_State_H)); break;
				case 5: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_Inputs_State_L)); break;
				case 6: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(WorkingCycleTime_uS>>16)); break;
				case 7: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(WorkingCycleTime_uS)); break;
				case 8: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Peak_WorkingCycleTime_uS>>16)); break;
				case 9: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Peak_WorkingCycleTime_uS)); break;
				case 10: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MainCyclesCounter)); break;
				//case 10: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_EMIF_WD_Value)); break;

				case 11: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[0]*100.0)); break;
				case 12: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[1]*100.0)); break;
				case 13: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[2]*100.0)); break;
				case 14: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[3])); break;
				case 15: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[4])); break;
				case 16: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[5])); break;
				case 17: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[6])); break;
				case 18: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[7])); break;
				case 19: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[8])); break;
				case 20: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[9])); break;
				case 21: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[10])); break;
				case 22: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[11])); break;
				case 23: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[12])); break;
				case 24: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[13])); break;
				case 25: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[14])); break;
				case 26: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[15])); break;
				case 27: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[16])); break;
				case 28: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[17])); break;
				case 29: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[18])); break;
				case 30: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[19])); break;

				//case 31: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(CalculationsDelay)); break; //TEMP
				case 31: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SCADASystemState)); break;
				case 32: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(LastCommand)); break;
				case 33: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(GlobalError)); break;
				case 34: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(RotationFrequency/*Частота вращения генератора об/мин*/)); break;
				case 35: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ActivePower*0.01/*Активная мощность*/)); break;
				case 36: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ReactivePower*0.01/*Реактивная мощность*/)); break;
				case 37: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((OutputFrequency-50.0)*1000.0/*Частота*/)); break;
				case 38: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(OutputPhase/*Фаза*/)); break;
				case 39: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(DCPower*0.01)); break;
				case 40: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SystemState/*Состояние*/)); break;
				case 41: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags1>>16)&0x0000FFFF)); break;
				case 42: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags1&0x0000FFFF)); break;
				case 43: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags2>>16)&0x0000FFFF)); break;
				case 44: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags2&0x0000FFFF)); break;
				case 45: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags4>>16)&0x0000FFFF)); break;
				case 46: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags4&0x0000FFFF)); break;
				case 47: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Isolated_Grid_ControlMode)); break;
				// SCADA DATA END-----------------------------

				case    48  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    ChopperCurrent*100.0  ));break;
				case    49  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    ExcitationCurrent*100.0   ));break;
				case    50  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    ExcitationCurrent2*100.0  ));break;
				case    51  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    DCVoltage_IN    ));break;
				case    52  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    DCVoltage_OUT   ));break;
				case    53  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    EXCTemp1    ));break;
				case    54  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    EXCTemp2    ));break;
				case    55  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    EXCTemp3    ));break;
				case    56  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_U1  ));break;
				case    57  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_V1  ));break;
				case    58  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_W1  ));break;
				case    59  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Voltage_1   ));break;
				case    60  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp1_1 ));break;
				case    61  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp1_2 ));break;
				case    62  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp1_3 ));break;
				case    63  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_U2  ));break;
				case    64  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_V2  ));break;
				case    65  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_W2  ));break;
				case    66  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Voltage_2   ));break;
				case    67  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp2_1 ));break;
				case    68  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp2_2 ));break;
				case    69  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp2_3 ));break;
				case    70  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_U3  ));break;
				case    71  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_V3  ));break;
				case    72  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Current_W3  ));break;
				case    73  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Voltage_3   ));break;
				case    74  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp3_1 ));break;
				case    75  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp3_2 ));break;
				case    76  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Temp3_3 ));break;

				case    78  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    DMABufFPGA1[14] ));break; // velocity H
				case    79  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    DMABufFPGA1[15] ));break; // velocity L

				case    80  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Gen_TempPh1*100.0 ));break; // GEN TEMP 1
				case    81  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Gen_TempPh2*100.0 ));break; // GEN TEMP 2
				case    82  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Gen_TempPh3*100.0 ));break; // GEN TEMP 3
				case    83  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Gen_TempExc1*100.0 ));break; // GEN TEMP 2
				case    84  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Gen_TempExc2*100.0 ));break; // GEN TEMP 2
				case    85  :   WriteTo_CTOM_MSGRAM(Addr_Offset,(short)(    Gen_TempRearBearing*100.0 ));break; // GEN TEMP 2

				default: WriteTo_CTOM_MSGRAM(0, (short)(FPGA_Error_Latch_H));break;
				}


			//Time = MeasurmentsTime;
			//Time0 = (Uint16) Time; Time = Time>>16;
			//Time1 = (Uint16) Time; Time = Time>>16;
			//Time2 = (Uint16) Time; Time = Time>>16;
			//Time3 = (Uint16) Time; Time = Time>>16;
			//WriteTo_CTOM_MSGRAM(100, (short)Time3);
			//WriteTo_CTOM_MSGRAM(101, (short)Time2);
			//WriteTo_CTOM_MSGRAM(102, (short)Time1);
			//WriteTo_CTOM_MSGRAM(103, (short)Time0);
			TransmittCounter++;
	//	}
	//else
	//	{
	//	CTOMtimeoutCounter = 0;
	//	}
	}

}
