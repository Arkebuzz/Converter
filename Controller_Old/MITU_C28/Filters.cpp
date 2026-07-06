/*
 * Filters.cpp
 *
 *  Created on: 09.01.2015
 *      Author: VKD
 */

#include "Filters.h"

SecOrdFilter::SecOrdFilter()
{
}

void SecOrdFilter::SecOrdFilterInit(float theA0, float theA1, float theA2, float theA3, float theB0, float theB1, float theC0, float theC1, float theD)
{
CurrentOutput = 0;
A0 = theA0;
A1 = theA1;
A2 = theA2;
A3 = theA3;

B0 = theB0;
B1 = theB1;

C0 = theC0;
C1 = theC1;

X0 = 0;
X1 = 0;

D = theD;
}

void SecOrdFilter::Input(float theInput)
{
	CurrentOutput = C0*X0+C1*X1+D*theInput;
	float X0t = A0*X0+A1*X1+B0*theInput;
	float X1t = A2*X0+A3*X1+B1*theInput;
	X0 = X0t;
	X1 = X1t;
}

void SecOrdFilter::ResetData()
{
	CurrentOutput = 0;
	X0 = 0;
	X1 = 0;
}

