#include "F28M35x_Device.h"
#include "F28M35x_Examples.h"
#include "F28M35x_Ipc.h"

#include <string.h>

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


}
