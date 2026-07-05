#include "ErrorList.h"
#include "InitFunctions.h"
#include "MemFunctions.h"
#include "GlobalData.h"


void main(void) {
	//GpioG2DataRegs.GPEDAT.bit.GPIO134 = 0; //Remove system OK flag to FPGA
	GpioG1DataRegs.GPADAT.bit.GPIO0 = 0; 	 //Remove system OK flag to FPGA

	InitSysCtrl(); // Step. Initialize System Control
	InitGpio(); // Step. Initialize GPIO
	INIT_GPIO_Setup(); //Setup GPIO pins
	Init_SPI(); //Setup SPI pins

#ifdef _FLASH    // Step 4. Copy time critical code and Flash setup code to RAM
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t) &RamfuncsLoadSize);
	InitFlash();
#endif

	// Инициализация системы прерываний
	DINT;  // Step. Clear all interrupts and initialize PIE vector table: Disable CPU interrupts

	InitPieCtrl(); // Initialize the PIE control registers to their default state.
	IER = 0x0000; // Disable CPU interrupts and clear all CPU interrupt flags
	IFR = 0x0000; // Disable CPU interrupts and clear all CPU interrupt flags
	InitPieVectTable(); // Initialize the PIE vector table with pointers to the shell ISR.

	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM

	//ErrorSet(ERR_EMERGENCY_STOP_ALG);  Исходно стартуем с прерывания, чтобы безопасно все было, когда систему ошибок поправим, вернем

	//Start system timers
	INIT_Setup_Timers(MAIN_CYCLE_US, C28_FREQ); //Setup Timers
    INIT_Start_Timers(); //Start timers operation

    // Проверка запуска М3
	CtoMIpcRegs.CTOMIPCSET.bit.IPC1 = 1; //Send data ready signal
	while (CtoMIpcRegs.CTOMIPCFLG.bit.IPC1 != 0); //Wait for M3 to read init data.

	// Настройка DMA
	// DMASourceFPGA - память на FPGA, DMADestFPGA - локальная копия
	INIT_Setup_DMA(&DMABufFPGA[0], 51);

	// Я НАСРАЛ:
	const Uint16 READY_TO_CALC_MEANS = 50;
	float buffer_data[4] = {0, 0, 0, 0};
	Uint16 buffer_counter = 0;
	float current_1, current_2, voltage_inp, voltage_out;
	Uint8 WatchDog = 0;
	DMABufFPGA[50] = 0;

	Uint32 LedCounter = 0;

    for(;;) {  // Итерации раз в 300 мкс

        // Моргаем светодиодиком
    	LedCounter++;
    	if (LedCounter == 500) {
    		GpioG1DataRegs.GPADAT.bit.GPIO8 = 0;
    	}
    	else if (LedCounter > 1000) {
    		GpioG1DataRegs.GPADAT.bit.GPIO8 = 1;
    		LedCounter = 0;
    	}

    	if (WatchDog != DMABufFPGA[50]) {
    		ErrorSet(ERROR_FPGA_CONNECT_FAIL);
    	}

    	WriteWordTo_FPGA(50, ++WatchDog);
    	WriteWordTo_FPGA(55, 398);  // const_4

    	Uint16 ErrorsFPGA = DMABufFPGA[1];
    	WriteTo_CTOM_MSGRAM(2, ErrorsFPGA);

    	Uint16 const_1 = DMABufFPGA[5];
    	Uint16 const_2 = DMABufFPGA[6];
    	Uint16 const_3 = DMABufFPGA[7];
    	Uint16 const_4 = DMABufFPGA[8];

    	buffer_data[0] += ((float)DMABufFPGA[10] - 2070.0) * 1.271565755;   // voltage_inp
    	buffer_data[1] += ((float)DMABufFPGA[11] - 2070.0) * 1.271565755;   // voltage_out
    	buffer_data[2] += ((float)DMABufFPGA[12] - 1706.66) * 0.366210938;  // current_1
    	buffer_data[3] += ((float)DMABufFPGA[13] - 1706.66) * 0.366210938;  // current_2

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

    } //MAIN CYCLE END
}
