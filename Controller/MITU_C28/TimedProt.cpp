/*
 * TimedProt.cpp
 *
 *  Created on: 27 дек. 2016 г.
 *      Author: Volkodav
 */

#include "TimedProt.h"
#include <math.h>

TimedProtection::TimedProtection()
{
}




void TimedProtection::TimedProtectionInit(Uint16 TheMinCyclesToOP, Uint16 TheMaxCyclesToOP, Uint16 TheCyclesToRelease, float TheMinValue, float TheMaxValue)
{
	MinCyclesToOP = TheMinCyclesToOP;
	MaxCyclesToOP = TheMaxCyclesToOP;
	CyclesToRelease = TheCyclesToRelease;
	MinValue = TheMinValue;
	MaxValue = TheMaxValue;
	OperatingTimer = 0;
	ReleasingTimer = 0;
	ProtectionActivated = 0;
	ProtectionActive = 0;
	ProtectionActive_f = 0;
	ProtectionValuesRange = MaxValue - MinValue;
	ProtectionTimesRange = MaxCyclesToOP - MinCyclesToOP;
	if (ProtectionValuesRange==0) {ProtectionValuesRange=1;}
	TimeLimit = TheMaxCyclesToOP;
	CurrentValue = 0;
}

Uint16 TimedProtection::TimedProtectionStep(float theInput)
{
	CurrentValue = theInput;
	if (CurrentValue>MinValue)
		{
		OperatingTimer++;
		if (OperatingTimer>32000) {OperatingTimer=32000;}
		ReleasingTimer = CyclesToRelease;
		ProtectionActivated=1;
		TimeLimit = MinCyclesToOP + (ProtectionTimesRange*(MaxValue - CurrentValue)/ProtectionValuesRange);
		if (TimeLimit<MinCyclesToOP) {TimeLimit = MinCyclesToOP;}
		if (TimeLimit>MaxCyclesToOP) {TimeLimit = MaxCyclesToOP;}
		if (OperatingTimer>TimeLimit) {ProtectionActive=1;ProtectionActive_f=1;}
		}
	else
		{
		if (ReleasingTimer>0) {ReleasingTimer--;}
		if (ReleasingTimer==0) {ProtectionActivated=0;OperatingTimer=0; ProtectionActive=0;ProtectionActive_f=0;}
		}
	return (ProtectionActive);
}

void TimedProtection::TimedProtectionReset(void)
{
	OperatingTimer = 0;
	ReleasingTimer = 0;
	ProtectionActivated = 0;
	ProtectionActive = 0;
	ProtectionActive_f = 0;
	CurrentValue = 0;
}
