#include "communication_M3.h"
#include "error_handling.h"


short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value) {
	// COPY_PASTE

	float *XMEM_pw;
	if (offset > 0x3FF) {
		return -1;
	}
	else {
		XMEM_pw = (float *) (0x0003F800 + offset);
		*XMEM_pw = value;
		return 0;
	}
}


short WriteTo_CTOM_MSGRAM(unsigned short offset, short value) {
	// COPY_PASTE

	short *XMEM_pw;
	if (offset > 0x3FF) {
		return -1;
	}
	else {
		XMEM_pw = (short *) (0x0003F800 + offset);
		*XMEM_pw = value;
		return 0;
	}
}


short ReadFrom_MTOC_MSGRAM(short offset) {
	// COPY_PASTE

	short *XMEM_pw;
	short result;
	if (offset > 0x3FF) {
		return 0;
	}
	else {
		XMEM_pw = (short *) (0x0003FC00 + offset);
		result = *XMEM_pw;
		return result;
	}
}


float ReadFrom_MTOC_MSGRAM_Float(short offset) {
	// COPY_PASTE

	float *XMEM_pw;
	float result;
	if (offset > 0x3FF) {
		return 0;
	}
	else {
		XMEM_pw = (float *) (0x0003FC00 + offset);
		result = *XMEM_pw;
		return result;
	}
}


// Communication with M3 uses SRAM6-SRAM7
#pragma DATA_SECTION(SHARERAMS6, "SHARERAMS6")
volatile Uint8 SHARERAMS6[0x1000];

#pragma DATA_SECTION(SHARERAMS7, "SHARERAMS7")
volatile Uint8 SHARERAMS7[0x1000];

#define S6_START SHARERAMS6
#define S7_END   (&SHARERAMS7[sizeof(SHARERAMS7) / sizeof(SHARERAMS7[0])])

void WriteToM3Data(const DataToM3 Data) {
	// Продублируем ошибки в CtoM:
	WriteTo_CTOM_MSGRAM(0, Data.C28_Errors);
	WriteTo_CTOM_MSGRAM(1, Data.C28_Errors_Latch);
	WriteTo_CTOM_MSGRAM(2, Data.FPGA_Errors);
	WriteTo_CTOM_MSGRAM(3, Data.FPGA_Errors_Latch);

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
	static volatile Uint16 *Dest = (Uint16 *)S6_START;

	if (NumIter == 64) {
		// IVAN: Пакет заполнился

		WriteTo_CTOM_MSGRAM(10, NumPacket);  // Информируем М3 о готовности пакета

		NumIter = 0; // IVAN: сбрасываем кол-во отправленых замеров в текущем пакете
		NumPacket++; // сдвигаем запись на след пакет
	}
	if (NumPacket == 8) {
		// IVAN: все пакеты заполнились => зацикливаемся на 0 пакет
		NumPacket = 0;
		Dest = (Uint16 *)S6_START;
	}

	// IVAN: CycleCounter размером 64 бита, слово у нас 16 бит
	// поэтому разбиваем его на 4 части
	Uint16 CyclesCounter0 = (Uint16) ((Data.CycleCounter & 0x000000000000FFFF) >> 0);
	Uint16 CyclesCounter1 = (Uint16) ((Data.CycleCounter & 0x00000000FFFF0000) >> 16);
	Uint16 CyclesCounter2 = (Uint16) ((Data.CycleCounter & 0x0000FFFF00000000) >> 32);
	Uint16 CyclesCounter3 = (Uint16) ((Data.CycleCounter & 0xFFFF000000000000) >> 48);

	*Dest = CyclesCounter0; 		Dest++;  // 0
	*Dest = CyclesCounter1; 		Dest++;  // 1
	*Dest = CyclesCounter2; 		Dest++;  // 2
	*Dest = CyclesCounter3; 		Dest++;  // 3
	*Dest = Data.C28_Errors;		Dest++;  // 4
	*Dest = Data.C28_Errors_Latch;  Dest++;  // 5
	*Dest = Data.FPGA_Errors; 		Dest++;  // 6
	*Dest = Data.FPGA_Errors_Latch; Dest++;  // 7
	*Dest = Data.Voltage_Inp;		Dest++;  // 8
	*Dest = Data.Voltage_Out;		Dest++;  // 9
	*Dest = Data.Current_1;			Dest++;  // 10
	*Dest = Data.Current_2;			Dest++;  // 11
	*Dest = Data.WatchDog;			Dest++;  // 12
	*Dest = Data.FreeTimeCounter;	Dest++;  // 13
	Dest++;  // 14
	Dest++;  // 15

	// IVAN:  Записываем 14 слов за один замер
	// ARTEM: Для простоты деления памяти на блоки, один замер занимает 16 слов

	NumIter++;
}
