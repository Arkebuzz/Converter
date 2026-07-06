/*
 * pid.cpp
 *
 *  Created on: 10.12.2014
 *      Author: VKD
 */

#include "PID.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

FloatPID::FloatPID()
{
}

void FloatPID::PIDInit(float thePIDStepSize, float theProportional, float theIntegral, float theDifferential, float theInputGain, float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit, int theStoredValuesNum)
{
CurrentOutput = 0;
PIDStepSize = thePIDStepSize;
Proportional = theProportional;
Differential = theDifferential;
Integral = theIntegral;
InputGain = theInputGain;
UpperIntegratorLimit = theUpperIntegratorLimit;
LowerIntegratorLimit = theLowerIntegratorLimit;
UpperOutputLimit = theUpperOutputLimit;
LowerOutputLimit = theLowerOutputLimit;
StoredValuesNum = theStoredValuesNum;
CurrentPosition = 0;
}

float FloatPID::PIDStep(float theInput)
{
	CurrentInput=InputGain*(theInput);
	Integrator = Integral*PIDStepSize*CurrentInput+Integrator;
	if (Integrator>UpperIntegratorLimit) {Integrator=UpperIntegratorLimit;}
	if (Integrator<LowerIntegratorLimit) {Integrator=LowerIntegratorLimit;}
	float Output = Integrator + CurrentInput*Proportional;
	if (Output>UpperOutputLimit) {Output = UpperOutputLimit;}
	if (Output<LowerOutputLimit) {Output = LowerOutputLimit;}
	CurrentOutput = Output;
	return (Output);
}

void FloatPID::SetProp(float theProportional)
{
	Proportional = theProportional;
}

void FloatPID::SetInt(float theIntegral)
{
	Integral = theIntegral;
}

void FloatPID::SetDiff(float theDifferential)
{
	Differential = theDifferential;
}

void FloatPID::SetInputGain(float theInputGain)
{
	InputGain = theInputGain;
}

void FloatPID::SetPIDStepSize(float thePIDStepSize)
{
	PIDStepSize = thePIDStepSize;
}

void FloatPID::SetUpperIntegratorLimit(float theUpperIntegratorLimit)
{
	UpperIntegratorLimit = theUpperIntegratorLimit;
}

void FloatPID::SetLowerIntegratorLimit(float theLowerIntegratorLimit)
{
	LowerIntegratorLimit = theLowerIntegratorLimit;
}

void FloatPID::SetLowerLimit(float theLowerLimit)
{
	LowerIntegratorLimit = theLowerLimit;
	LowerOutputLimit=theLowerLimit;
}

void FloatPID::SetUpperLimit(float theUpperLimit)
{
	UpperIntegratorLimit = theUpperLimit;
	UpperOutputLimit=theUpperLimit;
}

void FloatPID::SetUpperOutputLimit(float theUpperOutputLimit)
{
	UpperOutputLimit=theUpperOutputLimit;
}

void FloatPID::SetLowerOutputLimit(float theLowerOutputLimit)
{
	LowerOutputLimit=theLowerOutputLimit;
}

void FloatPID::ResetData(float TheInputState, float TheIntegratorState)
{
	CurrentInput = TheInputState;
	Integrator = TheIntegratorState;
	if (Integrator>UpperIntegratorLimit) {Integrator=UpperIntegratorLimit;}
	if (Integrator<LowerIntegratorLimit) {Integrator=LowerIntegratorLimit;}
	float Output = Integrator + CurrentInput*Proportional;
	if (Output>UpperOutputLimit) {Output = UpperOutputLimit;}
	if (Output<LowerOutputLimit) {Output = LowerOutputLimit;}
	CurrentOutput = Output;
}

void FloatPID::SetpointSet(float theSetpoint)
{
	Setpoint=theSetpoint;
}

int FloatPID::GetCurrentPosition(void)
{
return CurrentPosition;
}

void FloatPID::SetCurrentPos(int theCurrentPosition)
{
	CurrentPosition = theCurrentPosition;
}

float FloatPID::GetCurrentOutput()
{
return CurrentOutput;
}

//------------------------------------------------------------------------------------

FloatPID_D::FloatPID_D()
{
}

void FloatPID_D::PIDInit(float thePIDStepSize,
		float theProportional, float theIntegral, float theDifferential, float theInputGain,
		float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit,
		float thePropRateLimit, float theIntRateLimit,
		int theStoredValuesNum, float theDerDenominator0, float theDerDenominator1)
{
PropRateLimit = thePropRateLimit;
IntRateLimit = theIntRateLimit;
PropRateLimit_lower = -1*thePropRateLimit;
IntRateLimit_lower = -1*theIntRateLimit;
CurrentOutput = 0;
PIDStepSize = thePIDStepSize;
Proportional = theProportional;
Differential = theDifferential;
Integral = theIntegral;
InputGain = theInputGain;
UpperIntegratorLimit = theUpperIntegratorLimit;
LowerIntegratorLimit = theLowerIntegratorLimit;
UpperOutputLimit = theUpperOutputLimit;
LowerOutputLimit = theLowerOutputLimit;
StoredValuesNum = theStoredValuesNum;
CurrentPosition = 0;
DerivativeDenominator0 = theDerDenominator0;
DerivativeDenominator1 = theDerDenominator1;
DerivativeState = 0;
DerivativeTmp=0;
}

float FloatPID_D::PIDStep(float theInput)
{
	CurrentInput=InputGain*(theInput);

	float ProportionaPart = CurrentInput*Proportional;
	if (ProportionaPart>PropRateLimit) {ProportionaPart = PropRateLimit;}
	if (ProportionaPart<PropRateLimit_lower) {ProportionaPart = PropRateLimit_lower;}
	float IntegralPart = Integral*CurrentInput;
	if (IntegralPart>IntRateLimit) {IntegralPart = IntRateLimit;}
	if (IntegralPart<IntRateLimit_lower) {IntegralPart = IntRateLimit_lower;}
	Integrator = IntegralPart*PIDStepSize+Integrator;
	//DerivativeTmp = (CurrentInput*Differential - DerivativeDenominator1 * DerivativeState)/DerivativeDenominator0;
	if (Integrator>UpperIntegratorLimit) {Integrator=UpperIntegratorLimit;}
	if (Integrator<LowerIntegratorLimit) {Integrator=LowerIntegratorLimit;}
	float Output = Integrator + ProportionaPart ; //+ (2*DerivativeTmp - 2*DerivativeState);
	if (Output>UpperOutputLimit) {Output = UpperOutputLimit;}
	if (Output<LowerOutputLimit) {Output = LowerOutputLimit;}
	//DerivativeState = DerivativeTmp;
	CurrentOutput = Output;
	return (Output);
}

void FloatPID_D::SetProp(float theProportional)
{
	Proportional = theProportional;
}

void FloatPID_D::SetInt(float theIntegral)
{
	Integral = theIntegral;
}

void FloatPID_D::SetDiff(float theDifferential)
{
	Differential = theDifferential;
}

void FloatPID_D::SetInputGain(float theInputGain)
{
	InputGain = theInputGain;
}

void FloatPID_D::SetPIDStepSize(float thePIDStepSize)
{
	PIDStepSize = thePIDStepSize;
}

void FloatPID_D::SetUpperIntegratorLimit(float theUpperIntegratorLimit)
{
	UpperIntegratorLimit = theUpperIntegratorLimit;
}

void FloatPID_D::SetLowerIntegratorLimit(float theLowerIntegratorLimit)
{
	LowerIntegratorLimit = theLowerIntegratorLimit;
}

void FloatPID_D::SetLowerLimit(float theLowerLimit)
{
	LowerIntegratorLimit = theLowerLimit;
	LowerOutputLimit=theLowerLimit;
}

void FloatPID_D::SetUpperLimit(float theUpperLimit)
{
	UpperIntegratorLimit = theUpperLimit;
	UpperOutputLimit=theUpperLimit;
}

void FloatPID_D::SetUpperOutputLimit(float theUpperOutputLimit)
{
	UpperOutputLimit=theUpperOutputLimit;
}

void FloatPID_D::SetLowerOutputLimit(float theLowerOutputLimit)
{
	LowerOutputLimit=theLowerOutputLimit;
}

void FloatPID_D::ResetData(float TheInputState, float TheIntegratorState)
{
	CurrentInput = TheInputState;
	Integrator = TheIntegratorState;
	DerivativeState = 0;
	DerivativeTmp=0;
	if (Integrator>UpperIntegratorLimit) {Integrator=UpperIntegratorLimit;}
	if (Integrator<LowerIntegratorLimit) {Integrator=LowerIntegratorLimit;}
	float Output = Integrator + CurrentInput*Proportional;
	if (Output>UpperOutputLimit) {Output = UpperOutputLimit;}
	if (Output<LowerOutputLimit) {Output = LowerOutputLimit;}
	CurrentOutput = Output;
}

void FloatPID_D::SetpointSet(float theSetpoint)
{
	Setpoint=theSetpoint;
}

int FloatPID_D::GetCurrentPosition(void)
{
return CurrentPosition;
}

void FloatPID_D::SetCurrentPos(int theCurrentPosition)
{
	CurrentPosition = theCurrentPosition;
}

float FloatPID_D::GetCurrentOutput()
{
return CurrentOutput;
}

//------------------------------------------------------------------------------------



FloatCyclicPID::FloatCyclicPID()
{
}

void FloatCyclicPID::PIDInit(float thePIDStepSize,
		float theProportional, float theIntegral, float theDifferential, float theInputGain,
		float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit,
		float thePropRateLimit, float theIntRateLimit,
		int theStoredValuesNum)
{
PropRateLimit = thePropRateLimit;
IntRateLimit = theIntRateLimit;
PropRateLimit_lower = -1*thePropRateLimit;
IntRateLimit_lower = -1*theIntRateLimit;
CurrentOutput = 0;
PIDStepSize = thePIDStepSize;
Proportional = theProportional;
Differential = theDifferential;
Integral = theIntegral;
InputGain = theInputGain;
UpperIntegratorLimit = theUpperIntegratorLimit;
LowerIntegratorLimit = theLowerIntegratorLimit;
UpperOutputLimit = theUpperOutputLimit;
LowerOutputLimit = theLowerOutputLimit;
StoredValuesNum = theStoredValuesNum;
CurrentPosition = 0;
}

float FloatCyclicPID::PIDStep(float theInput)
{
	CurrentInput=InputGain*(theInput);
	float ProportionaPart = CurrentInput*Proportional;
	if (ProportionaPart>PropRateLimit) {ProportionaPart = PropRateLimit;}
	if (ProportionaPart<PropRateLimit_lower) {ProportionaPart = PropRateLimit_lower;}
	float IntegralPart = Integral*CurrentInput;
	if (IntegralPart>IntRateLimit) {IntegralPart = IntRateLimit;}
	if (IntegralPart<IntRateLimit_lower) {IntegralPart = IntRateLimit_lower;}
	Integrator = IntegralPart*PIDStepSize+Integrator;
	if (Integrator>UpperIntegratorLimit) {Integrator=Integrator - UpperIntegratorLimit + LowerIntegratorLimit;}
	if (Integrator<LowerIntegratorLimit) {Integrator=Integrator + UpperIntegratorLimit - LowerIntegratorLimit;}
	float Output = Integrator + ProportionaPart;
	if (Output>UpperOutputLimit) {Output = Output - UpperOutputLimit + LowerOutputLimit;}
	if (Output<LowerOutputLimit) {Output = Output + UpperOutputLimit - LowerOutputLimit;}
	CurrentOutput = Output;
	return (Output);
}

float FloatCyclicPID::GetCurrentOutput()
{
return CurrentOutput;
}

void FloatCyclicPID::SetProp(float theProportional)
{
	Proportional = theProportional;
}

void FloatCyclicPID::SetInt(float theIntegral)
{
	Integral = theIntegral;
}

void FloatCyclicPID::SetDiff(float theDifferential)
{
	Differential = theDifferential;
}

void FloatCyclicPID::SetInputGain(float theInputGain)
{
	InputGain = theInputGain;
}

void FloatCyclicPID::SetPIDStepSize(float thePIDStepSize)
{
	PIDStepSize = thePIDStepSize;
}

void FloatCyclicPID::SetUpperIntegratorLimit(float theUpperIntegratorLimit)
{
	UpperIntegratorLimit = theUpperIntegratorLimit;
}

void FloatCyclicPID::SetLowerIntegratorLimit(float theLowerIntegratorLimit)
{
	LowerIntegratorLimit = theLowerIntegratorLimit;
}

void FloatCyclicPID::SetUpperOutputLimit(float theUpperOutputLimit)
{
	UpperOutputLimit=theUpperOutputLimit;
}

void FloatCyclicPID::SetLowerOutputLimit(float theLowerOutputLimit)
{
	LowerOutputLimit=theLowerOutputLimit;
}

void FloatCyclicPID::ResetData(float TheInputState, float TheIntegratorState)
{
	CurrentInput = TheInputState;
	Integrator = TheIntegratorState;
	if (Integrator>UpperIntegratorLimit) {Integrator=Integrator - UpperIntegratorLimit + LowerIntegratorLimit;}
	if (Integrator<LowerIntegratorLimit) {Integrator=Integrator + UpperIntegratorLimit - LowerIntegratorLimit;}
	float Output = Integrator + CurrentInput*Proportional;
	if (Output>UpperOutputLimit) {Output = Output - UpperOutputLimit + LowerOutputLimit;}
	if (Output<LowerOutputLimit) {Output = Output + UpperOutputLimit - LowerOutputLimit;}
	CurrentOutput = Output;
}

void FloatCyclicPID::SetpointSet(float theSetpoint)
{
	Setpoint=theSetpoint;
}

int FloatCyclicPID::GetCurrentPosition(void)
{
return CurrentPosition;
}

void FloatCyclicPID::SetCurrentPos(int theCurrentPosition)
{
	CurrentPosition = theCurrentPosition;
}

//----------------------------------------------------------------------------------------
FloatBipolarPID::FloatBipolarPID()
{
}

void FloatBipolarPID::PIDInit(float thePIDStepSize, float theProportional, float theIntegral, float theDifferential, float theInputGainPos, float theInputGainNeg, float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit, int theStoredValuesNum)
{
CurrentOutput = 0;
PIDStepSize = thePIDStepSize;
Proportional = theProportional;
Differential = theDifferential;
Integral = theIntegral;
InputGainPos = theInputGainPos;
InputGainNeg = theInputGainNeg;
UpperIntegratorLimit = theUpperIntegratorLimit;
LowerIntegratorLimit = theLowerIntegratorLimit;
UpperOutputLimit = theUpperOutputLimit;
LowerOutputLimit = theLowerOutputLimit;
StoredValuesNum = theStoredValuesNum;
CurrentPosition = 0;
}



float FloatBipolarPID::PIDStep(float theInput)
{
	if (theInput>0) {CurrentInput=InputGainPos*(theInput);}
	if (theInput<0) {CurrentInput=InputGainNeg*(theInput);}
	Integrator = Integral*PIDStepSize*CurrentInput+Integrator;
	if (Integrator<LowerIntegratorLimit) {Integrator=LowerIntegratorLimit;}
	if (Integrator>UpperIntegratorLimit) {Integrator=UpperIntegratorLimit;}
	float Output = Integrator + CurrentInput*Proportional;
	if (Output<LowerOutputLimit) {Output = LowerOutputLimit;}
	if (Output>UpperOutputLimit) {Output = UpperOutputLimit;}
	CurrentOutput = Output;
	return (Output);
}

void FloatBipolarPID::SetProp(float theProportional)
{
	Proportional = theProportional;
}

void FloatBipolarPID::SetInt(float theIntegral)
{
	Integral = theIntegral;
}

void FloatBipolarPID::SetDiff(float theDifferential)
{
	Differential = theDifferential;
}

void FloatBipolarPID::SetPosInputGain(float theInputGain)
{
	InputGainPos = theInputGain;
}

void FloatBipolarPID::SetNegInputGain(float theInputGain)
{
	InputGainNeg = theInputGain;
}

void FloatBipolarPID::SetPIDStepSize(float thePIDStepSize)
{
	PIDStepSize = thePIDStepSize;
}

void FloatBipolarPID::SetUpperIntegratorLimit(float theUpperIntegratorLimit)
{
	UpperIntegratorLimit = theUpperIntegratorLimit;
}

void FloatBipolarPID::SetLowerIntegratorLimit(float theLowerIntegratorLimit)
{
	LowerIntegratorLimit = theLowerIntegratorLimit;
}

void FloatBipolarPID::SetUpperOutputLimit(float theUpperOutputLimit)
{
	UpperOutputLimit=theUpperOutputLimit;
}

void FloatBipolarPID::SetLowerOutputLimit(float theLowerOutputLimit)
{
	LowerOutputLimit=theLowerOutputLimit;
}

void FloatBipolarPID::SetLowerLimit(float theLowerLimit)
{
	LowerIntegratorLimit = theLowerLimit;
	LowerOutputLimit=theLowerLimit;
}

void FloatBipolarPID::SetUpperLimit(float theUpperLimit)
{
	UpperIntegratorLimit = theUpperLimit;
	UpperOutputLimit=theUpperLimit;
}


void FloatBipolarPID::ResetData(float TheInputState, float TheIntegratorState)
{
	CurrentInput = TheInputState;
	Integrator = TheIntegratorState;
	if (Integrator>UpperIntegratorLimit) {Integrator=UpperIntegratorLimit;}
	if (Integrator<LowerIntegratorLimit) {Integrator=LowerIntegratorLimit;}
	float Output = Integrator + CurrentInput*Proportional;
	if (Output>UpperOutputLimit) {Output = UpperOutputLimit;}
	if (Output<LowerOutputLimit) {Output = LowerOutputLimit;}
	CurrentOutput = Output;
}

void FloatBipolarPID::SetpointSet(float theSetpoint)
{
	Setpoint=theSetpoint;
}


int FloatBipolarPID::GetCurrentPosition(void)
{
return CurrentPosition;
}

void FloatBipolarPID::SetCurrentPos(int theCurrentPosition)
{
	CurrentPosition = theCurrentPosition;
}

float FloatBipolarPID::GetCurrentOutput()
{
return CurrentOutput;
}

//------------------------------------------------------------------------------------



