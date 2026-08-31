/*
 * ExternalControls.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include <GlobalData.h>





void ErrorRemove(void)
	{
	GpioG1DataRegs.GPASET.bit.GPIO7 = 1;
	GpioG1DataRegs.GPASET.bit.GPIO11 = 1;
	} //Remove system ERR flag to FPGA

void ActivateInverter(Uint16 Activate)
{
	if (Activate==0) {InverterActivated=0;GpioG1DataRegs.GPACLEAR.bit.GPIO3 = 1;}
	else {InverterActivated=1;GpioG1DataRegs.GPASET.bit.GPIO3 = 1;}
}

void ActivateChopper(Uint16 Activate)
{
	if (Activate==0) {ChopperActivated=0;}
	else {ChopperActivated=1;}
}

void ActivateBrake(Uint16 Activate)
{
	if (Activate==0) {;BrakeActivated=0;}
	else {BrakeActivated=1;}
}

void ActivateExcitation(Uint16 Activate)
{
	if (Activate==0) {ExcitationActivated=0;GpioG1DataRegs.GPACLEAR.bit.GPIO4 = 1;}
	else {ExcitationActivated=1;GpioG1DataRegs.GPASET.bit.GPIO4 = 1;}
}

void ActivateBreakerControl(Uint16 Activate)
{
	if (Activate==0) {BreakerActivated=0;}
	else {BreakerActivated=1;}
}

