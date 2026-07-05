/*
 * ErrorHandling.cpp
 */

#include "ErrorList.h"
#include "MemFunctions.h"


Uint16 ErrorLatch = 0;

void ErrorSet(Uint8 ErrNum) {
	ErrorLatch |= 1 << ErrNum;
	WriteTo_CTOM_MSGRAM(1, ErrorLatch);
}


void ErrorReset(Uint8 ErrNum) {
    ErrorLatch &= ~(1 << ErrNum);
    WriteTo_CTOM_MSGRAM(1, ErrorLatch);
}
