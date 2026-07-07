#include "communication_M3.h"
#include "error_handling.h"

// IVAN: docs say 0x400, linker file says 0x380 :/
#pragma DATA_SECTION(CTOM_MSGRAM, "CTOM_MSGRAM")
volatile Uint16 CTOM_MSGRAM[0x380];

#pragma DATA_SECTION(MTOC_MSGRAM, "MTOC_MSGRAM")
volatile Uint16 MTOC_MSGRAM[0x380];

//// IVAN: data size IN 16 BIT WORDS
//Uint16 CTOM_Data[100];
//Uint16 MTOC_Data[100];

// Communication with M3 uses SRAM6-SRAM7
#pragma DATA_SECTION(SHARERAMS6, "SHARERAMS6")
volatile Uint16 SHARERAMS6[0x1000];

#pragma DATA_SECTION(SHARERAMS7, "SHARERAMS7")
volatile Uint16 SHARERAMS7[0x1000];

#define S6_START SHARERAMS6
#define S7_END   (&SHARERAMS7[sizeof(SHARERAMS7) / sizeof(SHARERAMS7[0])])

void WriteToM3Data(const DataToM3 Data) {
	Uint16 *CTOM_ptr = (Uint16 *)CTOM_MSGRAM;
	*CTOM_ptr = Data.C28_Errors;		CTOM_ptr++;
	*CTOM_ptr = Data.C28_Errors_Latch;	CTOM_ptr++;
	*CTOM_ptr = Data.FPGA_Errors;		CTOM_ptr++;
	*CTOM_ptr = Data.FPGA_Errors_Latch;	CTOM_ptr++;

	// Продублируем ошибки в CtoM:
//	WriteTo_CTOM_MSGRAM(0, Data.C28_Errors);
//	WriteTo_CTOM_MSGRAM(1, Data.C28_Errors_Latch);
//	WriteTo_CTOM_MSGRAM(2, Data.FPGA_Errors);
//	WriteTo_CTOM_MSGRAM(3, Data.FPGA_Errors_Latch);

	// АРТЕМ:
	// Отправляем сигналы на M3 при заполнении пакетов.
	// Имеем 8192 слова, пакеты по 1024 слова => 8 пакетов.
	// В 1 пакете 64 набора данных с 64 моментов времени (64 итераций).
	// Набор данных - 16 слов (берем с запасом) - результат одного цикла C28.

	// ИВАН:
	// 8 пакетов - буфер сдвига на 8 пакетов (по 1024 слов)
	// Используем 8 регистров IPCN (inter processor communication)
	//   - 1 на бите N означает что данные N пакета еще не считаны М3 =>
	//   посылаем ошибку ERROR_OSCI_BUF_OVERFLOW
	//   - 0 на бите N означает что М3 считал этот пакет и поставил обратно 0 =>
	//   мы можем свободно перезаписывать этот пакет
	// В каждом пакете 64 замера (чтобы 1 пакет заполнился эта функция должна быть вызвана 64 раза)

	static Uint8 NumPacket = 0;
	static Uint8 NumIter = 0;
	static volatile Uint16 *SRAM_ptr = (Uint16 *)S6_START;

	if (NumIter == 64) {
		// IVAN: Пакет заполнился

		*CTOM_ptr = NumPacket;
//		WriteTo_CTOM_MSGRAM(10, NumPacket);  // Информируем М3 о готовности пакета

		NumIter = 0; // IVAN: сбрасываем кол-во отправленых замеров в текущем пакете
		NumPacket++; // сдвигаем запись на след пакет
	}
	if (NumPacket == 8) {
		// IVAN: все пакеты заполнились => зацикливаемся на 0 пакет
		NumPacket = 0;
		SRAM_ptr = (Uint16 *)S6_START;
	}

	// IVAN: CycleCounter размером 64 бита, слово у нас 16 бит
	// поэтому разбиваем его на 4 части
//	Uint16 CyclesCounter0 = (Uint16) ((Data.CycleCounter & 0x000000000000FFFF) >> 0);
//	Uint16 CyclesCounter1 = (Uint16) ((Data.CycleCounter & 0x00000000FFFF0000) >> 16);
//	Uint16 CyclesCounter2 = (Uint16) ((Data.CycleCounter & 0x0000FFFF00000000) >> 32);
//	Uint16 CyclesCounter3 = (Uint16) ((Data.CycleCounter & 0xFFFF000000000000) >> 48);

	Uint16 *CyclesCounter = (Uint16 *)Data.CycleCounter;

	*SRAM_ptr = CyclesCounter[0]; 		SRAM_ptr++;  // 0
	*SRAM_ptr = CyclesCounter[1]; 		SRAM_ptr++;  // 1
	*SRAM_ptr = CyclesCounter[2]; 		SRAM_ptr++;  // 2
	*SRAM_ptr = CyclesCounter[3]; 		SRAM_ptr++;  // 3
	*SRAM_ptr = Data.C28_Errors;		SRAM_ptr++;  // 4
	*SRAM_ptr = Data.C28_Errors_Latch;  SRAM_ptr++;  // 5
	*SRAM_ptr = Data.FPGA_Errors; 		SRAM_ptr++;  // 6
	*SRAM_ptr = Data.FPGA_Errors_Latch; SRAM_ptr++;  // 7
	*SRAM_ptr = Data.Voltage_Inp;		SRAM_ptr++;  // 8
	*SRAM_ptr = Data.Voltage_Out;		SRAM_ptr++;  // 9
	*SRAM_ptr = Data.Current_1;			SRAM_ptr++;  // 10
	*SRAM_ptr = Data.Current_2;			SRAM_ptr++;  // 11
	*SRAM_ptr = Data.WatchDog;			SRAM_ptr++;  // 12
	*SRAM_ptr = Data.FreeTimeCounter;	SRAM_ptr++;  // 13
	SRAM_ptr++;  // 14
	SRAM_ptr++;  // 15

	// IVAN:  Записываем 14 слов за один замер
	// ARTEM: Для простоты деления памяти на блоки, один замер занимает 16 слов

	NumIter++;
}
