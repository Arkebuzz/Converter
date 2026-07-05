#include "ErrorList.h"
#include "InitFunctions.h"
#include "MemFunctions.h"
#include "DataTransfer.h"
#include "GlobalData.h"
#include "FPGA_DataAddresses.h"

void main(void) {
	//GpioG2DataRegs.GPEDAT.bit.GPIO134 = 0; //Remove system OK flag to FPGA
	GpioG1DataRegs.GPADAT.bit.GPIO0 = 0; 	 //Remove system OK flag to FPGA

	// Стандартная инициализация
	InitSysCtrl();  // Инициализация System Control
	InitGpio();     // Инициализация GPIO

	INIT_GPIO_Setup();  // Настройка портов GPIO
	Init_SPI(); 		// Настройка flash, по факту не используем

#ifdef _FLASH    // Step 4. Copy time critical code and Flash setup code to RAM
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t) &RamfuncsLoadSize);
	InitFlash();
#endif

	// Инициализация системы прерываний
	InitPieCtrl(); 		// Initialize the PIE control registers to their default state.
	IER = 0x0000; 		// Disable CPU interrupts and clear all CPU interrupt flags
	IFR = 0x0000; 		// Disable CPU interrupts and clear all CPU interrupt flags
	InitPieVectTable(); // Initialize the PIE vector table with pointers to the shell ISR.

	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM

	//ErrorSet(ERR_EMERGENCY_STOP_ALG);  Исходно стартуем с прерывания, чтобы безопасно все было, когда систему ошибок поправим, вернем

	// Запуска таймера на 300 мкс для главного цикла
	INIT_Setup_Timers(MAIN_CYCLE_US, C28_FREQ);
    INIT_Start_Timers();

    // Проверка запуска М3
	CtoMIpcRegs.CTOMIPCSET.bit.IPC1 = 1; 		   // Send data ready signal
	while (CtoMIpcRegs.CTOMIPCFLG.bit.IPC1 != 0);  // Wait for M3 to read init data

	// Настройка DMA
	// DMASourceFPGA - память на FPGA, DMADestFPGA - локальная копия
	INIT_Setup_DMA(&DMABufFPGA[0], 51);
	DMABufFPGA[ADR_WATCHDOG] = 0;

	Uint32 LedCounter = 0;

    for(;;) {  // Итерации раз в 300 мкс
    	if (DmaRegs.CH1.CONTROL.bit.TRANSFERSTS) {
    		continue;  // Ждём пока DMA считает всё с FPGA
    	}

        // Моргаем светодиодиком
    	LedCounter++;
    	if (LedCounter == 500) {
    		GpioG1DataRegs.GPADAT.bit.GPIO8 = 0;
    	}
    	else if (LedCounter > 1000) {
    		GpioG1DataRegs.GPADAT.bit.GPIO8 = 1;
    		LedCounter = 0;
    	}

    	ReadFPGAData();  // Чтение с FPGA в глобальные переменные

    	WriteOSCI_S6S7();  // Взаимодействие с М3

    	WriteFPGAData();  // Запись в FPGA

    	EALLOW;
		DmaRegs.CH1.CONTROL.bit.PERINTFRC = 1;  // DMA запуск получения значений
		EDIS;

    	CPU_OverloadFlag = 1;
    	Uint16 WaitCyclesCounter = 0;
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
		CpuTimer1Regs.TCR.bit.TIF = 1; // Перезапуск таймера

		CyclesCounter++;
    } //MAIN CYCLE END
}
