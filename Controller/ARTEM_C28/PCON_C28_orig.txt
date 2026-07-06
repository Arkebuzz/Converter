#include "F28M35x_Device.h"
#include "F28M35x_Examples.h"
#include "F28M35x_Ipc.h"

// Shared memory buffer
// telling the compiler to put this variable into the specific section
// this only takes section 0 of 0-7 sections
// look into .cmd file to see the layout
#pragma DATA_SECTION(shared_mem, "SHARERAMS0")
volatile Uint16 shared_mem[0x1000];


// Currently we are using Flash memory to store the code
inline void setup_flash_mem(void) {
    #ifdef _FLASH
        // Copy the ramfuncs from Flash to RAM
        memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

        // Initialize the Flash controller
        // This should be in RAM while executing otherwise flash is fried
        InitFlash();
    #endif
}

// Original code is in InitFunctions.c
void setup_GPIO(void) {
    EALLOW;
    // Sets GPIO128 (pin 140 not connected) and GPIO19 (pin 29 = EPWM6A) to output
    GpioG2CtrlRegs.GPEDIR.bit.GPIO128 = 1;
    GpioG1CtrlRegs.GPADIR.bit.GPIO19 = 1;
    EDIS;
}

void setup_SPI(void) {
    // TODO
}

void main(void) {
    // --- CONFIGURING THE CHIP ---

    // Disable all interrupts while configuring
    DINT;

    setup_flash_mem();

    InitSysCtrl();      // wakes up the CPU and the clocks
    InitGpio();

    setup_GPIO();       // setup GPIO pins
    setup_SPI();        // setup SPI pins

    // Configuring PIE (Peripheral Interrupt Expansion)

    InitPieCtrl(); // Обнуляет все биты у 12 групп (INT1-INT12)
    IER = 0x0000;  // Обнуление битов для регистра, разрешающего включения и выключения прерывания
    IFR = 0x0000;  // Обнуление регистра, отвечающего за нахождения определенного прерывания в очереди

    InitPieVectTable();  // Создание таблицы групп для адресации прерываний (таблица копируется из FLASH памяти в RAM. В ней хранятся адреса функций, обрабатывающих прерывания)

    EALLOW;                                   // Открытия доступа к некоторой группе особо важных регистров (всё в PIE table защищены EALLOW)
    PieVectTable.ADCINT1 = (PINT)(&adc1_isr); // Записывает адрес функции прерывания ADC в таблицу PIE_VECT_TABLE PieVectTable в ADCINT1
    EDIS;									  // Закрытие доступа к регистрам из группы EALLOW


    INIT_Setup_Timers(ADC_SAMPLE_CYCLE_US, MAIN_CYCLE_US, C28_FREQ);  // ADC_SAMPLE_CYCLE_US - требуемый период срабатывания АЦП
    																  // MAIN_CYCLE_US - требуемый период срабатывания главного цикла
                                                                      // C28_FREQ - частота CPU
    																  // Конфигурация Timer 1, нужен судя по всему для цикла for(;;)


    // Включение ADCINT1 в PIE
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // бит INTx1 соответствует прерыванию ADCINT1 в PIE
    IER |= M_INT1;                      // устанавливает 1 бит для разрешения прерываний уже на уровне ядра
    EINT;  								// Врубаем прием маскируемых прерываний
    ERTM;  								// ой че то для отладки, если при остановке кода DBGM = 1 - прерывания игнорируются, если DBGM = 0 - нет (пусть будет пока что)

    ErrorSet(ERR_EMERGENCY_STOP_ALG);   // какие-то ошипки, пусть будут тоже

    INIT_Setup_Timers(ADC_SAMPLE_CYCLE_US, MAIN_CYCLE_US, C28_FREQ);  // Второй раз настраиваем таймер. В первый раз мы настроили, но прерывание не разрешили,
                                                                      // еще и мусор убрали. После разрешения прерываний и настройки обработки ошибок можно уже реально пользоваться таймером.

    INIT_StartTimers(); // Для таймера: сброс флагов прерывания, разрешение прерывания, перезагрузка счетчика, запуск таймера



	CtoMIpcRegs.CTOMIPCSET.bit.IPC1 = 1;                // бит связи с M3
	while (CtoMIpcRegs.CTOMIPCFLG.bit.IPC1 != 0) {k++;} // ожидание сброса IPC1 в 0, по идее после получения данных

	INIT_Setup_DMA(&DMABufFPGA[0], 51);  // Какщьки из оригинального файла. Связывает локальный массив DMABufFPGA с некоторой областью внешней памяти, откуда будет читать
									     // данные размером в 50 слов. Как только такой буфер в 50 слов кончается, возврат к началу + настройка режима работы. Одно слово 16 бит.

	DMABufFPGA[ADR_WATCHDOG] = 0;		 // обнулили 50 элемент, шобы потом не было ошибки при первом сравнении в ReadFPGAData 👍

	Uint32 LedCounter = 0;               // без комментариев

  EALLOW;
  DmaRegs.CH1.CONTROL.bit.PERINTFRC = 1;  // DMA запуск получения значений
  EDIS;

  while (DmaRegs.CH1.CONTROL.bit.TRANSFERSTS){}   // ожидание завершения передачи DMA

	    for(;;) {

        ReadFPGAData();  // Проверка связи с FPGA и пересчет тока и напряжения, запись в глобальные переменные

    		EALLOW;
    		DmaRegs.CH1.CONTROL.bit.PERINTFRC = 1;  // DMA запуск получения значений
    		EDIS;
    
    	  while (DmaRegs.CH1.CONTROL.bit.TRANSFERSTS){}   // ожидание завершения передачи DMA
    
    		// Моргаем светодиодиком 👍
    		LedCounter++;
    		if (LedCounter == 500) {
    			GpioG1DataRegs.GPADAT.bit.GPIO8 = 0;
    		}
    		else if (LedCounter > 1000) {
    			GpioG1DataRegs.GPADAT.bit.GPIO8 = 1;
    			LedCounter = 0;
    		}
    
    		WriteOSCI_S6S7();  // Передача данных в М3, доделать Dest, а то не работает пока
    
    		WriteFPGAData();  // Запись в FPGA
    
    
        Uint16 WaitCyclesCounter = 0;              // если что завершает с ошибкой цикл
    		while (CpuTimer1Regs.TCR.bit.TIF == 0) {   // Ждем до 300 мкс
    			WaitCyclesCounter++;
    			if (WaitCyclesCounter > 30000)
    				ErrorSet(ERROR_MAIN_TIMER_DEAD);
    		}
        
    		CpuTimer1Regs.TCR.bit.TIF = 1; // Перезапуск таймера
    		CyclesCounter++;
    
    	}

}
