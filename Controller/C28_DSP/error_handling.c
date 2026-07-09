#include "error_handling.h"


static Uint16 Errors = 0;
static Uint16 ErrorsLatch = 0;


Uint16 ErrorGetCurrent() {
	return Errors;
}


Uint16 ErrorGetLatch() {
	return ErrorsLatch;
}


void ErrorSet(Uint8 ErrNum) {
	Errors |= 1 << ErrNum;
	ErrorsLatch |= Errors;
}


void ErrorReset(Uint8 ErrNum) {
	Errors &= ~(1 << ErrNum);
}


void ErrorResetAll() {
	ErrorsLatch = 0;
}
