
#include "F28M35x_Device.h"

Bool flash_is_ready();

Bool flash_read_array(volatile Uint16 *buf, Uint16 count, Uint32 address);

Bool flash_write_enable(void);
Bool flash_write_disable(void);

Bool flash_block_erase_4K(Uint32 address);
Bool flash_block_erase_32K(Uint32 address);
Bool flash_block_erase_64K(Uint32 address);

// writes up to one page (256 bytes) starting at address
Bool flash_write_array(volatile const Uint16 *buf, Uint16 count, Uint32 address);

union FlashStatusRegister {
	// bytes are reversed because we send 16 bit words
	// so we send <cmd>00, thus skipping the first byte
	// this way the status register appears reversed

	// snd byte
	Uint16 RDY_BSY_2 : 1;	// R; 0 = Device is ready. 1 = Device is busy with an internal operation.
	Uint16 WEL : 1;			// R; 0 = Device is not write enabled (default). 1 = Device is write enabled.
	Uint16 SWP : 2;			// R; 00 = All sectors are software unprotected (all Sector	Protection Registers are 0).
							// 01 =	Some sectors are software protected.
							// 		Read individual Sector Protection Registers to determine
							// 		which sectors are protected.
							// 10 = Reserved for future use.
							// 11 = All sectors are software protected (all Sector Protection Registers are 1 – default).
	Uint16 WPP : 1;			// R; 0 = nWP is asserted. 1 = nWP is deasserted.
	Uint16 EPE : 1;			// 0 = Erase or program operation was successful. 1 = Erase or program error detected.
	Uint16 rsvd2 : 1;
	Uint16 SPRL : 1;		// R/W; 0 = Sector Protection Registers are unlocked (default).	1 = Sector Protection Registers are locked.

	// fst byte
	Uint16 RDY_BSY_1 : 1;	// R; 0 = Device is ready. 1 = Device is busy with an internal operation.
	Uint16 ES : 1;			// R; 0 = No sectors are erase suspended (default). 1 = A sector is erase suspended.
	Uint16 PS : 1;			// R; 0 = No sectors are program suspended (default). 1 = A sector is program suspended.
	Uint16 SLE : 1;			// R/W; 0 = Sector Lockdown and Freeze Sector Lockdown State commands are disabled (default).
							// 1 = Sector Lockdown and Freeze Sector Lockdown State	commands are enabled.
	Uint16 RSTE : 1;		// R/W; 0 = Reset command is disabled (default). 1 = Reset command is enabled.
	Uint16 rsvd1 : 3;
};

Bool flash_read_status(union FlashStatusRegister *status);

void flash_spi_setup(void);
