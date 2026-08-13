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

	Page Program = 1-3 ms
	Byte Program = 7 mus
	Block Erase 4K = 50-200 ms
	Block Erase 32K = 250-600 ms
	Block Erase 64K = 400-950 ms
	Chip Erase = 32-56 s

NOTE:
	Flash works with 8 bit words, but we operate on 16 bit words
	All reads and writes are 16 bit wide

*/

#include "F28M35x_Device.h"

#include "flash.h"

#define SPISTEA_ASSERT (GpioDataRegs.GPADAT.bit.GPIO19 = 0)
#define SPISTEA_DEASSERT (GpioDataRegs.GPADAT.bit.GPIO19 = 1)

volatile Uint16 *spi_rx_buf = 0; // buffer of data to be sent
Uint16 spi_rx_read_count = 0; // count to read from the flash
Uint16 spi_rx_skip_count = 0; // count to skip flash's response to things like cmd and address
Bool spi_rx_overflowed = 0;

volatile const Uint16 *spi_tx_buf = 0; // buffer of data to receive into
Uint16 spi_tx_send_count = 0; // count to send to the flash
Uint16 spi_tx_dummy_count = 0; // count of dummy words to send to keep the clock ticking

interrupt void spi_rx_int(void) {
	// check for FIFO overflow
	if (SpiaRegs.SPIFFRX.bit.RXFFOVF) {
		SpiaRegs.SPIFFRX.bit.RXFFOVFCLR = 1;
		spi_rx_overflowed = 1;
	}

	Uint16 read_count = SpiaRegs.SPIFFRX.bit.RXFFST;
	// skip garbage
	while (read_count > 0 && spi_rx_skip_count > 0) {
		(void)SpiaRegs.SPIRXBUF;
		spi_rx_skip_count--;
		read_count--;
	}
	// copy data from FIFO to memory
	while (read_count > 0 && spi_rx_read_count > 0) {
		*spi_rx_buf = SpiaRegs.SPIRXBUF;
		spi_rx_buf++;
		spi_rx_read_count--;
		read_count--;
	}

	// unexpected data => skip
	while (read_count > 0) {
		(void)SpiaRegs.SPIRXBUF;
		read_count--;
	}
	if (spi_rx_read_count == 0 && spi_rx_skip_count == 0) {
		spi_rx_buf = 0; // signal that the read has finished
		SPISTEA_DEASSERT;
//		SpiaRegs.SPIFFRX.bit.RXFFIL = 16; // restore interrupt level
        SpiaRegs.SPIFFRX.bit.RXFFIENA = 0; // disable rx interrupt
	} else {
		// adjust interrupt level so RX FIFO doesn't have stale data of <16 words
//		Uint16 remaining = spi_rx_read_count + spi_rx_skip_count;
//		if (remaining < 16) {
//			SpiaRegs.SPIFFRX.bit.RXFFIL = 1; // remaining
//		}
	}

    SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}

interrupt void spi_tx_int(void) {
	Uint8 send_count = 16 - SpiaRegs.SPIFFTX.bit.TXFFST;

	// generate clocks for reading
	while (send_count > 0 && spi_tx_dummy_count > 0) {
		SpiaRegs.SPITXBUF = 0xFFFF; // write dummies
		spi_tx_dummy_count--;
		send_count--;
	}
	// send data
	while (send_count > 0 && spi_tx_send_count > 0) {
		SpiaRegs.SPITXBUF = *spi_tx_buf;
		spi_tx_buf++;
		spi_tx_send_count--;
		send_count--;
	}
	// disable the interrupt if no data left to send
    if (spi_tx_send_count == 0 && spi_tx_dummy_count == 0) {
        SpiaRegs.SPIFFTX.bit.TXFFIENA = 0;
    }

    SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}

// returns 1 if no operation is being performed, 0 otherwise
Bool flash_is_ready() {
	// check if both interrupts are disabled -
	// this means that rx and tx interrupts finished all their work
	return (
		SpiaRegs.SPIFFRX.bit.RXFFIENA == 0 &&
		SpiaRegs.SPIFFTX.bit.TXFFIENA == 0
	);
}

// returns 1 if rx queue overflowed and data got lost, otherwise 0
Bool flash_has_overflowed() {
	return spi_rx_overflowed;
}

static inline void spi_begin_reset(void) {
	// disable interrupts
	SpiaRegs.SPIFFRX.bit.RXFFIENA = 0;
	SpiaRegs.SPIFFTX.bit.TXFFIENA = 0;

	SPISTEA_ASSERT;

	// reset FIFO pointers and clear pending interrupts
	SpiaRegs.SPIFFTX.bit.TXFIFO = 0;
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;
	SpiaRegs.SPIFFTX.bit.TXFIFO = 1;

	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1;
	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;

	spi_rx_overflowed = 0;
}

static inline void spi_end_reset(void) {
	// enable interrupts
	SpiaRegs.SPIFFRX.bit.RXFFIENA = 1;
	SpiaRegs.SPIFFTX.bit.TXFFIENA = 1;
}

// "nonblocking" read
// starts reading `count` words from `address` on flash into `buf`
// returns 1 if started reading, 0 if busy
Bool flash_read_array(volatile Uint16 *buf, Uint16 count, Uint32 address) {
	if (!flash_is_ready()) {
		return 0;
	}

	spi_begin_reset();

	// write command and address
	SpiaRegs.SPITXBUF = ((Uint16)0x03 << 8) | (Uint16)(address >> 16);
	SpiaRegs.SPITXBUF = (Uint16)address;

	spi_rx_buf = buf;
	spi_rx_read_count = count;
	spi_tx_dummy_count = count;
	spi_rx_skip_count = 2;

	spi_end_reset();

	return 1;
}

static inline Bool flash_send_raw_word(Uint16 byte) {
	if (!flash_is_ready()) {
		return 0;
	}

	spi_begin_reset();

	// write command
	SpiaRegs.SPITXBUF = byte;
	spi_rx_skip_count = 1;

	spi_end_reset();

	return 1;
}

// SPRL - Sector Protection Registers Locked register
Bool flash_write_status_fst(Bool SPRL, Bool global_protect, Bool global_unprotect) {
	if (global_protect && global_unprotect) {
		return 0;
	}
	Uint8 byte = 0x0;
	byte |= ((Uint8)SPRL) << 7;
	if (global_unprotect) {
		byte |= 0b00000000;
	} else if (global_protect) {
		byte |= 0b00111100;
	} else {
		byte |= 0b00101000;
	}
	Uint16 word = (0x01 << 8) | byte;
	return flash_send_raw_word(word);
}

//Bool flash_write_status_snd() {
//
//}

Bool flash_reset() {
	return flash_send_raw_word(0xF0D0);
}

static inline Bool flash_send_cmd(Uint16 cmd) {
	return flash_send_raw_word(cmd << 8);
}

Bool flash_write_disable(void) {
	return flash_send_cmd(0x04);
}

Bool flash_write_enable(void) {
	return flash_send_cmd(0x06);
}

static inline Bool flash_send_cmd_address(Uint16 cmd, Uint32 address) {
	if (!flash_is_ready()) {
		return 0;
	}

	spi_begin_reset();

	// write command and address
	SpiaRegs.SPITXBUF = (cmd << 8) | (Uint16)(address >> 16);
	SpiaRegs.SPITXBUF = (Uint16)address;
	spi_rx_skip_count = 2;

	spi_end_reset();

	return 1;
}

Bool flash_block_erase_4K(Uint32 address) {
	return flash_send_cmd_address(0x20, address);
}

Bool flash_block_erase_32K(Uint32 address) {
	return flash_send_cmd_address(0x52, address);
}

Bool flash_block_erase_64K(Uint32 address) {
	return flash_send_cmd_address(0xD8, address);
}

// writes up to one page (256 bytes) starting at address
Bool flash_write_array(volatile const Uint16 *buf, Uint16 count, Uint32 address) {
	if (!flash_is_ready() || count > 128) {
		return 0;
	}

	spi_begin_reset();

	// write command and address
	SpiaRegs.SPITXBUF = ((Uint16)0x02 << 8) | (Uint16)(address >> 16);
	SpiaRegs.SPITXBUF = (Uint16)address;

	spi_tx_buf = buf;
	spi_tx_send_count = count;
	spi_rx_skip_count = 2 + count;

	spi_end_reset();

	return 1;
}

Bool flash_read_status(FlashStatusRegister *status) {
	if (!flash_is_ready()) {
		return 0;
	}

	spi_begin_reset();

	// write command
	SpiaRegs.SPITXBUF = ((Uint16)0x05 << 8);

	spi_rx_buf = (Uint16 *) status;
	spi_rx_read_count = 1;
	spi_tx_dummy_count = 1;
	spi_rx_skip_count = 1;

	spi_end_reset();

	return 1;
}

// configures SPI registers and enables SPI interrupts
void flash_spi_setup(void) {
	// force SPI module into reset state before configuring
	SpiaRegs.SPICCR.bit.SPISWRESET = 0;

	// set C28 as a Master of SPI
	SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;

	// set mode 3 of SPI
	SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
	SpiaRegs.SPICTL.bit.CLK_PHASE = 0;

	// set baud rate to 0 (max speed, see TIMING)
	SpiaRegs.SPIBRR = 0;

	// set character length to 16 bits
	SpiaRegs.SPICCR.bit.SPICHAR = (16 - 1);

	SpiaRegs.SPIFFTX.bit.SPIFFENA = 1; // enable FIFO
	SpiaRegs.SPIFFCT.bit.TXDLY = 0; // set TXFIFO transmit delay to 0

	// configure FIFO
	SpiaRegs.SPIFFTX.bit.TXFIFO = 0; // put TXFIFO into reset
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1; // clear interrupt flag
	SpiaRegs.SPIFFTX.bit.TXFFIENA = 0; // disable TXFIFO interrupt
	SpiaRegs.SPIFFTX.bit.TXFFIL = 0; // fire an interrupt when TXFIFO is empty
	SpiaRegs.SPIFFTX.bit.TXFIFO = 1; // release TXFIFO from reset

	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0; // put RXFIFO into reset
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1; // clear interrupt flag
	SpiaRegs.SPIFFRX.bit.RXFFIENA = 0; // disable RXFIFO interrupt

	// FIXME: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	SpiaRegs.SPIFFRX.bit.RXFFIL = 16; // fire an interrupt when RXFIFO has 16 words
	SpiaRegs.SPIFFRX.bit.RXFFIL = 1;

	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1; // release RXFIFO from reset

	// enable transmission
	SpiaRegs.SPICTL.bit.TALK = 1;

	// halting on a breakpoint will not halt the SPI
	SpiaRegs.SPIPRI.bit.FREE = 1;

	// finish configuration and release the module from the reset
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;

	EALLOW;
	PieVectTable.SPIRXINTA = spi_rx_int;
	PieVectTable.SPITXINTA = spi_tx_int;

	// configure pins
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // SPISIMOA
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // SPISOMIA
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0; // manually manage SPISTEA
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;  // set as output
    SPISTEA_DEASSERT;  // start deasserted
	EDIS;

	PieCtrlRegs.PIEIER6.bit.INTx1 = 1; // enable SPIRXINTA (6.1) interrupt
	PieCtrlRegs.PIEIER6.bit.INTx2 = 1; // enable SPITXINTA (6.2) interrupt
	IER |= M_INT6; // enable interrupt level 6
}
