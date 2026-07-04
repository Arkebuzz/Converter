#include <GlobalData.h>
#include <ERROR_LIST.h>


void ErrorSet(Uint32 ErrNum) {
	WriteTo_CTOM_MSGRAM(1, ErrNum);
}


void main(void)
{
   Uint32 LedCounter=0;

   SCADASystemState = SYSTEM_STATE_INIT;
   //GpioG2DataRegs.GPEDAT.bit.GPIO134 = 0; //Remove system OK flag to FPGA
   GpioG1DataRegs.GPADAT.bit.GPIO0 = 0; //Remove system OK flag to FPGA

   InitSysCtrl(); // Step. Initialize System Control
   InitGpio(); // Step. Initialize GPIO
   INIT_GPIO_Setup(); //Setup GPIO pins
   Init_SPI(); //Setup SPI pins

   DINT; // Step. Clear all interrupts and initialize PIE vector table: Disable CPU interrupts

   #ifdef _FLASH    // Step 4. Copy time critical code and Flash setup code to RAM
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    InitFlash();
   #endif
    // Step. Interrupt Initialization
    InitPieCtrl(); // Initialize the PIE control registers to their default state.
    IER = 0x0000; // Disable CPU interrupts and clear all CPU interrupt flags
    IFR = 0x0000; // Disable CPU interrupts and clear all CPU interrupt flags
    InitPieVectTable(); // Initialize the PIE vector table with pointers to the shell ISR.
    EALLOW; // Write to EALLOW protected register
    PieVectTable.ADCINT1 = (PINT)(&adc1_isr);  // Interrupt are re-mapped to ISR functions
    EDIS;   // Disable write to EALLOW protected registers

    INIT_Peripherals(); //Initialize Device Peripherals
    INIT_Setup_Timers(ADC_SAMPLE_CYCLE_US, MAIN_CYCLE_US, C28_FREQ);  //Setup Timers
    INIT_Setup_ADC(); //Setup ADC conversion

    // Enable ADCINT1 in PIE
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // Enable INT 1.1 in the PIE
    IER |= M_INT1;                      // Enable CPU Interrupt 1
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    //Start system timers
    //ErrorSet(ERR_EMERGENCY_STOP_ALG);

    INIT_Setup_Timers(ADC_SAMPLE_CYCLE_US, MAIN_CYCLE_US, C28_FREQ);  //Setup Timers
    INIT_StartTimers(); //Start timers operation

    //Read init data from FLASH, init parameters, copy data for M3, message M3
    Setups_Init(); //Assign setups pointers for data load

  /*  while (ReadResult!=1)
    {
    	if (LED==0) {GpioG1DataRegs.GPADAT.bit.GPIO4 = 1; LED=1;}
    	else {GpioG1DataRegs.GPADAT.bit.GPIO4 = 0; LED=0;}
    	ReadResult = ReadDataFromFlash(); //Read data from flash
    	if (ReadResult!=1) {CtoMIpcRegs.CTOMIPCSET.bit.IPC2 = 1;}
    	if (CtoMIpcRegs.MTOCIPCSTS.bit.IPC2 == 1)  	{SaveDataToFlash(); CtoMIpcRegs.MTOCIPCACK.bit.IPC2 = 1;}
    } //Abort loading if data corrupted
  */
    InitData(); //Init structures with setup parameters

    // Вот это надо:
	CtoMIpcRegs.CTOMIPCSET.bit.IPC1 = 1; //Send data ready signal
	while (CtoMIpcRegs.CTOMIPCFLG.bit.IPC1 != 0); //Wait for M3 to read init data.

	OsciChannelsInit(); //Init links to osci data


	//DMA setup
	// Initialize the DMA
	// DMA Source is buffer 2, destination is buffer 1
	DMADestFPGA = &DMABufFPGA1[0];
	DMASourceFPGA = (Uint16 *)(0x340000);
	DMACH1AddrConfig(DMADestFPGA,DMASourceFPGA);

	// Set up to use 16-bit data size
	// Pointers are based on 16-bit words
	// Increment by 1 (1 16-bit words)
	//Uint16 DataSize = 51;
	DMACH1BurstConfig(0, 0, 0);
	DMACH1TransferConfig(50, 1, 1);
	DMACH1WrapConfig(50, 0, 50, 0);

	// This is a static copy use one shot mode, so only one trigger is needed
	// Use 16-bit mode
	// Enable the PIE interrupt for the DMA channel
	DMACH1ModeConfig(0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,
					 SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,
					 CHINT_END,
					 CHINT_ENABLE);
	StartDMACH1();

	// Я НАСРАЛ:
	const Uint16 READY_TO_CALC_MEANS = 50;
	float buffer_data[4] = {0, 0, 0, 0};
	Uint16 buffer_counter = 0;
	float current_1, current_2, voltage_inp, voltage_out;
	Uint8 WatchDog = 0;
	DMABufFPGA1[50] = 0;

// Step. Main loop.
    for(;;) {
    	// Cycle initialization, blink and WD signals toggle---------------------
    	///MainCyclesCounter++;
    	//CalcCycleCounter++;
    	//if (CalcCycleCounter>3) {CalcCycleCounter=0;}

    	// Circular buffer counter reload
        //if(MainCycleMeasurmentsCounter == (MEASURMENTS_BUFFER_SIZE - 1)) {MainCycleMeasurmentsCounter = 0;}
    	//else MainCycleMeasurmentsCounter++;

    	//WatchdogValue=1; //GpioG2DataRegs.GPEDAT.bit.GPIO128 = WatchdogValue; // Watchdog out

        // Моргаем светодиодиком
    	LedCounter++;
    	if (LedCounter == 500) {
    		GpioG1DataRegs.GPADAT.bit.GPIO8 = 0;
    	}
    	else if (LedCounter > 1000) {
    		GpioG1DataRegs.GPADAT.bit.GPIO8 = 1;
    		LedCounter = 0;
    	}

    	// ----------------------------------------------------------------------

    	//Values calculations, data processing-----------------------------------
    	//if (RecalcOffset>0)  {if (RecalcOffset>1){MeanOffsetCalc();}  if (RecalcOffset>0) {RecalcOffset++;}} //Offset recalculation

    	//  M3_Read_Data();  // Read data from M3

    	// M3_Get_Setups(); // Check if new setup data incoming.
    	// CalculateValues(); //Sensor values calculation according sensors gains
    	// FPGA_Read_Data(); //Read data from FPGA

    	if (WatchDog != DMABufFPGA1[50]) {
    		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
    	}

    	WriteWordTo_FPGA(50, ++WatchDog);
    	WriteWordTo_FPGA(55, 398);  // const_4

    	Uint16 ErrorsFPGA = DMABufFPGA1[1];
    	WriteTo_CTOM_MSGRAM(2, ErrorsFPGA);

    	Uint16 const_1 = DMABufFPGA1[5];
    	Uint16 const_2 = DMABufFPGA1[6];
    	Uint16 const_3 = DMABufFPGA1[7];
    	Uint16 const_4 = DMABufFPGA1[8];

    	buffer_data[0] += ((float)DMABufFPGA1[10] - 2070.0) * 1.271565755;   // voltage_inp
    	buffer_data[1] += ((float)DMABufFPGA1[11] - 2070.0) * 1.271565755;   // voltage_out
    	buffer_data[2] += ((float)DMABufFPGA1[12] - 1706.66) * 0.366210938;  // current_1
    	buffer_data[3] += ((float)DMABufFPGA1[13] - 1706.66) * 0.366210938;  // current_2

    	if (buffer_counter >= READY_TO_CALC_MEANS) {
    		buffer_counter = 0;

    		// Расчет среднего:
    		voltage_inp = buffer_data[0] / READY_TO_CALC_MEANS;
    		voltage_out = buffer_data[1] / READY_TO_CALC_MEANS;
    		current_1   = buffer_data[2] / READY_TO_CALC_MEANS;
    		current_2   = buffer_data[3] / READY_TO_CALC_MEANS;

    		// Отправка на M3:
    		WriteTo_CTOM_MSGRAM_Float(100, voltage_inp);
    		WriteTo_CTOM_MSGRAM_Float(101, voltage_out);
    		WriteTo_CTOM_MSGRAM_Float(102, current_1);
    		WriteTo_CTOM_MSGRAM_Float(103, current_2);
    		WriteTo_CTOM_MSGRAM(104, const_1);
    		WriteTo_CTOM_MSGRAM(105, const_2);
    		WriteTo_CTOM_MSGRAM(106, const_3);
    		WriteTo_CTOM_MSGRAM(107, const_4);
    		WriteTo_CTOM_MSGRAM(108, FreeTimeCounter);

    		buffer_data[0] = 0;
    		buffer_data[1] = 0;
    		buffer_data[2] = 0;
    		buffer_data[3] = 0;
    	} else {
    		buffer_counter += 1;
    	}

    	EALLOW;
    	if (DmaRegs.CH1.CONTROL.bit.TRANSFERSTS != 0) {
    		ErrorSet(ERROR_DMA_OVERRIDE);
    	}
		DmaRegs.CH1.CONTROL.bit.PERINTFRC = 1;  // DMA CH1 запуск получения значений (молимся, что на следующей итерации будут новые значения)
		EDIS;

    	CPU_OverloadFlag = 1;
		WaitCyclesCounter = 0;
		// Ждем до 300 мкс (одна итерация 300 мкс)
		while (CpuTimer1Regs.TCR.bit.TIF == 0) {
			FreeTimeCounter = CpuTimer1Regs.TIM.all;
			CPU_OverloadFlag = 0;
			WaitCyclesCounter++;
			if (WaitCyclesCounter > 30000) {
				ErrorSet(ERROR_MAIN_TIMER_DEAD);
			}
		}
		if (CPU_OverloadFlag != 0) {
			ErrorSet(ERROR_CPU_OVERLOAD);
		}
		CpuTimer1Regs.TCR.bit.TIF = 1; //Reset interrupt flag

/*
    	// SignalProcessing(); //Signal processing - calculating Mean/RMS values, filtering
    	//CalculatePower(); //Active, reactive power calculation
    	//RotationSpeedMeasurments(); //Rotation speed measurements / zero cross detection
    	//CalcFundamentalValues(); //Phase and frequency calculations for synchronization
    	ProtectionsCheck(); //Check values for safety operation
    	CommandsProcess(); //State machine operation, processing commands from M3 (SCADA)

    	// ----------------------------------------------------------------------
    	// Regulation and control -----------------------------------------------
    	//SynchronizerOperation(); //Synchronizer operation
    	ExcitationRegulation(); //Control and regulation of excitation PWM
    	//DCBoostRegulation(); //Control and regulation of boost chopper PWM
    	//RotationSpeedRegulation(); //Control and regulation of generator speed by active power load
    	UnitLoadRegulation(); //Control and regulation of unit load dependent of output frequency
    	ActivePowerRegulation(); //Control and regulation of output active power by PMW phase and frequency shift
    	ReactivePowerRegulation(); //Control and regulation of output reactive power by PWM amp
    	CoolingControl(); //Control of cooling systems depending on operation parameters
    	// ----------------------------------------------------------------------

    	StateRecognition(); //State recognition depending on operation parameters

    	DMADataTransferSet(); //Trasmitt oscillogramms data to M3 core

    	FPGA_Write_Aux(); //---------------------FPGA WRITE------------------
    	FPGA_Write_Booster(); //---------------------FPGA WRITE------------------
    	FPGA_Write_Exitation(); //---------------------FPGA WRITE------------------
    	FPGA_Write_Phase_Freq(); //---------------------FPGA WRITE------------------
    	FPGA_Write_Amp(); //---------------------FPGA WRITE------------------
    	FPGA_Write_Data(); //Write data to FPGA //---------------------FPGA WRITE------------------

    	//FPGA_Write_DMA(); // new FPGA write ----------------
    	M3_Write_Data(MainCyclesCounter); //Transmit general data to M3 core using shmem
    	// Cycle end processing. Timers reload. Cycle time measurement ------------------------------
    	if (SetTryToResetError==1) {SetTryToResetError=0;} //Clear error reset flag
    	//else if (SetTryToResetError!=0) {SetTryToResetError++;}
    	WorkingCycleTime_uS = MAIN_CYCLE_US - CpuTimer1Regs.TIM.all; //Measure time elapsed from timer INT
    	Peak_ResetCounter++; if (Peak_ResetCounter > 100000) {Peak_WorkingCycleTime_uS = 0;Peak_ResetCounter=0;}
    	if (WorkingCycleTime_uS > Peak_WorkingCycleTime_uS) {Peak_WorkingCycleTime_uS = WorkingCycleTime_uS;}
    	if (MeasurmentsCounter!= PrevMeasurmentCounterValue) {PrevMeasurmentCounterValue = MeasurmentsCounter;}
    	else {ErrorSet(ERR_ADC_IRQ_LOSS);}

    	//WatchdogValue=0; //GpioG2DataRegs.GPEDAT.bit.GPIO128 = WatchdogValue; // Watchdog out

    	if (SaveDataRequestFlag==1) {SaveDataToFlash();	SaveDataRequestFlag=2;}
		if (SaveDataRequestFlag==3)	{EraseFlashData(); SaveDataRequestFlag=4;}

    	CPU_OverloadFlag = 1; WaitCyclesCounter = 0;
    	while (CpuTimer1Regs.TCR.bit.TIF==0)
    	{
    		FreeTimeCounter=CpuTimer1Regs.TIM.all;
    		CPU_OverloadFlag=0;
    		WaitCyclesCounter++;
    		if (WaitCyclesCounter>(30000)) {ErrorSet(ERR_MAIN_IRQ_LOSS);}
    	}
    	if (CPU_OverloadFlag!=0) {ErrorSet(ERR_CPU_OVERLOAD);}
    	CpuTimer1Regs.TCR.bit.TIF = 1; //Reset interrupt flag
    	// ---------------------------------------------------------------------------------------------
*/
    } //MAIN CYCLE END
}



