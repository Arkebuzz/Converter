/*
 * ErrorHandling.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: Volkodav
 */

#include "ErrorList.h"
#include "MemFunctions.h"

void ErrorSet(Uint32 ErrNum) {
	WriteTo_CTOM_MSGRAM(1, ErrNum);
}

void ErrorReset(void) {

}
