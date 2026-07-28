/*

Communication with external AT25DF321A Flash memory. Uses SPI module of C28.

PINS:
	102: SPISIMO/SSIOTX <-> SI (SIO)
	98: SPISOMI/SSI0RX  <-> SO (SOI)
	28: SPICLK/SSI0CLK  <-> SCK
	29: SPISTEA/SSI0FSS <-> nCS

TIMING:
	Flash device supports up to 100MHz SPI clock on both RX and TX

	* SYSCLKOUT (system clock) = 100MHz
	* LSPCLK (low-speed peripheral clock) is by default = SYSCLKOUT / 4 = 25MHz
	(however can be configured to be SYSCLKOUT / 1)
	* SPI Baud rate (SPICLK) = if SPIBRR is 0, 1, 2 then LSPCLK / 4 else LSPCLK / (SPIBRR + 1)
	SPIBRR = 0 => SPI Baud rate (SPICLK) = 6.25MHz

*/

#include "F28M35x_Device.h"

#include "flash.h"

interrupt void spi_rx_int(void) {
	// ...
}

interrupt void spi_tx_int(void) {
	// ...
}

void setup_SPI(void) {
	// force SPI module into reset state before configuring
	SpiaRegs.SPICCR.bit.SPISWRESET = 0;

	// set C28 as a Master of SPI
	SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;

	// set both clock polarity and phase to 0 (mode 0 of SPI)
	SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
	SpiaRegs.SPICTL.bit.CLK_PHASE = 0;

	// set baud rate to 0 (max speed, see TIMING)
	SpiaRegs.SPIBRR = 0;

	// set character length to 16 bits
	SpiaRegs.SPICCR.bit.SPICHAR = (16 - 1);

	// set up SPI interrupts
//	SpiaRegs.SPICTL.bit.SPIINTENA = 1; // enable interrupts
//	SpiaRegs.SPICTL.bit.OVERRUNINTENA = 1; // enable overrun interrupts

	SpiaRegs.SPIFFTX.bit.SPIFFENA = 1; // enable FIFO
	SpiaRegs.SPIFFCT.bit.TXDLY = 0; // set TXFIFO transmit delay to 0

	SpiaRegs.SPIFFTX.bit.TXFIFO = 0; // put TXFIFO into reset
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1; // clear interrupt flag
	SpiaRegs.SPIFFTX.bit.TXFFIENA = 1; // enable TXFIFO interrupt
	SpiaRegs.SPIFFTX.bit.TXFFIL = 0; // fire an interrupt when TXFIFO is empty
	SpiaRegs.SPIFFTX.bit.TXFIFO = 1; // release TXFIFO from reset

	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0; // put RXFIFO into reset
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1; // clear interrupt flag
	SpiaRegs.SPIFFRX.bit.RXFFIENA = 1; // enable RXFIFO interrupt
	SpiaRegs.SPIFFRX.bit.RXFFIL = 16; // fire an interrupt when RXFIFO has 16 words
	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1; // release RXFIFO from reset

	// enable transmission
	SpiaRegs.SPICTL.bit.TALK = 1;

	// finish configuration and release the module from the reset
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;

	PieVectTable.SPIRXINTA = spi_rx_int;
	PieVectTable.SPITXINTA = spi_tx_int;

	PieCtrlRegs.PIEIER6.bit.INTx1 = 1; // enable SPIRXINTA (6.1) interrupt
	PieCtrlRegs.PIEIER6.bit.INTx2 = 1; // enable SPITXINTA (6.2) interrupt
	IER |= M_INT6; // enable interrupt level 6
}
