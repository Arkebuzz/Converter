/*
 * TimedProt.h
 *
 *  Created on: 27 дек. 2016 г.
 *      Author: Volkodav
 */

#ifndef TIMEDPROT_H_
#define TIMEDPROT_H_
typedef unsigned int Uint16;

class TimedProtection
{
public:
	Uint16 ProtectionActive;
	float ProtectionActive_f;
	TimedProtection(); //constructor
	void TimedProtectionInit(Uint16 TheMinCyclesToOP, Uint16 TheMaxCyclesToOP, Uint16 TheCyclesToRelease, float TheMinValue, float TheMaxValue);
	Uint16 TimedProtectionStep(float theInput);
	void TimedProtectionReset(void);

private:

	Uint16 ProtectionActivated;
	Uint16 OperatingTimer;
	Uint16 ReleasingTimer;
	Uint16 MinCyclesToOP;
	Uint16 MaxCyclesToOP;
	Uint16 CyclesToRelease;
	Uint16 TimeLimit;
	float MinValue;
	float MaxValue;
	float ProtectionValuesRange;
	float ProtectionTimesRange;
	float CurrentValue;

};



#endif /* TIMEDPROT_H_ */
