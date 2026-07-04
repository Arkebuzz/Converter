/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
// #include <ti/drivers/EMAC.h>
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/USBMSCHFatFs.h>
// #include <ti/drivers/Watchdog.h>

#include "driverlib/ram.h"  // HWREG

#include "Board.h"

#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"


int main(void) {
	// ѕолучение доступа к защищенным регистрам, дл€ конфигурировани€ системы
	HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

	// Ќастройка частот, M3 и C28 работают на 100 м√ц
	SysCtlClockConfigSet(
		SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xA) |  // —истемна€ частота 10 м√ц (?), множитель 10 = 100 м√ц
		SYSCTL_SYSDIV_1 | 							   // ƒелитель частоты C28 1 = 100 м√ц
		SYSCTL_M3SSDIV_1 | 							   // ƒелитель частоты M3  1 = 100 м√ц
		SYSCTL_XCLKDIV_4							   // ƒелитель частоты внешних устройств (хз что это) 4 = 25 м√ц
	);

	// ѕередача управлени€ SM6 и SM7 на C28, зачем?
	RAMMReqSharedMemAccess((S6_ACCESS | S7_ACCESS), SX_C28MASTER);

    /* Ќастройка портов */
    Board_initGeneral();  // ¬ключение GPIO портов
    Board_initGPIO();	  // ¬ключили светодиоды ?
    Board_initEMAC();	  // Ќастройка Ethernet
    Board_initEPI();	  // Ќастройка EPI
    // Buffers_Init();	  // —оздание блока ошибок



    System_flush();

    /* Start BIOS */
    BIOS_start();

    return 0;
}
