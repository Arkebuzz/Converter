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

	OsciValues1 = (short)(SEG_EXC_UIN);
	OsciValues2 = (short)(SEG_U_MAIN);
	OsciValues3 = (short)(SEG_I_IN*10.0);
	OsciValues4 = (short)(SEG_IEXC1*10.0);
	OsciValues5 = (short)(SEG_RECT_RI1);
	OsciValues6 = (short)(SEG_RECT_RI2);
	OsciValues7 = (short)(SEG_RECT_RI3);
	OsciValues8 = (short)(SEG_RECT_RI4);
	OsciValues9 = (short)(SEG_RECT_RI5);
	OsciValues10 = (short)(SEG_RECT_RI6);
	OsciValues11 = (short)(SEG_I1_UDC);
	OsciValues12 = (short)(SEG_F_UA);
	OsciValues13 = (short)(SEG_F_UB);
	OsciValues14 = (short)(SEG_F_UC);
	OsciValues15 = (short)(SEG_I1_IPH2+SEG_I2_IPH2+SEG_I3_IPH2); //Фаза А выход скипа
	OsciValues16 = (short)(SEG_I1_IPH3+SEG_I2_IPH3+SEG_I3_IPH3); //Фаза B выход скипа
	OsciValues17 = (short)(SEG_I1_IPH1+SEG_I2_IPH1+SEG_I3_IPH1); //Фаза С выход скипа
	OsciValues18 = (short)(SEG_F_IA); //Фаза А выход преобр
	OsciValues19 = (short)(SEG_F_IB); //Фаза B выход преобр
	OsciValues20 = (short)(SEG_F_IC); //Фаза C выход преобр


	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues1; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues2; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues3; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues4; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues5; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues6; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues7; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues8; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues9; CurrentDataOffset++;

	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues10; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues11; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues12; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues13; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues14; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues15; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues16; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues17; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues18; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues19; CurrentDataOffset++;
	DMADest = (Uint16*)(0x00012000 + CurrentDataOffset + CurrentBufOffset); *DMADest  =  OsciValues20; CurrentDataOffset++;


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

void FPGA_Read_Data (Uint16 CyclesCounter)
{
	short ValueFromEMIF;
	int k;

	FPGA_Error_Latch_H = DMABufFPGA1[0];
	FPGA_Error_Latch_L = DMABufFPGA1[1];
	FPGA_Error_State_H = DMABufFPGA1[2];
	FPGA_Error_State_L = DMABufFPGA1[3];
	FPGA_EMIF_WD_Value = DMABufFPGA1[4];
	FPGA_Inputs_State_H = DMABufFPGA1[5];
	FPGA_Inputs_State_L = DMABufFPGA1[6];

	FPGA_OCP_Counter  = DMABufFPGA1[11];

	BoosterInput1 = DMABufFPGA1[12];
	Booster_ChopperCurrent = (float)BoosterInput1;
	BoosterInput2 = DMABufFPGA1[13];
	Booster_BrakeCurrent = (float)BoosterInput2;
	BoosterInput3 = DMABufFPGA1[14];
	Booster_DCVoltage_IN = (float)BoosterInput3;
	BoosterInput4 = DMABufFPGA1[15];
	Booster_DCVoltage_OUT = (float)BoosterInput4;
	BoosterInput5 = DMABufFPGA1[16];
	Booster_ChopperSKiiPCurrent = (float)BoosterInput5;
	BoosterInput6 = DMABufFPGA1[17];
	Booster_BrakeSKiiPCurrent = (float)BoosterInput6;
	BoosterInput7 = DMABufFPGA1[18];
	Booster_ReGCurrentLimit = (float)BoosterInput7;
	BoosterInput8 = DMABufFPGA1[19];
	Booster_ChopperSKiiPTemperature = (float)((BoosterInput8>>8)&0x00FF);
	Booster_BrakeSKiiPTemperature = (float)(BoosterInput8&0x00FF);

	BoosterInput9 = DMABufFPGA1[20];
	BoosterInput10 = DMABufFPGA1[21];
	BoosterInput11 = DMABufFPGA1[22];
	BoosterInput12 = DMABufFPGA1[23];

	for (int i=0; i<60; i++)
	{
		CellsInput[i] = ReadWordFrom_FPGA(110+i);
	}


/*
	ValueFromEMIF=ReadWordFrom_FPGA(0);
	FPGA_Error_Latch_H = (Uint16) ValueFromEMIF;

	ValueFromEMIF=ReadWordFrom_FPGA(1);
	FPGA_Error_Latch_L = (Uint16) ValueFromEMIF;

	ValueFromEMIF =  ReadWordFrom_FPGA(4);
	FPGA_EMIF_WD_Value = (Uint16) ValueFromEMIF;

	ValueFromEMIF = ReadWordFrom_FPGA(12);
	BoosterInput1 = (Uint16) ValueFromEMIF;
	Booster_ChopperCurrent = (float)BoosterInput1;

	ValueFromEMIF = ReadWordFrom_FPGA(14);
	BoosterInput3 = (Uint16) ValueFromEMIF;
	Booster_DCVoltage_IN = (float)BoosterInput3;

	if ((CalcCycleCounter%2)==0)
	{
		ValueFromEMIF = ReadWordFrom_FPGA(17);
		BoosterInput6 = (Uint16) ValueFromEMIF;
		Booster_BrakeSKiiPCurrent = (float)BoosterInput6;

		ValueFromEMIF = ReadWordFrom_FPGA(15);
		BoosterInput4 = (Uint16) ValueFromEMIF;
		Booster_DCVoltage_OUT = (float)BoosterInput4;
	}
	else
	{
		ValueFromEMIF = ReadWordFrom_FPGA(16);
		BoosterInput5 = (Uint16) ValueFromEMIF;
		Booster_ChopperSKiiPCurrent = (float)BoosterInput5;

		ValueFromEMIF = ReadWordFrom_FPGA(18);
		BoosterInput7 = (Uint16) ValueFromEMIF;
		Booster_ReGCurrentLimit = (float)BoosterInput7;
	}

	if (CalcCycleCounter==0)
		{
		ValueFromEMIF = ReadWordFrom_FPGA(2);
		FPGA_Error_State_H = (Uint16) ValueFromEMIF;

		ValueFromEMIF =  ReadWordFrom_FPGA(3);
		FPGA_Error_State_L = (Uint16) ValueFromEMIF;

		ValueFromEMIF = ReadWordFrom_FPGA(23);
		BoosterInput12 = (Uint16) ValueFromEMIF;
		}
	if (CalcCycleCounter==1)
		{
		ValueFromEMIF = ReadWordFrom_FPGA(5);
		FPGA_Inputs_State_H = (Uint16) ValueFromEMIF;

		ValueFromEMIF = ReadWordFrom_FPGA(6);
		FPGA_Inputs_State_L = (Uint16) ValueFromEMIF;

		ValueFromEMIF = ReadWordFrom_FPGA(13);
		BoosterInput2 = (Uint16) ValueFromEMIF;
		Booster_BrakeCurrent = (float)BoosterInput2;
		}
	if (CalcCycleCounter==2)
		{
		ValueFromEMIF = ReadWordFrom_FPGA(11);
		FPGA_OCP_Counter = (Uint16) ValueFromEMIF;

		ValueFromEMIF = ReadWordFrom_FPGA(19);
		BoosterInput8 = (Uint16) ValueFromEMIF;
		Booster_ChopperSKiiPTemperature = (float)((BoosterInput8>>8)&0x00FF);
		Booster_BrakeSKiiPTemperature = (float)(BoosterInput8&0x00FF);
		}
	if (CalcCycleCounter==3)
		{
		ValueFromEMIF = ReadWordFrom_FPGA(20);
		BoosterInput9 = (Uint16) ValueFromEMIF;

		ValueFromEMIF = ReadWordFrom_FPGA(21);
		BoosterInput10 = (Uint16) ValueFromEMIF;

		ValueFromEMIF = ReadWordFrom_FPGA(22);
		BoosterInput11 = (Uint16) ValueFromEMIF;
		}
*/

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
	  WriteWordTo_FPGA(62,0);
	  WriteWordTo_FPGA(63,(unsigned short)(ExcitationPWM_Width&0x0000FFFF));
}

void FPGA_Write_Phase_Freq (void)
{
	WriteWordTo_FPGA(54,(unsigned short)((MainPWM_OutputFrequency>>16)&0x0000FFFF));
	WriteWordTo_FPGA(55,(unsigned short)(MainPWM_OutputFrequency&0x0000FFFF));

	WriteWordTo_FPGA(56,(unsigned short)((MainPWM_PhaseShift>>16)&0x0000FFFF));
	WriteWordTo_FPGA(57,(unsigned short)(MainPWM_PhaseShift&0x0000FFFF));

}

void FPGA_Write_Amp (void)
{
	WriteWordTo_FPGA(52,0);
	WriteWordTo_FPGA(53,(unsigned short)(MainPWM_AMP&0x0000FFFF));

	WriteWordTo_FPGA(58,(unsigned short)((MainPWM_PhaseSet>>16)&0x0000FFFF));
	WriteWordTo_FPGA(59,(unsigned short)(MainPWM_PhaseSet&0x0000FFFF));
}

void FPGA_Write_Booster (void)
{
	//WriteWordTo_FPGA(82,(unsigned short)(Booster_Voltage_Setpoint));
	//WriteWordTo_FPGA(83,(unsigned short)(Booster_Current_Limit));
	//WriteWordTo_FPGA(84,(unsigned short)(Brake_PWM_Width));

    WriteWordTo_FPGA(82,(unsigned short)(ChopperPWM_Width));
    WriteWordTo_FPGA(83,(unsigned short)(ExcitationPWM_Width));
}


void FPGA_Write_Aux (void)
{

	if (CalcCycleCounter==0)
	{
	WriteWordTo_FPGA(64,0);
	WriteWordTo_FPGA(65,(unsigned short)(CoolerFanPWM_Width&0x0000FFFF));
	}

	if (CalcCycleCounter==1)
	{
	WriteWordTo_FPGA(75,(unsigned short)(Const_TopACurrentLimit));
	WriteWordTo_FPGA(76,(unsigned short)(Const_TopBCurrentLimit));
	}

	if (CalcCycleCounter==2)
	{
	WriteWordTo_FPGA(77,(unsigned short)(Const_TopCCurrentLimit));
	WriteWordTo_FPGA(78,(unsigned short)(Const_BotACurrentLimit));
	}

	if (CalcCycleCounter==3)
	{
	WriteWordTo_FPGA(79,(unsigned short)(Const_BotBCurrentLimit));
	WriteWordTo_FPGA(80,(unsigned short)(Const_BotCCurrentLimit));
	}
}

void FPGA_Write_Data (void)
{
	CommitDataValue=43690;
	CommitCurrentsValue=21845;
	FPGA_EMIF_WD_Value = DMABufFPGA1[4];
	if (FPGA_EMIF_WD_Value_Prev==FPGA_EMIF_WD_Value) {ErrorSet(ERR_DMA_OVERRIDE);}
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


void M3_Read_Data (void)
{
	int k;
	for (k=0;k<4;k++)
	{
		//if (CtoMIpcRegs.MTOCIPCSTS.bit.IPC8 == 1)
		//{
			if (M3_TransmittCounter >= MTOC_DATA_TRANSMITT_COUNT)
				{
				M3_TransmittCounter = 0;
				Data_FromM3[M3_TransmittCounter] = ReadFrom_MTOC_MSGRAM(M3_TransmittCounter);
				//CtoMIpcRegs.MTOCIPCACK.bit.IPC8 = 1;
				}
			else
				{
				Data_FromM3[M3_TransmittCounter] = ReadFrom_MTOC_MSGRAM(M3_TransmittCounter);
				M3_TransmittCounter++;
				}
		//}
	}
	//Data_FromM3[1] = MTOC_cyclesCounter;
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
		//CTOMtimeoutCounter++;
		//if ((CtoMIpcRegs.CTOMIPCFLG.bit.IPC7 == 0) || CTOMtimeoutCounter>10000000)
		//{
		if (TransmittCounter >= CTOM_DATA_TRANSMITT_COUNT)
		{
			TransmittCounter = 0;
			//CtoMIpcRegs.CTOMIPCSET.bit.IPC7 = 1;
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

				case 11: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SCADASystemState)); break;
				case 12: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(LastCommand)); break;
				case 13: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(GlobalError)); break;

				case 14: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(0)); break;	//VGEN
				case 15: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ActivePower*0.001)); break; //Активная мощность
				case 16: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ReactivePower*0.001)); break; // Реактивная мощность
				case 17: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((OutputFrequency)*100.0)); break; //Частота
				case 18: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(OutputPhase)); break; //Фаза
				case 19: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(DCPower*0.001)); break; //Мощность DC

				case 20: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SystemState)); break; //Состояние
				case 21: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags1>>16)&0x0000FFFF)); break;
				case 22: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags1&0x0000FFFF)); break;
				case 23: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags2>>16)&0x0000FFFF)); break;
				case 24: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags2&0x0000FFFF)); break;
				case 25: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags3>>16)&0x0000FFFF)); break;
				case 26: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags3&0x0000FFFF)); break;
				case 27: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags4>>16)&0x0000FFFF)); break;
				case 28: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags4&0x0000FFFF)); break;

				case 29: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(0)); break; //RESERVE
				case 30: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(0)); break; //RESERVE

				case 31:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_EXC_UIN)); break;
				case 32:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_U_MAIN)); break;
				case 33:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_U_R)); break;
				case 34:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I_IN*10.0)); break;
				case 35:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(IExcMeanCurrent*10.0)); break;
				case 36:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_IEXC2*10.0)); break;
				case 37:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SKiiPC_MeanTemp)); break;

				case 38:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_UG1)); break;
				case 39:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_UG2)); break;
				case 40:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(UDCMeanVoltage)); break;
				case 41:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_UDCP)); break;
				case 42:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_UDCN)); break;
				case 43:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_RI1)); break;
				case 44:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_RI2)); break;
				case 45:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_RI3)); break;
				case 46:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_RI4)); break;
				case 47:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_RI5)); break;
				case 48:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_RI6)); break;
				case 49:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SKiiPA_MeanTemp)); break;
				case 50:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SKiiPB_MeanTemp)); break;
				case 51:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_RECT_TC1)); break;

				case 52:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhC1IGBTCurrentRMS)); break;
				case 53:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhA1IGBTCurrentRMS)); break;
				case 54:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhB1IGBTCurrentRMS)); break;
				case 55:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_SKIPH1)); break;
				case 56:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_SKIPH2)); break;
				case 57:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_SKIPH3)); break;
				case 58:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_SKIDISCH)); break;
				case 59:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_UDC)); break;
				case 60:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_TSK1)); break;
				case 61:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_TSK2)); break;
				case 62:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_TSK3)); break;
				case 63:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_TSKDISCH)); break;
				case 64:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_TLF)); break;
				case 65:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_TC1)); break;
				case 66:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I1_TC2)); break;

                case 67:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhC2IGBTCurrentRMS)); break;
                case 68:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhA2IGBTCurrentRMS)); break;
                case 69:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhB2IGBTCurrentRMS)); break;
                case 70:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_SKIPH1)); break;
                case 71:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_SKIPH2)); break;
                case 72:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_SKIPH3)); break;
                case 73:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_SKIDISCH)); break;
                case 74:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_UDC)); break;
                case 75:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_TSK1)); break;
                case 76:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_TSK2)); break;
                case 77:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_TSK3)); break;
                case 78:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_TSKDISCH)); break;
                case 79:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_TLF)); break;
                case 80:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_TC1)); break;
                case 81:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I2_TC2)); break;

                case 82:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhC3IGBTCurrentRMS)); break;
                case 83:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhA3IGBTCurrentRMS)); break;
                case 84:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhB3IGBTCurrentRMS)); break;
                case 85:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_SKIPH1)); break;
                case 86:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_SKIPH2)); break;
                case 87:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_SKIPH3)); break;
                case 88:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_SKIDISCH)); break;
                case 89:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_UDC)); break;
                case 90:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_TSK1)); break;
                case 91:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_TSK2)); break;
                case 92:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_TSK3)); break;
                case 93:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_TSKDISCH)); break;
                case 94:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_TLF)); break;
                case 95:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_TC1)); break;
                case 96:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_I3_TC2)); break;

                case 97:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhACurrentRMS)); break;
                case 98:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhBCurrentRMS)); break;
                case 99:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhCCurrentRMS)); break;
                case 100:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhAVoltRMS)); break;
                case 101:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhBVoltRMS)); break;
                case 102:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhCVoltRMS)); break;
                case 103:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_F_UN)); break;
                case 104:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_F_UGRID)); break;
                case 105:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_F_TC1)); break;
                case 106:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_F_TC2)); break;
                case 107:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_F_TC3)); break;

                case 108:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_TEMPCELL_EXC)); break;
                case 109:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_TEMPCELL_RECT)); break;
                case 110:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_TEMPCELL_INV1)); break;
                case 111:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_TEMPCELL_INV2)); break;
                case 112:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_TEMPCELL_INV3)); break;
                case 113:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_TEMPCELL_FILT)); break;

                case 114:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_DCS1)); break;
                case 115:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_DCS2)); break;
                case 116:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_DCS3)); break;
                case 117:  WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SEG_DCS3)); break;

				//case 10: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_EMIF_WD_Value)); break;
				/*case 11: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[0])); break;
				case 12: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[1])); break;
				case 13: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[2])); break;
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
				case 30: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MeanValues[19])); break;*/


				 /*
				case 11: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN1_Values)); break;
				case 12: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN2_Values)); break;
				case 13: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN3_Values)); break;
				case 14: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN4_Values)); break;
				case 15: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN5_Values)); break;
				case 16: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN6_Values)); break;
				case 17: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN7_Values)); break;
				case 18: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN8_Values)); break;
				case 19: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN9_Values)); break;
				case 20: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN10_Values)); break;
				case 21: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN11_Values)); break;
				case 22: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN12_Values)); break;
				case 23: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN13_Values)); break;
				case 24: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN14_Values)); break;
				case 25: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN15_Values)); break;
				case 26: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN16_Values)); break;
				case 27: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN17_Values)); break;
				case 28: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN18_Values)); break;
				case 29: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN19_Values)); break;
				case 30: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(AIN20_Values)); break;
				*/

				//case 31: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(CalculationsDelay)); break; //TEMP

				/*case 31: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SCADASystemState)); break;
				case 32: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(LastCommand)); break;
				case 33: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(GlobalError)); break;
				case 34: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(RotationFrequency)); break; //Частота вращения генератора об/мин
				case 35: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ActivePower*0.001)); break; //Активная мощность
				case 36: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ReactivePower*0.001)); break; // Реактивная мощность
				case 37: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((OutputFrequency-60.0)*1000.0)); break; //Частота
				case 38: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(OutputPhase)); break; //Фаза
				case 39: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(DCPower*0.001)); break;
				case 40: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(SystemState)); break; //Состояние
				case 41: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags1>>16)&0x0000FFFF)); break;
				case 42: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags1&0x0000FFFF)); break;
				case 43: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags2>>16)&0x0000FFFF)); break;
				case 44: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags2&0x0000FFFF)); break;
				case 45: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((ErrorFlags4>>16)&0x0000FFFF)); break;
				case 46: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ErrorFlags4&0x0000FFFF)); break;
				case 47: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Const_IsConnectedToResistiveLoad)); break;
				// SCADA DATA END-----------------------------

				case 48: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FreqChangeAmp*1000)); break;
				case 49: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhaseChangeAmp*10)); break;
				case 50: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FVAL_VoltageMagnitude)); break;

				// Voltage/AMP REGULATORS
				case 51: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Reg_VoltageError)); break;
				case 52: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(OutputRMSVoltage)); break;
				case 53: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ReactivePowerPID->CurrentOutput*0.001)); break;
				case 54: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Reg_ReactivePowerBoost*0.001)); break;
				case 55: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(ReactivePowerSetpoint*0.001)); break;
				case 56: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(VoltagePID->CurrentOutput*10000)); break;
				case 57: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(MainPWM_AMP)); break;

				// Freq-power REGULATORS``
				case 58: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Alg_PowerDamping*0.001)); break;
				case 59: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FreqToPowerPID->CurrentOutput*0.001)); break;
				case 60: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PIDActivePowerSetpoint*0.001)); break;
				case 61: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((FrequencyPID->CurrentOutput-60.0)*10000.0)); break;
				case 62: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(PhasePID->CurrentOutput)); break;
				case 63: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((Alg_BaseFrequency-60.0)*10000.0)); break;
				case 64: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Reg_APowerSetpoint*0.001)); break;
				case 65: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((Const_BaseFrequency-60.0)*10000.0)); break;
				case 66: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)((Const_FrequencyWindow)*10000.0)); break;
				case 67: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(UDCErr)); break;

				case 68: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(FPGA_OCP_Counter)); break;

				case 70: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput1)); break;
				case 71: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput2)); break;
				case 72: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput3)); break;
				case 73: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput4)); break;
				case 74: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput5)); break;
				case 75: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput6)); break;
				case 76: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput7)); break;
				case 77: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput8)); break;
				case 78: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput9)); break;
				case 79: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput10)); break;
				case 80: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput11)); break;
				case 81: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(BoosterInput12)); break;
				case 82: WriteTo_CTOM_MSGRAM(Addr_Offset, (short)(Const_BoosterModeDisable)); break; */

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
