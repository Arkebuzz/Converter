/*
 * ALG_StructuresInit.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>




void InitData(void)
{
   SCADASystemState = SYSTEM_STATE_INIT;
   Const_pi = 3.1415926535897932384626433832795;
   Const_2pi = 2 * Const_pi;
   Const_sqrt3_div2 = sqrt(3.0)/2.0;
   Const_2pi_div3 = 2.0*Const_pi/3.0;
   Const_2_div3 = 2.0/3.0;
   Const_1_div360 = 1.0/360.0;


   //Const_50MHzCountsForPeriod = 50000000.0/Const_BaseFrequency; //FPGA Counts for base frequency
   Const_CycleTime = ((float)MAIN_CYCLE_US)/1000000.0; //Cycle time in seconds
   Const_DelayedPhaseCounts = 6000/MAIN_CYCLE_US; //Number of cycles for phase change calculation. Value is time in uS for shift
   Const_DelayedPhaseFrequency = 1000000.0/(((float)MAIN_CYCLE_US)*((float)Const_DelayedPhaseCounts));
   Const_FreqUpdateDelay = 100000/MAIN_CYCLE_US; //Cycles for sync frequency update. Value is time in uS for update
   Const_RotFreqMultiplier =  (1000000.0/((float)(ADC_SAMPLE_CYCLE_US*ADC_SAMPLE_DATA_DIVIDER)))*60.0/Const_GenNumberOfPoles; //Multiplier for rot freq calculations
   FrequencyMultiplier = 1000000.0/MAIN_CYCLE_US/4.0;
   //FrequencyMultiplier2 = 1000000.0/(ADC_SAMPLE_CYCLE_US*ADC_SAMPLE_DATA_DIVIDER)/4.0;
   MinOutputFrequency = FrequencyMultiplier/MEASURMENTS_BUFFER_SIZE + 2;
   float RMSCalcCount = 1000000.0/Const_BaseFrequency/MAIN_CYCLE_US;
   Const_RMSCalcCount = (Uint16)(RMSCalcCount);
   RotationSpeedPID = new FloatPID();
   	   	   	   	   	   	   // PIDStep,  Prop, 	Int, 	Diff, InpGain, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, StoredValuesNum
   RotationSpeedPID->PIDInit(0.0003,	500,	100,	0,		1,		350000,			0,			350000,		  0,			10);

   // PIDStep, Proportional, Integral, Dif, InputG, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, StoredValuesNum
   VoltagePID = new FloatPID();
   VoltagePID->PIDInit(Const_CycleTime,VPID_P,VPID_I,0,VPID_G,0.98,0,0.98,0,5);

   // PIDStep, Proportional, Integral, Dif, InputG, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, StoredValuesNum
   ReactivePowerPID = new FloatPID();
   ReactivePowerPID->PIDInit(Const_CycleTime,RPPID_P,RPPID_I,0,RPPID_G,Const_ReactivePowerMax,Const_ReactivePowerMin,Const_ReactivePowerMax,Const_ReactivePowerMin,10);

  // PIDStep, Proportional, Integral, Dif, InputG, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, StoredValuesNum
   FreqToPowerPID = new FloatPID();
   FreqToPowerPID->PIDInit(Const_CycleTime,APPID_P,APPID_I,0,1,Const_MaxOutputPower,Const_MinOutputPower,Const_MaxOutputPower,Const_MinOutputPower,10);

   FrequencyPID = new FloatPID_D();
   	   	   	   	   	   // PIDStep, 	Proportional, Integral, 	Dif,  InputG, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, PropLimit , IntLimit , StoredValuesNum, DerDenom0, DerDenom1
   FrequencyPID->PIDInit(Const_CycleTime, FRPID_P,	FRPID_I,  FRPID_D, FRPID_G, Const_FreqUpperLim,	Const_FreqLowerLim,		Const_FreqUpperLim,	Const_FreqLowerLim, Const_FreqPropLimit, Const_FreqIntLimit, 10, 0.0042, -0.0038);

   // PIDStep, Proportional, Integral, Dif, InputG, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, PropLimit, IntLimit, StoredValuesNum
   PhasePID = new FloatCyclicPID();
   PhasePID->PIDInit(Const_CycleTime,PHPID_P,PHPID_I,0,PHPID_G,360,0,360,0, Const_PhasePropLimit , Const_PhaseIntLimit ,10);

   //StepSize, Prop, Integ, Diff, InputGainPos, InputGainNeg, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, StoredValuesNum
   ChopperPID = new FloatBipolarPID();
   ChopperPID->PIDInit(Const_CycleTime,0.002,0.008,0,4,2,1,0,1,0,10);

   //StepSize, Prop, Integ, Diff, InputGainPos, InputGainNeg, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, StoredValuesNum
   DCVoltagePID = new FloatBipolarPID();
   DCVoltagePID->PIDInit(Const_CycleTime,DCVPID_P,DCVPID_I,0,DCVPID_POSG,DCVPID_NEGG,Const_ExcitationMax,0,Const_ExcitationMax,0,10);

   // PIDStep, Proportional, Integral, Dif, InputG, UpperIntLim, LowerIntLim, UpperOutLim, LowerOutLim, PropLimit, IntLimit, StoredValuesNum
   ExcitationCurrentPID = new FloatPID();
   ExcitationCurrentPID->PIDInit(Const_CycleTime,EXCPID_P,EXCPID_I,0,1,2400,100,2400,0,10);

   //200Hz +
   APLowpassFilter = new SecOrdFilter();
   APLowpassFilter->SecOrdFilterInit(0.49574818181875879, -354.29862811679584, 0.00022436222727281383,  0.94685520578248072, 0.7478740909093794, 0.00011218111363640691, 0.05314479421751938, 461.15257697458804, 0.02657239710875969);
   //60Hz +
   APBandstopFilter = new SecOrdFilter();
   APBandstopFilter->SecOrdFilterInit(0.79164095825106429, -38.194820971329577, 0.00026874614373765962, 0.99427077685430065, 0.89582047912553209, 0.00013437307186882978, -0.20262981860323631, 4.3197324828734285, 0.89868509069838187);
   //120Hz +
   APBandstopFilter2 = new SecOrdFilter();
   APBandstopFilter2->SecOrdFilterInit(0.61422366289467645, -137.65031108047768, 0.00024213354943420143,   0.97935245333792831, 0.80711183144733822, 0.00012106677471710073, -0.36512879044325192, 31.135766835944036, 0.81743560477837407);
   //200Hz +
   APLowpassFilter2 = new SecOrdFilter();
   APLowpassFilter2->SecOrdFilterInit(0.49574818181875879, -354.29862811679584, 0.00022436222727281383,  0.94685520578248072, 0.7478740909093794, 0.00011218111363640691, 0.05314479421751938, 461.15257697458804, 0.02657239710875969);


   //200Hz +
   RPLowpassFilter = new SecOrdFilter();
   RPLowpassFilter->SecOrdFilterInit(0.49574818181875879, -354.29862811679584, 0.00022436222727281383,  0.94685520578248072, 0.7478740909093794, 0.00011218111363640691, 0.05314479421751938, 461.15257697458804, 0.02657239710875969);
   //60Hz +
   RPBandstopFilter = new SecOrdFilter();
   RPBandstopFilter->SecOrdFilterInit(0.79164095825106429, -38.194820971329577, 0.00026874614373765962, 0.99427077685430065, 0.89582047912553209, 0.00013437307186882978, -0.20262981860323631, 4.3197324828734285, 0.89868509069838187);
   //120Hz +
   RPBandstopFilter2 = new SecOrdFilter();
   RPBandstopFilter2->SecOrdFilterInit(0.61422366289467645, -137.65031108047768, 0.00024213354943420143,   0.97935245333792831, 0.80711183144733822, 0.00012106677471710073, -0.36512879044325192, 31.135766835944036, 0.81743560477837407);
   //50Hz +
   RPLowpassFilter2 = new SecOrdFilter();
   RPLowpassFilter2->SecOrdFilterInit(0.85594371854441209, -27.476145439080121, 0.0002783915577816618, 0.995878578184138, 0.927971859272206, 0.0001391957788908309, 0.0041214218158620169, 29.547797998929209, 0.0020607109079310085);

   //??Hz +
   GenPhaseRMSLowpassFilter = new SecOrdFilter();
   GenPhaseRMSLowpassFilter->SecOrdFilterInit(0.99822390428669461, -0.00788867177604397, 0.00019982239042866943,    0.99999921113282242, 0.99911195214334736, 9.9911195214334728E-5, 7.8886717760439689E-7, 0.0078956804065487, 3.9443358880219845E-7 );
   //25Hz +
   GenSpeedLowpassFilter = new SecOrdFilter();
   GenSpeedLowpassFilter->SecOrdFilterInit(0.92631038863074344, -7.1294705585603, 0.00028894655829461144, 0.99893057941621588, 0.96315519431537167, 0.00014447327914730572, 0.0010694205837840449, 7.3982452665293943, 0.00053471029189202247);
   //500Hz +
   ExcCurrentFilter = new SecOrdFilter();
   ExcCurrentFilter->SecOrdFilterInit(0.002557578617609052, -1484.2270035404765, 0.00015038363679264135, 0.77736594946892867, 0.50127878930880454, 7.5191818396320675E-5, 0.22263405053107146, 2631.284819583735, 0.11131702526553573);
   //50Hz +
   IDCLowpassFilter = new SecOrdFilter();
   IDCLowpassFilter->SecOrdFilterInit(0.85594371854441209, -27.476145439080121, 0.0002783915577816618, 0.995878578184138, 0.927971859272206, 0.0001391957788908309, 0.0041214218158620169, 29.547797998929209, 0.0020607109079310085);
   //50Hz +
   dFilter = new SecOrdFilter();
   dFilter->SecOrdFilterInit(0.85594371854441209, -27.476145439080121, 0.0002783915577816618, 0.995878578184138, 0.927971859272206, 0.0001391957788908309, 0.0041214218158620169, 29.547797998929209, 0.0020607109079310085);
   //50Hz +
   qFilter = new SecOrdFilter();
   qFilter->SecOrdFilterInit(0.85594371854441209, -27.476145439080121, 0.0002783915577816618, 0.995878578184138, 0.927971859272206, 0.0001391957788908309, 0.0041214218158620169, 29.547797998929209, 0.0020607109079310085);
   //40Hz +
   FreqFilter = new SecOrdFilter();
   FreqFilter->SecOrdFilterInit(0.88370069226492465, -17.847725417004447, 0.00028255510383973865, 0.99732284118744918, 0.94185034613246232, 0.00014127755191986933, 0.0026771588125506676, 18.924274851628748, 0.0013385794062753338);
   //50Hz +
   UDCFilter= new SecOrdFilter();
   UDCFilter->SecOrdFilterInit(0.85594371854441209, -27.476145439080121, 0.0002783915577816618, 0.995878578184138, 0.927971859272206, 0.0001391957788908309, 0.0041214218158620169, 29.547797998929209, 0.0020607109079310085);
   //10Hz +
   UDCFilterSlow= new SecOrdFilter();
   UDCFilterSlow->SecOrdFilterInit(0.9664650325732238, -1.1644939164043808, 0.00029496975488598354, 0.99982532591253925, 0.9832325162866119, 0.00014748487744299177, 0.00017467408746065714, 1.1842490902821814, 8.733704373032857E-5);
   //10Hz +
   OutputFrequencyFilter = new SecOrdFilter();
   OutputFrequencyFilter->SecOrdFilterInit(0.9664650325732238, -1.1644939164043808, 0.00029496975488598354, 0.99982532591253925, 0.9832325162866119, 0.00014748487744299177, 0.00017467408746065714, 1.1842490902821814, 8.733704373032857E-5);
   //??Hz +
   RegVoltageErrorFilter = new SecOrdFilter();
   RegVoltageErrorFilter->SecOrdFilterInit(0.99822390428669461, -0.00788867177604397, 0.00019982239042866943,    0.99999921113282242, 0.99911195214334736, 9.9911195214334728E-5, 7.8886717760439689E-7, 0.0078956804065487, 3.9443358880219845E-7 );
   //??Hz +
   RMSVoltageFilter = new SecOrdFilter();
   RMSVoltageFilter->SecOrdFilterInit(0.99822390428669461, -0.00788867177604397, 0.00019982239042866943,    0.99999921113282242, 0.99911195214334736, 9.9911195214334728E-5, 7.8886717760439689E-7, 0.0078956804065487, 3.9443358880219845E-7 );

   //PR_DCOvervoltage = new TimedProtection();
   PR_DCOvervoltage.TimedProtectionInit(Const_DCPRMinCycles, Const_DCPRMaxCycles, Const_DCPRReleaseCycles, Const_TPR_DCLower, Const_TPR_DCUpper);

   //PR_RectUnbalance = new TimedProtection();
   PR_RectUnbalance.TimedProtectionInit(Const_RectPRMinCycles, Const_RectPRMaxCycles, Const_RectPRReleaseCycles, Const_TPR_RectCurrLower, Const_TPR_RectCurrUpper);

   //PR_OutpVoltageUnbalance = new TimedProtection();
   PR_OutpVoltageUnbalance.TimedProtectionInit(Const_VoutPRMinCycles, Const_VoutPRMaxCycles, Const_VoutPRReleaseCycles, Const_TPR_VoutLower, Const_TPR_VoutUpper);

   //PR_IGBTCurrUnbalance = new TimedProtection();
   PR_IGBTCurrUnbalance.TimedProtectionInit(Const_IGBTUnbPRMinCycles, Const_IGBTUnbPRMaxCycles, Const_IGBTUnbPRReleaseCycles, Const_TPR_IGBTUnbLower, Const_TPR_IGBTUnbUpper);

   //PR_IGBTAOverCurr = new TimedProtection();
   PR_IGBTAOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);
   //PR_IGBTBOverCurr = new TimedProtection();
   PR_IGBTBOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);
   //PR_IGBTCOverCurr = new TimedProtection();
   PR_IGBTCOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);

   //PR_APOverload = new TimedProtection();
   PR_APOverload.TimedProtectionInit(Const_APHighPRMinCycles, Const_APHighPRMaxCycles, Const_APHighPRReleaseCycles, Const_TPR_APHighLower, Const_TPR_APHighUpper);

   //PR_NegativeAPower = new TimedProtection();
   PR_NegativeAPower.TimedProtectionInit(Const_APLowPRMinCycles, Const_APLowPRMaxCycles, Const_APLowPRReleaseCycles, Const_TPR_APLowLower, Const_TPR_APLowUpper);

   //PR_RPOverload  = new TimedProtection();
   PR_RPOverload.TimedProtectionInit(Const_RPHighPRMinCycles, Const_RPHighPRMaxCycles, Const_RPHighPRReleaseCycles, Const_TPR_RPHighLower, Const_TPR_RPHighUpper);

   //PR_NegativeRPower = new TimedProtection();
   PR_NegativeRPower.TimedProtectionInit(Const_RPLowPRMinCycles, Const_RPLowPRMaxCycles, Const_RPLowPRReleaseCycles, Const_TPR_RPLowLower, Const_TPR_RPLowUpper);

   //PR_OutputFrequency = new TimedProtection();
   PR_OutputFrequency.TimedProtectionInit(Const_FreqPRMinCycles, Const_FreqPRMaxCycles, Const_FreqPRReleaseCycles, Const_TPR_FreqLower, Const_TPR_FreqUpper);

   PR_ActivePowerRegulation.TimedProtectionInit(Const_APOscPRMinCycles, Const_APOscPRMaxCycles, Const_APOscPRReleaseCycles, Const_TPR_APOscLower, Const_TPR_APOscUpper);

   PR_GroundFault.TimedProtectionInit(Const_GFaultPRMinCycles, Const_GFaultPRMaxCycles, Const_GFaultPRReleaseCycles, 1.5, 2.5);

   PR_OutpPhAOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);
   PR_OutpPhBOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);
   PR_OutpPhCOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);


   if (Const_IsResistiveLoadConverter!=0)
   {
	   ChGain[3] = 0.555875739981785;
	   ChGain[4] = 0.555875739981785;
	   ChGain[5] = 0.555875739981785;
   }


}



void PIDReInit()
{
	VoltagePID->PIDInit(Const_CycleTime,VPID_P,VPID_I,0,VPID_G,0.98,0,0.98,0,5);
	ReactivePowerPID->PIDInit(Const_CycleTime,RPPID_P,RPPID_I,0,RPPID_G,Const_ReactivePowerMax,Const_ReactivePowerMin,Const_ReactivePowerMax,Const_ReactivePowerMin,10);
	FreqToPowerPID->PIDInit(Const_CycleTime,APPID_P,APPID_I,0,1,Const_MaxOutputPower,Const_MinOutputPower,Const_MaxOutputPower,Const_MinOutputPower,10);
	FrequencyPID->PIDInit(Const_CycleTime, FRPID_P,	FRPID_I,  FRPID_D, FRPID_G, Const_FreqUpperLim,	Const_FreqLowerLim,		Const_FreqUpperLim,	Const_FreqLowerLim, Const_FreqPropLimit, Const_FreqIntLimit, 10, 0.0042, -0.0038);
	PhasePID->PIDInit(Const_CycleTime,PHPID_P,PHPID_I,0,PHPID_G,360,0,360,0, Const_PhasePropLimit , Const_PhaseIntLimit ,10);
	DCVoltagePID->PIDInit(Const_CycleTime,DCVPID_P,DCVPID_I,0,DCVPID_POSG,DCVPID_NEGG,Const_ExcitationMax,0,Const_ExcitationMax,0,10);
	ExcitationCurrentPID->PIDInit(Const_CycleTime,EXCPID_P,EXCPID_I,0,1,10000,0,10000,0,10);

	PR_DCOvervoltage.TimedProtectionInit(Const_DCPRMinCycles, Const_DCPRMaxCycles, Const_DCPRReleaseCycles, Const_TPR_DCLower, Const_TPR_DCUpper);
	PR_RectUnbalance.TimedProtectionInit(Const_RectPRMinCycles, Const_RectPRMaxCycles, Const_RectPRReleaseCycles, Const_TPR_RectCurrLower, Const_TPR_RectCurrUpper);
	PR_OutpVoltageUnbalance.TimedProtectionInit(Const_VoutPRMinCycles, Const_VoutPRMaxCycles, Const_VoutPRReleaseCycles, Const_TPR_VoutLower, Const_TPR_VoutUpper);
	PR_IGBTCurrUnbalance.TimedProtectionInit(Const_IGBTUnbPRMinCycles, Const_IGBTUnbPRMaxCycles, Const_IGBTUnbPRReleaseCycles, Const_TPR_IGBTUnbLower, Const_TPR_IGBTUnbUpper);
	PR_IGBTAOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);
	PR_IGBTBOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);
	PR_IGBTCOverCurr.TimedProtectionInit(Const_IGBTOCPPRMinCycles, Const_IGBTOCPPRMaxCycles, Const_IGBTOCPPRReleaseCycles, Const_TPR_IGBTOCPLower, Const_TPR_IGBTOCPUpper);
	PR_APOverload.TimedProtectionInit(Const_APHighPRMinCycles, Const_APHighPRMaxCycles, Const_APHighPRReleaseCycles, Const_TPR_APHighLower, Const_TPR_APHighUpper);
	PR_NegativeAPower.TimedProtectionInit(Const_APLowPRMinCycles, Const_APLowPRMaxCycles, Const_APLowPRReleaseCycles, Const_TPR_APLowLower, Const_TPR_APLowUpper);
	PR_RPOverload.TimedProtectionInit(Const_RPHighPRMinCycles, Const_RPHighPRMaxCycles, Const_RPHighPRReleaseCycles, Const_TPR_RPHighLower, Const_TPR_RPHighUpper);
	PR_NegativeRPower.TimedProtectionInit(Const_RPLowPRMinCycles, Const_RPLowPRMaxCycles, Const_RPLowPRReleaseCycles, Const_TPR_RPLowLower, Const_TPR_RPLowUpper);
	PR_OutputFrequency.TimedProtectionInit(Const_FreqPRMinCycles, Const_FreqPRMaxCycles, Const_FreqPRReleaseCycles, Const_TPR_FreqLower, Const_TPR_FreqUpper);


	/*VoltagePID->SetProp(VPID_P);
	 VoltagePID->SetInt(VPID_I);
	 VoltagePID->SetInputGain(VPID_G);

	 ReactivePowerPID->SetProp(RPPID_P);
	 ReactivePowerPID->SetInt(RPPID_I);
	 ReactivePowerPID->SetInputGain(RPPID_G);

	 FreqToPowerPID->SetProp(APPID_P);
	 FreqToPowerPID->SetInt(APPID_I);

	 FrequencyPID->SetProp(FRPID_P);
	 FrequencyPID->SetInt(FRPID_I);
	 FrequencyPID->SetInputGain(FRPID_G);

	 PhasePID->SetProp(PHPID_P);
	 PhasePID->SetInt(PHPID_I);
	 PhasePID->SetInputGain(PHPID_G);

	 DCVoltagePID->SetProp(DCVPID_P);
	 DCVoltagePID->SetInt(DCVPID_I);

	 ExcitationCurrentPID->SetProp(EXCPID_P);
	 ExcitationCurrentPID->SetInt(EXCPID_I);*/
}
