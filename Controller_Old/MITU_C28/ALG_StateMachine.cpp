/*
 * ALG_StateMachine.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */
#include <GlobalData.h>

void CommandsProcess(void)
{


	MomentLimitedPower = 150000; //3.1415*GenSpeedFiltered*Const_GeneratorMomentLimit/30.0;
	Alg_DCInpSetpoint = 0;


	if(Const_BoosterModeDisable==0)
	{
	 //---------old turbine char start ------------------
	Const_SpeedLimitFreerun = 1200;
	float SpeedLineLag = GenSpeedFiltered/3.4-340.0; //GenSpeedFiltered*200/(2200-1600)-340.0;
	if (SpeedLineLag>400) {SpeedLineLag=400;}
	if (SpeedLineLag<200) {SpeedLineLag=200;}
	if (GenSpeedFiltered>(ZeroBaseSpeed+SpeedLineLag)) {ZeroBaseSpeed = ZeroBaseSpeed+0.002;}
	else {ZeroBaseSpeed = ZeroBaseSpeed-0.002;}
	//if (Const_SpeedLimitFreerun<1200) {Const_SpeedLimitFreerun=1200;}
	if (ZeroBaseSpeed < Const_SpeedLimitFreerun) {ZeroBaseSpeed=Const_SpeedLimitFreerun;}
	if (ZeroBaseSpeed > 2500) {ZeroBaseSpeed=2500;}
	if (Const_SpeedLimitMaxPower<2900) {Const_SpeedLimitMaxPower=2900;}

	TurbinePower_K = 0.001*Const_MaxOutputPower/(Const_SpeedLimitMaxPower - ZeroBaseSpeed);
	TurbinePower_B = -1*ZeroBaseSpeed*TurbinePower_K;
	 //---------old turbine char end ------------------
	}
	else
	{
	Const_SpeedLimitFreerun = 2300;
	if (GenSpeedFiltered>(ZeroBaseSpeed+300)) {ZeroBaseSpeed = ZeroBaseSpeed+0.001;}
	else {ZeroBaseSpeed = ZeroBaseSpeed-0.001;}
	//if (Const_SpeedLimitFreerun<2200) {Const_SpeedLimitFreerun=2200;}
	if (ZeroBaseSpeed < Const_SpeedLimitFreerun) {ZeroBaseSpeed=Const_SpeedLimitFreerun;}
	if (ZeroBaseSpeed > 2500) {ZeroBaseSpeed=2500;}
	if (Const_SpeedLimitMaxPower<2900) {Const_SpeedLimitMaxPower=2900;}

	TurbinePower_K = 0.001*Const_MaxOutputPower/(Const_SpeedLimitMaxPower - ZeroBaseSpeed);
	TurbinePower_B = -1*ZeroBaseSpeed*TurbinePower_K;
	}


	//---------new turbine char start ------------------
	/*if (GenSpeedFiltered>CurrentBaseSpeed) {CurrentBaseSpeed = CurrentBaseSpeed+0.001;}
		else {CurrentBaseSpeed = CurrentBaseSpeed-0.001;}
	if (CurrentBaseSpeed<1000) {CurrentBaseSpeed=1000;}
	if (CurrentBaseSpeed>2900) {CurrentBaseSpeed=2900;}

	Uint16 CharPoint=0;
	if (CurrentBaseSpeed>=1000 && CurrentBaseSpeed<1100) {CharPoint=0;}
	else if (CurrentBaseSpeed>=1100 && CurrentBaseSpeed<1200) {CharPoint=1;}
	else if (CurrentBaseSpeed>=1200 && CurrentBaseSpeed<1300) {CharPoint=2;}
	else if (CurrentBaseSpeed>=1300 && CurrentBaseSpeed<1400) {CharPoint=3;}
	else if (CurrentBaseSpeed>=1400 && CurrentBaseSpeed<1500) {CharPoint=4;}
	else if (CurrentBaseSpeed>=1500 && CurrentBaseSpeed<1600) {CharPoint=5;}
	else if (CurrentBaseSpeed>=1600 && CurrentBaseSpeed<1700) {CharPoint=6;}
	else if (CurrentBaseSpeed>=1700 && CurrentBaseSpeed<1800) {CharPoint=7;}
	else if (CurrentBaseSpeed>=1800 && CurrentBaseSpeed<1900) {CharPoint=8;}
	else if (CurrentBaseSpeed>=1900 && CurrentBaseSpeed<2000) {CharPoint=9;}
	else if (CurrentBaseSpeed>=2000 && CurrentBaseSpeed<2100) {CharPoint=10;}
	else if (CurrentBaseSpeed>=2100 && CurrentBaseSpeed<2200) {CharPoint=11;}
	else if (CurrentBaseSpeed>=2200 && CurrentBaseSpeed<2300) {CharPoint=12;}
	else if (CurrentBaseSpeed>=2300 && CurrentBaseSpeed<2400) {CharPoint=13;}
	else if (CurrentBaseSpeed>=2400 && CurrentBaseSpeed<2500) {CharPoint=14;}
	else if (CurrentBaseSpeed>=2500 && CurrentBaseSpeed<2600) {CharPoint=15;}
	else if (CurrentBaseSpeed>=2600 && CurrentBaseSpeed<2700) {CharPoint=16;}
	else if (CurrentBaseSpeed>=2700 && CurrentBaseSpeed<2800) {CharPoint=17;}
	else if (CurrentBaseSpeed>=2800 && CurrentBaseSpeed<=2900) {CharPoint=18;}

	TurbinePower_K = CurrentBaseSpeed*1.4/2900.0;
	float CharSpeed = (float)(CharPoint)*100.0 + 1000.0;
	float PowerSetpoint = TurbineChar[CharPoint] + (TurbineChar[CharPoint+1] - TurbineChar[CharPoint])*(CurrentBaseSpeed - CharSpeed)/100.0;
	TurbinePower_B = PowerSetpoint - TurbinePower_K*CurrentBaseSpeed;
	*/
	//---------new turbine char end ------------------

	//TurbineLimitedPower = 1000.0*(GenSpeedFiltered*TurbinePower_K + TurbinePower_B);

	//if (TurbineLimitedPower<MomentLimitedPower) {MomentLimitedPower = TurbineLimitedPower; }
    PowerDampingLimit = Const_MaxOutputPower - MomentLimitedPower;

    float DCOUT_Limited = DCVoltage_OUT;
    if (DCOUT_Limited<10) {DCOUT_Limited=10;}
    float BrakePWMWidth = 3750*((MomentLimitedPower/DCOUT_Limited)/(DCOUT_Limited/1.05));
    if (BrakePWMWidth>3750) {BrakePWMWidth = 3750;}
    if (BrakePWMWidth<26) {BrakePWMWidth = 0;}
    if (GenSpeedFiltered<1200) {BrakePWMWidth = 0;}
    if (Const_BrakeModeDisable==0) {Brake_PWM_Width = (unsigned short) BrakePWMWidth;}
    else {Brake_PWM_Width = 0;}
    //Brake_PWM_Width = 1500; //TEST!

    if (PowerDampingLimit<0) PowerDampingLimit = 0;
    if (PowerDampingLimit>(Const_MaxOutputPower-20000)) PowerDampingLimit = Const_MaxOutputPower-20000;

    if (PowerDampingLimit>Alg_PowerDamping) {Alg_PowerDamping = Alg_PowerDamping+100;}
    if (PowerDampingLimit<Alg_PowerDamping) {Alg_PowerDamping = Alg_PowerDamping-100;}

    //if (Alg_PowerDamping<0) Alg_PowerDamping = 0;
    //if (Alg_PowerDamping>(Const_MaxOutputPower-20000)) Alg_PowerDamping = Const_MaxOutputPower-20000;
    Alg_PowerDamping = 0;


    NewCommand = Data_FromM3[0];

	if (NewCommand!=LastCommand || NewCommand==1)
	        {
	        CurrentCommand = NewCommand;
	        LastCommand = NewCommand;
	        }
	    else
	        {
	        CurrentCommand = 0;
	        }

	MasterCode = Data_FromM3[3];
	if (CurrentCommand==1) {ErrorSet(ERR_EMERGENCY_STOP_ALG);CurrentAlg=0;} //Emergency stop;

	if (ErrorFlags4!=0 && GlobalError==0) {ErrorSet(ERR_EMERGENCY_STOP_ALG);} //Always stop on GlobalError
	if (ErrorFlags3!=0 && GlobalError==0) {ErrorSet(ERR_EMERGENCY_STOP_ALG);} //Always stop on GlobalError
	if (ErrorFlags2!=0 && GlobalError==0) {ErrorSet(ERR_EMERGENCY_STOP_ALG);} //Always stop on GlobalError
	if (ErrorFlags1!=0 && GlobalError==0) {ErrorSet(ERR_EMERGENCY_STOP_ALG);} //Always stop on GlobalError

	if (CurrentAlg==0) //Emergency stop
		{
	    Const_MinOutputPower = -10000;

				Alg_Excitation_limit = 0;
				Alg_DCOutSetpoint = 0;
				BrakeLocker = 1;
				MinimumCoolingPWMWidth=0;
				ActivateBrake(0);
				ActivateExcitation(0);

		ErrorSet(ERR_EMERGENCY_STOP_ALG);
		CurrentSysytemState = 0;
		SCADASystemState = SYSTEM_STATE_EMERGENCY; //EMERGENCY
		if (CurrentCommand==2 /*&& (FPGA_Error_State_H&0x7FF)==0 && (FPGA_Error_State_L&0xFBFF)==0*/ && RecalcOffset==0)
			{
			ErrorReset();
			CurrentAlg = 1;
			}
		else if (CurrentCommand==114) {Rotation_Power_ControlMode=1;}
		else if (CurrentCommand==115) {Rotation_Power_ControlMode=0;}
		else if (MasterCode==11941)
			{
			if (CurrentCommand==100) {SystemManualControl=1;}
			else if (CurrentCommand==101) {SystemManualControl=0;}
			else if (CurrentCommand==102 && SystemManualControl==1) {ExcitationManualControl=1;}
			else if (CurrentCommand==103) {ExcitationManualControl=0;}
			else if (CurrentCommand==104 && SystemManualControl==1) {ChopperManualControl=1;}
			else if (CurrentCommand==105) {ChopperManualControl=0;}
			else if (CurrentCommand==106 && SystemManualControl==1) {InverterVoltageManualControl=1;}
			else if (CurrentCommand==107) {InverterVoltageManualControl=0;}
			else if (CurrentCommand==108 && SystemManualControl==1) {InverterFrequencyManualControl=1;}
			else if (CurrentCommand==109) {InverterFrequencyManualControl=0;}
			else if (CurrentCommand==110 && SystemManualControl==1) {VentilationManualControl=1;}
			else if (CurrentCommand==111) {VentilationManualControl=0;}
			else if (CurrentCommand==112) {Isolated_Grid_ControlMode=1;}
			else if (CurrentCommand==113) {Isolated_Grid_ControlMode=0;}
			else if (CurrentCommand==120) {PIDReInit();}
			else if (CurrentCommand==999) {if (SaveDataRequestFlag==0) {SaveDataRequestFlag = 1;}} //Save to flash
			else if (CurrentCommand==998) {if (SaveDataRequestFlag==0) {SaveDataRequestFlag = 3;}} //Erase flash


			}
		}
	else if (CurrentAlg==1)
	{
	 //Idle state Ready to work
	    Const_MinOutputPower = -10000;

		Alg_DCOutSetpoint = 0;
		MinimumCoolingPWMWidth=0;
		if (SystemManualControl==1)
		{
			Alg_Excitation_limit = Const_ExcitationMax;
			SCADASystemState = SYSTEM_STATE_OPERATION_WARNING;
			if (Data_FromM3[6]!=0)
			{
				if (InverterActivated==0)
				{
				if (InverterFrequencyManualControl==0)
					{
					OutputFrequency=Const_BaseFrequency;
					FrequencyPID->ResetData(OutputFrequency,OutputFrequency);
					PhasePID->ResetData(0,0);
					}
				ActivateInverter(1);
				}
			}
			else {ActivateInverter(0);}
			if (Data_FromM3[8]!=0) {ActivateExcitation(1);} else {ActivateExcitation(0);}
		}
		else
		{
			if (DCVoltage_OUT<30) {SCADASystemState = SYSTEM_STATE_IDLE_DISCHARGED;}
			else {SCADASystemState = SYSTEM_STATE_IDLE_CHARGED;}
			//if (CurrentCommand==3) {CurrentAlg=2;} //К запуску возбуждения
			ActivateInverter(0);
			ActivateChopper(0);
			ActivateExcitation(0);
		}
	}
	else if (CurrentAlg==2) //Запуск возбуждения
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(0);
	    FPGA_EXTOUT_SetBit(1); //Ready
	    FPGA_EXTOUT_ClearBit(2); //Emergency
	    FPGA_EXTOUT_ClearBit(3); //Operation
	    FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs

		Alg_Excitation_limit = PRECHARGE_EXC_CURRENT;
		ActivateExcitation(1);
	    //Alg_Excitation_limit = 0; // TEMP!!!
	    //ActivateExcitation(0); // TEMP!!!


		SCADASystemState = SYSTEM_STATE_EXCITATION;
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		MinimumCoolingPWMWidth=5000;
		Alg_DCOutSetpoint = PRECHARGE_DC_VOLTAGE;

		CurrentAlg = 3; //К ожиданию возбуждения
		AlgCounter=0;
	}
	else if (CurrentAlg==3) //Ожидание предзаряда шины
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(0);
	    FPGA_EXTOUT_SetBit(1); //Ready
	    FPGA_EXTOUT_ClearBit(2); //Emergency
	    FPGA_EXTOUT_ClearBit(3); //Operation
	    FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs

		Alg_Excitation_limit = PRECHARGE_EXC_CURRENT;
		//Alg_Excitation_limit = 0; // TEMP!!!
		//CurrentAlg=4; // TEMP!!!

		SCADASystemState = SYSTEM_STATE_EXCITATION;
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		Alg_DCOutSetpoint = PRECHARGE_DC_VOLTAGE;
		AlgCounter++;
		MinimumCoolingPWMWidth=5000;
		if (AlgCounter>25000 && DCVoltage_OUT>50 && ExcitationCurrent>1) {CurrentAlg=4;} //К запуску повышающего преобразователя
		if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
	}
	else if (CurrentAlg==4) //Запуск повышающего преобразователя
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(0);
	    FPGA_EXTOUT_SetBit(1); //Ready
        FPGA_EXTOUT_ClearBit(2); //Emergency
        FPGA_EXTOUT_ClearBit(3); //Operation
        FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs

		Alg_Excitation_limit = Const_ExcitationMax;
		//Alg_Excitation_limit = 0; // TEMP!!!

		SCADASystemState = SYSTEM_STATE_EXCITATION;
		Alg_DCOutSetpoint = Alg_DCOutSetpoint + 0.1;
		if (Alg_DCOutSetpoint<PRECHARGE_DC_VOLTAGE) {Alg_DCOutSetpoint=PRECHARGE_DC_VOLTAGE;}
		if (Alg_DCOutSetpoint>NOMINAL_DC_VOLTAGE) {Alg_DCOutSetpoint=NOMINAL_DC_VOLTAGE;}
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		MinimumCoolingPWMWidth=10000;

		if (AlgCounter>25000 && DCVoltage_OUT>MINIMAL_BOOST_DC_VOLTAGE) {CurrentAlg = 5;AlgCounter=0;} //К запуску повышающего преобразователя
		if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}

		 //К ожиданию заряда шины постоянного тока

	}
	else if (CurrentAlg==5) //Ожидание заряда шины
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(0);
	    FPGA_EXTOUT_SetBit(1); //Ready
        FPGA_EXTOUT_ClearBit(2); //Emergency
        FPGA_EXTOUT_ClearBit(3); //Operation
        FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs

	    Alg_Excitation_limit = Const_ExcitationMax;
	    //Alg_Excitation_limit = 0; // TEMP!!!

		SCADASystemState = SYSTEM_STATE_EXCITATION;
		Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE;
		AlgCounter++;
		MinimumCoolingPWMWidth=10000;
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		if (AlgCounter>25000 && DCVoltage_OUT>MINIMAL_BOOST_DC_VOLTAGE) {CurrentAlg=6;AlgCounter=0;} //К запуску инвертора
		if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
	}
	else if (CurrentAlg==6) //Запуск инвертора
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(0);
	    FPGA_EXTOUT_SetBit(1); //Ready
        FPGA_EXTOUT_ClearBit(2); //Emergency
        FPGA_EXTOUT_ClearBit(3); //Operation
        FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs

		Alg_Excitation_limit = Const_ExcitationMax;
        //Alg_Excitation_limit = 0; // TEMP!!!


		Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE;
		SCADASystemState = SYSTEM_STATE_EXCITATION;
		MinimumCoolingPWMWidth=10000;
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		if (DCVoltage_OUT>MINIMAL_BOOST_DC_VOLTAGE) {AlgCounter++;}
		if (AlgCounter>25000)
		{
			if (DCVoltage_OUT>MINIMAL_BOOST_DC_VOLTAGE)
			{
			    TurnOnCommand = 1; CurrentAlg = 7;AlgCounter=0; TryToSyncCounter=0;
			}
		}
		if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
	}
	else if (CurrentAlg==7) //Включение Выключателя
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(1);
	    FPGA_EXTOUT_SetBit(1); //Ready
	    FPGA_EXTOUT_ClearBit(2); //Emergency
	    FPGA_EXTOUT_ClearBit(3); //Operation
	    FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs


		Alg_Excitation_limit = Const_ExcitationMax;
		//Alg_Excitation_limit = 0; // TEMP!!!

		AlgCounter++;
		Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE+100;
		SCADASystemState = SYSTEM_STATE_READYTOSYNC;
		MinimumCoolingPWMWidth=10000;
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		TurnOnCommand = 2;
		if (DCVoltage_OUT>MINIMAL_BOOST_DC_VOLTAGE)
		        {
		            ActivateBreakerControl(1);
		        }
		if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
		if (AlgCounter>35000 && TurnOnCommand==2)
		        {
		        if (OutputRMSVoltage < 30) { VoltagePID->ResetData(0,0.3);ActivateInverter(1);AlgCounter=0;CurrentAlg=9;TurnOnCommand=3;} //К включению на изолированную нагрузку
		        else if (OutputRMSVoltage > 160) {TurnOnCommand = 1; CurrentAlg = 8;AlgCounter=0; TryToSyncCounter=0;} //К включению на сеть
		        }
	}
	else if (CurrentAlg==8) //Включение на сеть, ожидание включения выключателя
	{
	    Const_OutputVoltageSetpoint = 190;
	    Const_MinOutputPower = 15000;
	    ActivateBreakerControl(1);
	    FPGA_EXTOUT_SetBit(1); //Ready
        FPGA_EXTOUT_ClearBit(2); //Emergency
        FPGA_EXTOUT_ClearBit(3); //Operation
        FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs

	    Alg_Excitation_limit = Const_ExcitationMax;
	    //Alg_Excitation_limit = 0; // TEMP!!!

		Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE+100;

		if (TurnOnCommand==1) {TurnOnCommand = 2; }
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		if (AlgCounter==0) {SCADASystemState = SYSTEM_STATE_READYTOSYNC;}
		if (AlgCounter>30000 && TurnOnCommand==2 && TryToSyncCounter==0 && DCVoltage_OUT>MINIMAL_BOOST_DC_VOLTAGE)
			{
			TryToSyncCounter = 1;
			TurnOnCommand=3;
			ActivateSynchronizer(1);
			SCADASystemState = SYSTEM_STATE_SYNC;
			TurnOnCommand = 0;
			}

		if (InverterActivated==1)
			{
			AlgCounter=0;CurrentAlg=10; //К работе на сеть
			}

		MinimumCoolingPWMWidth=10000;
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		AlgCounter++;
		if (AlgCounter>200000) {AlgCounter=200000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
	}
	else if (CurrentAlg==9) //Преобразователь в работе на изолированного потребителя
	{
	    Const_MinOutputPower = -30000;
	    FPGA_EXTOUT_SetBit(1); //Ready
        FPGA_EXTOUT_ClearBit(2); //Emergency
        FPGA_EXTOUT_SetBit(3); //Operation
        FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs
        Const_OutputVoltageSetpoint = 180;
		Alg_Excitation_limit = Const_ExcitationMax;
		//Alg_Excitation_limit = 0; // TEMP!!!

		Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE;
		SCADASystemState = SYSTEM_STATE_OPERATION_NORMAL;
		MinimumCoolingPWMWidth=10000;
		if (CurrentCommand==4) {CurrentAlg=20;} //К останову
		AlgCounter++;
		if (AlgCounter>100000) {AlgCounter=100000;}
	}
	else if (CurrentAlg==10) //Преобразователь в работе на сеть
		{
	        FPGA_EXTOUT_SetBit(1); //Ready
	        FPGA_EXTOUT_ClearBit(2); //Emergency
	        FPGA_EXTOUT_SetBit(3); //Operation

			Alg_Excitation_limit = Const_ExcitationMax;
			//Alg_Excitation_limit = 0; // TEMP!!!

			Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE+100;
			SCADASystemState = SYSTEM_STATE_OPERATION_NORMAL;
			MinimumCoolingPWMWidth=10000;
			if (CurrentCommand==4) {CurrentAlg=20;} //К останову
			AlgCounter++;
			if (AlgCounter>30000)
			{
			    FPGA_EXTOUT_SetBit(4);  //TurnOffPowerInputs
			    Const_OutputVoltageSetpoint = 180;
			}
			if (AlgCounter>35000)
			{
			    Const_MinOutputPower = -30000;
			    if (Alg_DCOutSetpoint>NOMINAL_DC_VOLTAGE+50) {Alg_DCOutSetpoint = Alg_DCOutSetpoint - 1;}
			    else {Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE+50;}
			}
			if (AlgCounter>100000) {AlgCounter=100000;}
		}
	else if (CurrentAlg==20) //Старт алгоритма останова
	{

	        FPGA_EXTOUT_ClearBit(1); //Ready
	        FPGA_EXTOUT_ClearBit(2); //Emergency
	        FPGA_EXTOUT_ClearBit(2); //Operation
	        FPGA_EXTOUT_ClearBit(2); //TurnOffPowerInputs

			Alg_DCOutSetpoint = NOMINAL_DC_VOLTAGE;
			SCADASystemState = SYSTEM_STATE_STOPPING;
			MinimumCoolingPWMWidth=10000;
			if (InverterActivated==1) {CurrentAlg=21; AlgCounter=0;}//Переход к остановке инвертора
			else if (ChopperActivated==1) {CurrentAlg=22; AlgCounter=0;} //Переход к отключению повышающего преобразователя
			else if (ExcitationActivated==1) {CurrentAlg=23; AlgCounter=0;} //Переход к отключению возбуждения
			else
			{
				//Переход к отключенному состоянию
			    Const_MinOutputPower = -10000;
				ActivateExcitation(0); ActivateChopper(0);ActivateInverter(0);
				CurrentAlg=1; AlgCounter=0;
				MinimumCoolingPWMWidth=0;
				ActivateBreakerControl(0);
				FPGA_EXTOUT_ClearBit(1); //Ready
				FPGA_EXTOUT_ClearBit(2); //Emergency
				FPGA_EXTOUT_ClearBit(3); //Operation
				FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs
			}
	}
	else if (CurrentAlg==21) //Остановка инвертора
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(0);
	    ActivateInverter(0);
	    FPGA_EXTOUT_ClearBit(1); //Ready
	    FPGA_EXTOUT_ClearBit(2); //Emergency
	    FPGA_EXTOUT_ClearBit(3); //Operation
	    FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs


	    Alg_Excitation_limit = 2;
	    //Alg_Excitation_limit = 0; // TEMP!!!

	    Alg_DCOutSetpoint = PRECHARGE_DC_VOLTAGE;
	    SCADASystemState = SYSTEM_STATE_STOPPING;
		MinimumCoolingPWMWidth=10000;
		AlgCounter++;
		if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
		if (AlgCounter>5000) {AlgCounter=0; CurrentAlg=22;} //Переход к отключению повышающего преобразователя
	}
	else if (CurrentAlg==22) //Отключение повышающего преобразователя
	{
	    Const_MinOutputPower = -10000;
	   ActivateBreakerControl(0);
       ActivateInverter(0);
       FPGA_EXTOUT_ClearBit(1); //Ready
       FPGA_EXTOUT_ClearBit(2); //Emergency
       FPGA_EXTOUT_ClearBit(3); //Operation
       FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs

       Alg_Excitation_limit = 2;
       //Alg_Excitation_limit = 0; // TEMP!!!

       Alg_DCOutSetpoint = PRECHARGE_DC_VOLTAGE;
       SCADASystemState = SYSTEM_STATE_STOPPING;
       MinimumCoolingPWMWidth=10000;
       AlgCounter++;
       if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
       SCADASystemState = SYSTEM_STATE_EXCITATION;
		ActivateChopper(0); CurrentAlg=23; AlgCounter=0; //Переход к отключению возбуждения
	}
	else if (CurrentAlg==23) //Отключение возбуждения
	{
	    Const_MinOutputPower = -10000;
	    ActivateBreakerControl(0);
	    ActivateInverter(0);
	    ActivateExcitation(0);
	    ActivateChopper(0);
	    FPGA_EXTOUT_ClearBit(1); //Ready
	    FPGA_EXTOUT_ClearBit(2); //Emergency
	    FPGA_EXTOUT_ClearBit(3); //Operation
	    FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs
	    Alg_Excitation_limit = 2;
	    Alg_DCOutSetpoint = PRECHARGE_DC_VOLTAGE;
	    MinimumCoolingPWMWidth=10000;
	    Alg_Excitation_limit = PRECHARGE_EXC_CURRENT;
		Alg_DCOutSetpoint = 0;
		SCADASystemState = SYSTEM_STATE_EXCITATION;
		ActivateInverter(0);
		MinimumCoolingPWMWidth=1000;
		AlgCounter++;
		if (ExcitationCurrent<1 || AlgCounter>50000) //Переход к остановленному состоянию
		{
			ActivateExcitation(0); ActivateChopper(0);ActivateInverter(0);
			CurrentAlg=1; AlgCounter=0;
			MinimumCoolingPWMWidth=0;
		}
		if (AlgCounter>100000) {AlgCounter=100000;ErrorSet(ERR_EMERGENCY_STOP_ALG);}
	}
	else
	{
	    Const_MinOutputPower = -10000;
	    FPGA_EXTOUT_ClearBit(1); //Ready
	    FPGA_EXTOUT_SetBit(2); //Emergency
	    FPGA_EXTOUT_ClearBit(3); //Operation
	    FPGA_EXTOUT_ClearBit(4); //TurnOffPowerInputs
		Alg_Excitation_limit = 0;
		Alg_DCOutSetpoint = 0;
		SCADASystemState = SYSTEM_STATE_EMERGENCY;
		ErrorSet(ERR_UNEXPECTED);
		CurrentAlg=0;
		ActivateBreakerControl(0);
		ActivateInverter(0);
		ActivateExcitation(0);
		ActivateChopper(0);
	}

	if (CurrentAlg!=0)
		{
		if (InverterActivated==0 && ExcitationActivated==0 && ChopperActivated==0) CurrentSysytemState=1;
		if (InverterActivated==0 && (ExcitationActivated==1 || ChopperActivated==1)) CurrentSysytemState=2;
		if (InverterActivated==1 && (ExcitationActivated==1 || ChopperActivated==1)) CurrentSysytemState=3;
		}

	if (SystemManualControl==0)
	{
		ExcitationManualControl=0;
		ChopperManualControl=0;
		InverterVoltageManualControl=0;
		InverterFrequencyManualControl=0;
		VentilationManualControl=0;
	}

	if (CurrentAlg!=8 || DCVoltage_OUT<MINIMAL_DC_VOLTAGE)
	{
	ActivateSynchronizer(0);
	}

	if (CurrentAlg!=7 && CurrentAlg!=8)
	{
		TurnOnCommand = 0;
	}

	if (InverterActivated!=0)
	{
		InverterActivatedCounts++;
		if (InverterActivatedCounts>30000)
		{
			InverterActivatedCounts = 30000;
		}
	}
	else {InverterActivatedCounts=0;}


}



