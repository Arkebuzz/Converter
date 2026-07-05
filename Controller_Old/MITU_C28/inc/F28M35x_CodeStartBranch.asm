; //###########################################################################
; // TITLE: Branch for redirecting code execution after boot.
; // For these examples, code_start is the first code that is executed after
; // exiting the boot ROM code.
; //
; // The codestart section in the linker cmd file is used to physically place
; // this code at the correct memory location.  This section should be placed
; // at the location the BOOT ROM will re-direct the code to.  For example,
; // for boot to FLASH this code will be located at 0x13FFF0.
; //
; // In addition, the example F28M35x projects are setup such that the codegen
; // entry point is also set to the code_start label.  This is done by linker
; // option -e in the project build options.  When the debugger loads the code,
; // it will automatically set the PC to the "entry point" address indicated by
; // the -e linker option.  In this case the debugger is simply assigning the
; // PC, it is not the same as a full reset of the device.
; //
; // The compiler may warn that the entry point for the project is other then
; //  _c_init00.  _c_init00 is the C environment setup and is run before
; // main() is entered. The code_start code will re-direct the execution
; // to _c_init00 and thus there is no worry and this warning can be ignored.
; //###########################################################################
; // $TI Release: F28M35x Support Library v202 $
; // $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
; //###########################################################################

***********************************************************************

    .ref _c_int00
    .global code_start

***********************************************************************
*Function : codestart section
*
*Description : Branch to code starting point
***********************************************************************

    .sect "codestart"

code_start:
    LB _c_int00         ; Branch to start of boot.asm in RTS library
                        ; end codestart section




