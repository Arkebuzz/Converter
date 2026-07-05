/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== TMDXDOCKH52C1.c ========
 *  This file is responsible for setting up the board specific items for the
 *  TMDXDOCKH52C1 board.
 */

#include <stdint.h>
#include <stdbool.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <inc/hw_gpio.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_types.h>
#include <inc/hw_epi.h>

#include <driverlib/gpio.h>
#include <driverlib/i2c.h>
#include <driverlib/ssi.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <driverlib/udma.h>
#include <driverlib/epi.h>


#include "TMDXDOCKH52C1.h"

#define EPI_HB16_CSCFG_ALE_HIGH		  0x00080000

/*
 *  =============================== DMA ===============================
 */
#pragma DATA_SECTION(dmaControlTable, ".dma");
#pragma DATA_ALIGN(dmaControlTable, 1024)
static tDMAControlTable dmaControlTable[32];
static bool dmaInitialized = false;

/* Hwi_Struct used in the initDMA Hwi_construct call */
static Hwi_Struct dmaHwiStruct;

/*
 *  ======== dmaErrorHwi ========
 */
static Void dmaErrorHwi(UArg arg)
{
    System_printf("DMA error code: %d\n", uDMAErrorStatusGet());
    uDMAErrorStatusClear();
    System_abort("DMA error!!");
}

/*
 *  ======== TMDXDOCKH52C1_initDMA ========
 */
void TMDXDOCKH52C1_initDMA(void)
{
    Error_Block eb;
    Hwi_Params  hwiParams;

    if (!dmaInitialized) {
        Error_init(&eb);
        Hwi_Params_init(&hwiParams);
        Hwi_construct(&(dmaHwiStruct), INT_UDMAERR, dmaErrorHwi,
                      &hwiParams, &eb);
        if (Error_check(&eb)) {
            System_abort("Couldn't construct DMA error hwi");
        }

        SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
        uDMAEnable();
        uDMAControlBaseSet(dmaControlTable);

        dmaInitialized = true;
    }
}

/*
 *  =============================== EMAC ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(EMAC_config, ".const:EMAC_config")
#pragma DATA_SECTION(emacHWAttrs, ".const:emacHWAttrs")

#include <ti/drivers/EMAC.h>
#include <ti/drivers/emac/EMACTiva.h>

EMACTiva_Object emacObjects[TMDXDOCKH52C1_EMACCOUNT];

/*
 *  EMAC configuration structure
 *  Set user/company specific MAC octates. The following sets the address
 *  to ff-ff-ff-ff-ff-ff. Users need to change this to match the label on
 *  their boards.
 */
#ifndef MACADDRESS
#define MACADDRESS 0xD0,0xD0,0xD0,0x01,0x01,0x77
#endif
unsigned char macAddress[6] = {MACADDRESS};

/*
 *  Required by the Networking Stack (NDK). This array must be NULL terminated.
 *  This can be removed if NDK is not used.
 *  Double curly braces are needed to avoid GCC bug #944572
 *  https://bugs.launchpad.net/gcc-linaro/+bug/944572
 */
#pragma DATA_SECTION(NIMUDeviceTable, ".data:NIMUDeviceTable")
NIMU_DEVICE_TABLE_ENTRY  NIMUDeviceTable[2] = {{.init = EMACTiva_NIMUInit}, {NULL}};

const EMACTiva_HWAttrs emacHWAttrs[TMDXDOCKH52C1_EMACCOUNT] = {
    {
        .intNum = INT_ETH,
        .intPriority = (~0),
        .macAddress = macAddress
    }
};

const EMAC_Config EMAC_config[] = {
    {
        .fxnTablePtr = &EMACTiva_fxnTable,
        .object = &emacObjects[0],
        .hwAttrs = &emacHWAttrs[0]
    },
    {NULL, NULL, NULL}
};



void PCON_EPI_INIT(void)
{
	unsigned long ulBase;
		unsigned long ulConfig;
		unsigned long ulMap;

		int a =0;
		a = a +22;
		//EPI PortC
		GPIODirModeSet  (GPIO_PORTC_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4, GPIO_DIR_MODE_HW);
		GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4, GPIO_PIN_TYPE_STD_WPU);
		GPIOPinConfigure(GPIO_PC4_EPI0S2);
		GPIOPinConfigure(GPIO_PC5_EPI0S3);
		GPIOPinConfigure(GPIO_PC6_EPI0S4);
		GPIOPinConfigure(GPIO_PC7_EPI0S5);

		//EPI PortE
		GPIODirModeSet  (GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_DIR_MODE_HW);
		GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_TYPE_STD_WPU);
		GPIOPinConfigure(GPIO_PE0_EPI0S8);
		GPIOPinConfigure(GPIO_PE1_EPI0S9);
		GPIOPinConfigure(GPIO_PE2_EPI0S24);
		GPIOPinConfigure(GPIO_PE3_EPI0S25);

		//EPI PortF
		GPIODirModeSet  (GPIO_PORTF_BASE, GPIO_PIN_5|GPIO_PIN_4, GPIO_DIR_MODE_HW);
		GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_5|GPIO_PIN_4, GPIO_PIN_TYPE_STD_WPU);
		GPIOPinConfigure(GPIO_PF4_EPI0S12);
		GPIOPinConfigure(GPIO_PF5_EPI0S15);

		//EPI PortG
		GPIODirModeSet  (GPIO_PORTG_BASE, GPIO_PIN_1|GPIO_PIN_0, GPIO_DIR_MODE_HW);
		GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_TYPE_STD_WPU);
		GPIOPinConfigure(GPIO_PG0_EPI0S13);
		GPIOPinConfigure(GPIO_PG1_EPI0S14);

		//EPI PortH
		GPIODirModeSet  (GPIO_PORTH_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_DIR_MODE_HW);
		GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_TYPE_STD_WPU);
		GPIOPinConfigure(GPIO_PH0_EPI0S6);
		GPIOPinConfigure(GPIO_PH1_EPI0S7);
		GPIOPinConfigure(GPIO_PH2_EPI0S1);
		GPIOPinConfigure(GPIO_PH3_EPI0S0);
		GPIOPinConfigure(GPIO_PH4_EPI0S10);
		GPIOPinConfigure(GPIO_PH5_EPI0S11);
		GPIOPinConfigure(GPIO_PH6_EPI0S26);
		GPIOPinConfigure(GPIO_PH7_EPI0S27);

		//EPI PortJ
		GPIODirModeSet  (GPIO_PORTJ_BASE, GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_DIR_MODE_HW);
		GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_TYPE_STD_WPU);
		GPIOPinConfigure(GPIO_PJ0_EPI0S16);
		GPIOPinConfigure(GPIO_PJ1_EPI0S17);
		GPIOPinConfigure(GPIO_PJ2_EPI0S18);
		GPIOPinConfigure(GPIO_PJ3_EPI0S19);
		GPIOPinConfigure(GPIO_PJ4_EPI0S28);
		GPIOPinConfigure(GPIO_PJ5_EPI0S29);
		GPIOPinConfigure(GPIO_PJ6_EPI0S30);

		SysCtlPeripheralEnable(SYSCTL_PERIPH_EPI0);
		SysCtlPeripheralReset(SYSCTL_PERIPH_EPI0);

		// Set 16 Bit HostBus mode.
		EPIModeSet(EPI0_BASE, EPI_MODE_HB16);


		// PLL, M3 running at 100MHz and C28 running at 100MHz
		//Set clock divider to 2 (divide by 4). With this EPI Frq will be 25MHz (M3-Frq/4).
		EPIDividerSet(EPI0_BASE, 0x2);

		// Read wait state = 0
		// Write wait state = 0
		// Address & Data are not muxed (ADNOMUX = 0x1).
		//EPIConfigHB16Set(EPI0_BASE, (EPI_HB16_MODE_ADMUX | EPI_HB16_WRWAIT_0 | EPI_HB16_RDWAIT_0|EPI_HB16_CSCFG_ALE_DUAL_CS), 0);

		EPIConfigHB16Set(EPI0_BASE, (EPI_HB16_MODE_ADMUX | EPI_HB16_WRWAIT_0 | EPI_HB16_RDWAIT_0|EPI_HB16_CSCFG_DUAL_CS|EPI_HB16_CSCFG_ALE_HIGH), 0);
		ulConfig = (EPI_HB16_MODE_ADMUX | EPI_HB16_WRWAIT_0 | EPI_HB16_RDWAIT_0|EPI_HB16_CSCFG_DUAL_CS);
		ulBase = EPI0_BASE;
		HWREG(ulBase + EPI_O_HB16CFG2) = (((ulConfig & EPI_HB16_WORD_ACCESS) ?
		                                       EPI_HB16CFG2_WORD : 0) |
		                                      ((ulConfig & EPI_HB16_CSBAUD_DUAL) ?
		                                       EPI_HB16CFG2_CSBAUD : 0) |
		                                      ((ulConfig & EPI_HB16_CSCFG_MASK) << 15) |
		                                      0x8000000);


		//EPIAddressMapSet(EPI0_BASE, (EPI_ADDR_RAM_SIZE_16MB | EPI_ADDR_RAM_BASE_6) );

		ulMap = 0xBB;
		HWREG(ulBase + EPI_O_ADDRMAP) = ulMap;
}
/*
 *  ======== TMDXDOCKH52C1_initEMAC ========
 */
void TMDXDOCKH52C1_initEMAC(void)
{
			//-----------------------NEW SETUP-----------------------------------------
			/* RXD0, TXER, MDIO, RXD3 -- PE4, PE5, PE6, PE7 -- 7,C,C,C */
			GPIODirModeSet  (GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4, GPIO_DIR_MODE_HW);
			GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4, GPIO_PIN_TYPE_STD);
			GPIOPinConfigure(GPIO_PE4_MIIRXD0);
			GPIOPinConfigure(GPIO_PE5_MIITXER);
			GPIOPinConfigure(GPIO_PE6_MIIMDIO);
			GPIOPinConfigure(GPIO_PE7_MIIRXD3);

			/* CRS, RXD1 -- PB6, PB7 -- C,7 */
			HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK)= 0x4C4F434B;
			HWREG(GPIO_PORTB_BASE + GPIO_O_CR)=   0x000000FF;
			HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK)= 0x4C4F434B;
			GPIODirModeSet  (GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6, GPIO_DIR_MODE_HW);
		    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6, GPIO_PIN_TYPE_STD);
		    GPIOPinConfigure(GPIO_PB6_MIICRS);
		    GPIOPinConfigure(GPIO_PB7_MIIRXD1);
		    HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK)= 0x4C4F434B;
		    HWREG(GPIO_PORTB_BASE + GPIO_O_CR)=   0x0000007F;
		    HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK)= 0x00000001;

			/* COL, RXDV, TXEN, TXCK -- PG2, PG3, PG5, PG6 -- 3,C,3,3 */
			GPIODirModeSet  (GPIO_PORTG_BASE, GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_3|GPIO_PIN_2, GPIO_DIR_MODE_HW);
		    GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_3|GPIO_PIN_2, GPIO_PIN_TYPE_STD);
		    GPIOPinConfigure(GPIO_PG2_MIICOL);
		    GPIOPinConfigure(GPIO_PG3_MIIRXDV);
		    GPIOPinConfigure(GPIO_PG5_MIITXEN);
		    //GPIOPinConfigure(GPIO_PG6_MIITXCK); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			HWREG(GPIO_PORTG_BASE + GPIO_O_PCTL) &= 0xF0FFFFFF;
			HWREG(GPIO_PORTG_BASE + GPIO_O_PCTL) |= 0x03000000;

			/* TXD3, TXD2, TXD1, TXD0 -- PD4, PD5, PD6, PD7 -- 4,4,4,4 */
			GPIODirModeSet  (GPIO_PORTD_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4, GPIO_DIR_MODE_HW);
			GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4, GPIO_PIN_TYPE_STD);
			GPIOPinConfigure(GPIO_PD4_MIITXD3);
			GPIOPinConfigure(GPIO_PD5_MIITXD2);
			GPIOPinConfigure(GPIO_PD6_MIITXD1);
			GPIOPinConfigure(GPIO_PD7_MIITXD0);

			/* RXCK, RXER, PHYINTRn, MDC, RXD2 -- PF0, PF1, PF2, PF3, PF6 -- 4,4,3,3,3 */
			GPIODirModeSet  (GPIO_PORTF_BASE,  GPIO_PIN_6|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_DIR_MODE_HW);
			GPIOPadConfigSet(GPIO_PORTF_BASE,  GPIO_PIN_6|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_TYPE_STD);
			GPIOPinConfigure(GPIO_PF0_MIIRXCK);
			GPIOPinConfigure(GPIO_PF1_MIIRXER);
			GPIOPinConfigure(GPIO_PF2_MIIPHYINTRn);
			GPIOPinConfigure(GPIO_PF3_MIIMDC);
			//GPIOPinConfigure(GPIO_PF6_MIIRXD2); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			HWREG(GPIO_PORTF_BASE + GPIO_O_PCTL) &= 0xF0FFFFFF;
			HWREG(GPIO_PORTF_BASE + GPIO_O_PCTL) |= 0x03000000;

			/* PHYRSTn - PJ7 - C*/
			GPIODirModeSet  (GPIO_PORTJ_BASE, GPIO_PIN_7, GPIO_DIR_MODE_HW);
			GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_7, GPIO_PIN_TYPE_STD);
			GPIOPinConfigure(GPIO_PJ7_MIIPHYRSTn);
		    //-----------------------NEW SETUP-----------------------------------------

	    /* Enable and Reset the Ethernet Controller. */
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
	    SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

	    if (macAddress[0] == 0xff && macAddress[1] == 0xff &&
	        macAddress[2] == 0xff && macAddress[3] == 0xff &&
	        macAddress[4] == 0xff && macAddress[5] == 0xff) {
	        System_abort("Change the macAddress variable to match your board's MAC sticker");
	    }

	    /* Once EMAC_init is called, EMAC_config cannot be changed */
	    EMAC_init();
}

/*
 *  =============================== General ===============================
 */
/*
 *  ======== TMDXDOCKH52C1_initGeneral ========
 */
void TMDXDOCKH52C1_initGeneral(void)
{
	/* Disable Protection */
	    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

	    /* Enable clock supply for the following peripherals */
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

	    /* Disable clock supply for the watchdog modules */
	    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);
	    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG0);
}

/*
 *  =============================== GPIO ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(GPIOTiva_config, ".const:GPIOTiva_config")

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOTiva.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in TMDXDOCKH52C1.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {

    /* Output pins */
    GPIOTiva_PB_2 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW,
    GPIOTiva_PB_3 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW,
	GPIOTiva_PA_4 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW,
	GPIOTiva_PA_5 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW,
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in TMDXDOCKH52C1.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL   /* TMDXDOCKH52C1_BUTTON */
};

/* The device-specific GPIO_config structure */
const GPIOTiva_Config GPIOTiva_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  ======== TMDXDOCKH52C1_initGPIO ========
 */
void TMDXDOCKH52C1_initGPIO(void)
{
	/* Setup the M3 GPIO pins used */
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);

	/* Setup the C28 GPIO pins used */
	GPIOPinConfigureCoreSelect(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_7,GPIO_PIN_C_CORE_SELECT);
	GPIOPinConfigureCoreSelect(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_C_CORE_SELECT);

	GPIOPinConfigureCoreSelect(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_C_CORE_SELECT);

	/* Once GPIO_init is called, GPIO_config cannot be changed */
	GPIO_init();

	GPIO_write(TMDXDOCKH52C1_LED, GPIO_TURN_OFF);
	GPIO_write(TMDXDOCKH52C1_LED, GPIO_TURN_OFF);
}

/*
 *  =============================== I2C ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cTivaHWAttrs, ".const:i2cTivaHWAttrs")

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CTiva.h>

I2CTiva_Object i2cTivaObjects[TMDXDOCKH52C1_I2CCOUNT];

const I2CTiva_HWAttrs i2cTivaHWAttrs[TMDXDOCKH52C1_I2CCOUNT] = {
    {
        .baseAddr = I2C0_MASTER_BASE,
        .intNum = INT_I2C0,
        .intPriority = (~0)
    },
    {
        .baseAddr = I2C1_MASTER_BASE,
        .intNum = INT_I2C1,
        .intPriority = (~0)
    }
};

const I2C_Config I2C_config[] = {
    {
        .fxnTablePtr = &I2CTiva_fxnTable,
        .object = &i2cTivaObjects[0],
        .hwAttrs = &i2cTivaHWAttrs[0]
    },
    {
        .fxnTablePtr = &I2CTiva_fxnTable,
        .object = &i2cTivaObjects[1],
        .hwAttrs = &i2cTivaHWAttrs[1]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== TMDXDOCKH52C1_initI2C ========
 */
void TMDXDOCKH52C1_initI2C(void)
{
    /* I2C0 Init */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinUnlock(GPIO_PORTB_BASE, GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PB7_I2C0SCL); /* GPIO15 on Concerto base board */
    GPIOPinConfigure(GPIO_PB6_I2C0SDA); /* GPIO14 on Concerto base board */
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    /* I2C1 Init */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinConfigure(GPIO_PA0_I2C1SCL); /* GPIO00 on Concerto base board */
    GPIOPinConfigure(GPIO_PA1_I2C1SDA); /* GPIO01 on Concerto base board */
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    I2C_init();
}

/*
 *  =============================== SDSPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(SDSPI_config, ".const:SDSPI_config")
#pragma DATA_SECTION(sdspiTivaHWattrs, ".const:sdspiTivaHWattrs")

#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPITiva.h>

SDSPITiva_Object sdspiTivaObjects[TMDXDOCKH52C1_SDSPICOUNT];

const SDSPITiva_HWAttrs sdspiTivaHWattrs[TMDXDOCKH52C1_SDSPICOUNT] = {
    {
        .baseAddr = SSI0_BASE,

        .portSCK = GPIO_PORTD_BASE,
        .pinSCK = GPIO_PIN_2,
        .portMISO = GPIO_PORTD_BASE,
        .pinMISO = GPIO_PIN_1,
        .portMOSI = GPIO_PORTD_BASE,
        .pinMOSI = GPIO_PIN_0,
        .portCS = GPIO_PORTD_BASE,
        .pinCS = GPIO_PIN_3
    }
};

const SDSPI_Config SDSPI_config[] = {
    {
        .fxnTablePtr = &SDSPITiva_fxnTable,
        .object = &sdspiTivaObjects[0],
        .hwAttrs = &sdspiTivaHWattrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== TMDXDOCKH52C1_initSDSPI ========
 */
void TMDXDOCKH52C1_initSDSPI(void)
{
    /* Enable the peripherals used by the SD Card */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    /* Configure SSI Peripheral pin muxing */
    GPIOPinConfigure(GPIO_PD2_SSI0CLK);
    GPIOPinConfigure(GPIO_PD0_SSI0TX);
    GPIOPinConfigure(GPIO_PD1_SSI0RX);

    /* Configure pad settings */
    GPIOPadConfigSet(GPIO_PORTD_BASE,
            GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0,
            GPIO_PIN_TYPE_STD_WPU);

    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_TYPE_STD_WPU);

    SDSPI_init();
}

/*
 *  =============================== SPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiTivaDMAHWAttrs, ".const:spiTivaDMAHWAttrs")

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPITivaDMA.h>

SPITivaDMA_Object spiTivaDMAObjects[TMDXDOCKH52C1_SPICOUNT];

#pragma DATA_SECTION(spiTivaDMAscratchBuf, ".dma");
#pragma DATA_ALIGN(spiTivaDMAscratchBuf, 32)
uint32_t spiTivaDMAscratchBuf[TMDXDOCKH52C1_SPICOUNT];

const SPITivaDMA_HWAttrs spiTivaDMAHWAttrs[TMDXDOCKH52C1_SPICOUNT] = {
    {
        .baseAddr = SSI0_BASE,
        .intNum = INT_SSI0,
        .intPriority = (~0),
        .scratchBufPtr = &spiTivaDMAscratchBuf[0],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_CHANNEL_SSI0RX,
        .txChannelIndex = UDMA_CHANNEL_SSI0TX,
        .channelMappingFxn = uDMAChannel8_15SelectDefault,
        .rxChannelMappingFxnArg = UDMA_CHAN10_DEF_SSI0RX_M,
        .txChannelMappingFxnArg = UDMA_CHAN11_DEF_SSI0TX_M
    },
    {
        .baseAddr = SSI1_BASE,
        .intNum = INT_SSI1,
        .intPriority = (~0),
        .scratchBufPtr = &spiTivaDMAscratchBuf[1],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_CHANNEL_SSI1RX,
        .txChannelIndex = UDMA_CHANNEL_SSI1TX,
        .channelMappingFxn = uDMAChannel24_31SelectDefault,
        .rxChannelMappingFxnArg = UDMA_CHAN24_DEF_SSI1RX_M,
        .txChannelMappingFxnArg = UDMA_CHAN25_DEF_SSI1TX_M
    },
    {
        .baseAddr = SSI2_BASE,
        .intNum = INT_SSI2,
        .intPriority = (~0),
        .scratchBufPtr = &spiTivaDMAscratchBuf[2],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_THRD_CHANNEL_SSI2RX,
        .txChannelIndex = UDMA_THRD_CHANNEL_SSI2TX,
        .channelMappingFxn = uDMAChannel8_15SelectAltMapping,
        .rxChannelMappingFxnArg = UDMA_CHAN12_THRD_SSI2RX,
        .txChannelMappingFxnArg = UDMA_CHAN13_THRD_SSI2TX
    },
    {
        .baseAddr = SSI3_BASE,
        .intNum = INT_SSI3,
        .intPriority = (~0),
        .scratchBufPtr = &spiTivaDMAscratchBuf[3],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_THRD_CHANNEL_SSI3RX,
        .txChannelIndex = UDMA_THRD_CHANNEL_SSI3TX,
        .channelMappingFxn = uDMAChannel8_15SelectAltMapping,
        .rxChannelMappingFxnArg = UDMA_CHAN14_THRD_SSI3RX,
        .txChannelMappingFxnArg = UDMA_CHAN15_THRD_SSI3TX
    }
};

const SPI_Config SPI_config[] = {
    {
        .fxnTablePtr = &SPITivaDMA_fxnTable,
        .object = &spiTivaDMAObjects[0],
        .hwAttrs = &spiTivaDMAHWAttrs[0]
    },
    {
        .fxnTablePtr = &SPITivaDMA_fxnTable,
        .object = &spiTivaDMAObjects[1],
        .hwAttrs = &spiTivaDMAHWAttrs[1]
    },
    {
        .fxnTablePtr = &SPITivaDMA_fxnTable,
        .object = &spiTivaDMAObjects[2],
        .hwAttrs = &spiTivaDMAHWAttrs[2]
    },
    {
        .fxnTablePtr = &SPITivaDMA_fxnTable,
        .object = &spiTivaDMAObjects[3],
        .hwAttrs = &spiTivaDMAHWAttrs[3]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== TMDXDOCKH52C1_initSPI ========
 */
void TMDXDOCKH52C1_initSPI(void)
{
    /* SSI0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5);

    /* SSI1 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);

    GPIOPinConfigure(GPIO_PE0_SSI1CLK);
    GPIOPinConfigure(GPIO_PE1_SSI1FSS);
    GPIOPinConfigure(GPIO_PE2_SSI1RX);
    GPIOPinConfigure(GPIO_PE3_SSI1TX);

    GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);
    TMDXDOCKH52C1_initDMA();
    SPI_init();
}

/*
 *  =============================== UART ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartTivaHWAttrs, ".const:uartTivaHWAttrs")

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTTiva.h>

UARTTiva_Object uartTivaObjects[TMDXDOCKH52C1_UARTCOUNT];
unsigned char uartTivaRingBuffer[TMDXDOCKH52C1_UARTCOUNT][32];

/* UART configuration structure */
const UARTTiva_HWAttrs uartTivaHWAttrs[TMDXDOCKH52C1_UARTCOUNT] = {
    {
        .baseAddr = UART0_BASE,
        .intNum = INT_UART0,
        .intPriority = (~0),
        .flowControl = UART_FLOWCONTROL_NONE,
        .ringBufPtr  = uartTivaRingBuffer[0],
        .ringBufSize = sizeof(uartTivaRingBuffer[0])
    }
};

const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTTiva_fxnTable,
        .object = &uartTivaObjects[0],
        .hwAttrs = &uartTivaHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== TMDXDOCKH52C1_initUART ========
 */
void TMDXDOCKH52C1_initUART(void)
{
    /* Enable and configure the peripherals used by the uart. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PE4_U0RX);
    GPIOPinConfigure(GPIO_PE5_U0TX);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_TYPE_STD_WPU);

    /* Initialize the UART driver */
    UART_init();
}

/*
 *  =============================== USB ===============================
 */
/*
 *  ======== TMDXDOCKH52C1_initUSB ========
 *  This function just turns on the USB
 */
void TMDXDOCKH52C1_initUSB(TMDXDOCKH52C1_USBMode usbMode)
{
    /*
     * Setup USB clock tree for 60MHz
     * 20MHz * 12 / 4 = 60
     */
    SysCtlUSBPLLConfigSet(
            (SYSCTL_UPLLIMULT_M & 12) | SYSCTL_UPLLCLKSRC_X1 | SYSCTL_UPLLEN);

    /* Setup pins for USB operation */
    GPIOPinTypeUSBAnalog(GPIO_PORTF_BASE, GPIO_PIN_6);
    GPIOPinTypeUSBAnalog(
            GPIO_PORTG_BASE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);

    /* Additional configurations for Host mode */
    if (usbMode == TMDXDOCKH52C1_USBHOST) {
        /* Configure the pins needed */
        GPIOPinConfigure(GPIO_PC5_USB0EPEN);
        GPIOPinConfigure(GPIO_PJ1_USB0PFLT);
        GPIOPinTypeUSBDigital(GPIO_PORTC_BASE, GPIO_PIN_5);
        GPIOPinTypeUSBDigital(GPIO_PORTJ_BASE, GPIO_PIN_1);
    }
}

/*
 *  =============================== USBMSCHFatFs ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(USBMSCHFatFs_config, ".const:USBMSCHFatFs_config")
#pragma DATA_SECTION(usbmschfatfstivaHWAttrs, ".const:usbmschfatfstivaHWAttrs")

#include <ti/drivers/USBMSCHFatFs.h>
#include <ti/drivers/usbmschfatfs/USBMSCHFatFsTiva.h>

USBMSCHFatFsTiva_Object usbmschfatfstivaObjects[TMDXDOCKH52C1_USBMSCHFatFsCOUNT];

const USBMSCHFatFsTiva_HWAttrs usbmschfatfstivaHWAttrs[TMDXDOCKH52C1_USBMSCHFatFsCOUNT] = {
    {
        .intNum = INT_USB0,
        .intPriority = (~0)
    }
};

const USBMSCHFatFs_Config USBMSCHFatFs_config[] = {
    {
        .fxnTablePtr = &USBMSCHFatFsTiva_fxnTable,
        .object = &usbmschfatfstivaObjects[0],
        .hwAttrs = &usbmschfatfstivaHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== TMDXDOCKH52C1_initUSBMSCHFatFs ========
 */
void TMDXDOCKH52C1_initUSBMSCHFatFs(void)
{
    /* Call the USB initialization function for the USB Reference modules */
    TMDXDOCKH52C1_initUSB(TMDXDOCKH52C1_USBHOST);
    USBMSCHFatFs_init();
}

/*
 *  =============================== Watchdog ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#pragma DATA_SECTION(Watchdog_config, ".const:Watchdog_config")
#pragma DATA_SECTION(watchdogTivaHWAttrs, ".const:watchdogTivaHWAttrs")

#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogTiva.h>

WatchdogTiva_Object watchdogTivaObjects[TMDXDOCKH52C1_WATCHDOGCOUNT];

const WatchdogTiva_HWAttrs watchdogTivaHWAttrs[TMDXDOCKH52C1_WATCHDOGCOUNT] = {
    {
        .baseAddr = WATCHDOG0_BASE,
        .intNum = INT_WATCHDOG,
        .intPriority = (~0),
        .reloadValue = 75000000 // 1 second period at default CPU clock freq
    },
};

const Watchdog_Config Watchdog_config[] = {
    {
        .fxnTablePtr = &WatchdogTiva_fxnTable,
        .object = &watchdogTivaObjects[0],
        .hwAttrs = &watchdogTivaHWAttrs[0]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== TMDXDOCKH52C1_initWatchdog ========
 */
void TMDXDOCKH52C1_initWatchdog(void)
{
    /* Enable peripherals used by Watchdog */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    Watchdog_init();
}
