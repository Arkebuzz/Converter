/*
 * pid.h
 *
 *  Created on: 10.12.2014
 *      Author: VKD
 */

#ifndef PID_H_
#define PID_H_


class FloatPID
{
public:
	float CurrentOutput;

	//constructor
	FloatPID();
	void PIDInit(float thePIDStepSize, float theProportional, float theIntegral, float theDifferential, float theInputGain, float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit, int theStoredValuesNum);
	float PIDStep(float theInput);
	void SetProp(float theProportional);
	void SetInt(float theIntegral);
	void SetDiff(float theDifferential);
	void SetInputGain(float theInputGain);
	void SetPIDStepSize(float thePIDStepSize);

	void SetUpperIntegratorLimit(float theUpperIntegratorLimit);
	void SetLowerIntegratorLimit(float theLowerIntegratorLimit);

	void SetUpperOutputLimit(float theUpperOutputLimit);
	void SetLowerOutputLimit(float theLowerOutputLimit);

	void SetLowerLimit(float theLowerLimit);
	void SetUpperLimit(float theUpperLimit);

	void ResetData(float TheInputState, float TheIntegratorState);
	void SetpointSet(float theSetpoint);
	float GetValue(unsigned int ValueNum);
	int GetCurrentPosition(void);
	void SetCurrentPos(int theCurrentPosition);
	float GetCurrentOutput(void);

	float* ValuesArray;

private:

	volatile float PIDStepSize;

	volatile float Proportional;
	volatile float Differential;
	volatile float Integral;
	volatile float InputGain;



	volatile float UpperIntegratorLimit;
	volatile float LowerIntegratorLimit;
	volatile float UpperOutputLimit;
	volatile float LowerOutputLimit;

	volatile float CurrentInput;
	volatile float Integrator;
	volatile float Setpoint;


	volatile int StoredValuesNum;
	volatile int CurrentPosition;

};


class FloatPID_D
{
public:
	float CurrentOutput;

	//constructor
	FloatPID_D();
	void PIDInit(float thePIDStepSize,
			float theProportional, float theIntegral, float theDifferential, float theInputGain,
			float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit,
			float thePropRateLimit, float theIntRateLimit,
			int theStoredValuesNum, float theDerDenominator0, float theDerDenominator1);
	float PIDStep(float theInput);
	void SetProp(float theProportional);
	void SetInt(float theIntegral);
	void SetDiff(float theDifferential);
	void SetInputGain(float theInputGain);
	void SetPIDStepSize(float thePIDStepSize);

	void SetUpperIntegratorLimit(float theUpperIntegratorLimit);
	void SetLowerIntegratorLimit(float theLowerIntegratorLimit);

	void SetUpperOutputLimit(float theUpperOutputLimit);
	void SetLowerOutputLimit(float theLowerOutputLimit);

	void SetLowerLimit(float theLowerLimit);
	void SetUpperLimit(float theUpperLimit);

	void ResetData(float TheInputState, float TheIntegratorState);
	void SetpointSet(float theSetpoint);
	float GetValue(unsigned int ValueNum);
	int GetCurrentPosition(void);
	void SetCurrentPos(int theCurrentPosition);
	float GetCurrentOutput(void);

	float* ValuesArray;

private:

	volatile float PIDStepSize;

	volatile float Proportional;
	volatile float Differential;
	volatile float Integral;
	volatile float InputGain;

	volatile float DerivativeDenominator0;
	volatile float DerivativeDenominator1;

	volatile float DerivativeState;
	volatile float DerivativeTmp;

	volatile float UpperIntegratorLimit;
	volatile float LowerIntegratorLimit;
	volatile float UpperOutputLimit;
	volatile float LowerOutputLimit;

	volatile float PropRateLimit;
	volatile float IntRateLimit;

	volatile float PropRateLimit_lower;
	volatile float IntRateLimit_lower;


	volatile float CurrentInput;
	volatile float Integrator;
	volatile float Setpoint;


	volatile int StoredValuesNum;
	volatile int CurrentPosition;

};


class FloatCyclicPID
{
public:
	//constructor
	FloatCyclicPID();
	float CurrentOutput;
	void PIDInit(float thePIDStepSize,
			float theProportional, float theIntegral, float theDifferential, float theInputGain,
			float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit,
			float thePropRateLimit, float theIntRateLimit,
			int theStoredValuesNum);

	float PIDStep(float theInput);
	void SetProp(float theProportional);
	void SetInt(float theIntegral);
	void SetDiff(float theDifferential);
	void SetInputGain(float theInputGain);
	void SetPIDStepSize(float thePIDStepSize);

	void SetUpperIntegratorLimit(float theUpperIntegratorLimit);
	void SetLowerIntegratorLimit(float theLowerIntegratorLimit);

	void SetUpperOutputLimit(float theUpperOutputLimit);
	void SetLowerOutputLimit(float theLowerOutputLimit);

	void ResetData(float TheInputState, float TheIntegratorState);
	void SetpointSet(float theSetpoint);
	float GetValue(unsigned int ValueNum);
	int GetCurrentPosition(void);
	void SetCurrentPos(int theCurrentPosition);
	float GetCurrentOutput(void);

	float* ValuesArray;

private:

	volatile float PIDStepSize;

	volatile float Proportional;
	volatile float Differential;
	volatile float Integral;
	volatile float InputGain;

	volatile float UpperIntegratorLimit;
	volatile float LowerIntegratorLimit;
	volatile float UpperOutputLimit;
	volatile float LowerOutputLimit;

	volatile float PropRateLimit;
	volatile float IntRateLimit;

	volatile float PropRateLimit_lower;
	volatile float IntRateLimit_lower;

	volatile float CurrentInput;
	volatile float Integrator;
	volatile float Setpoint;

	volatile int StoredValuesNum;
	volatile int CurrentPosition;
};

class FloatBipolarPID
{
public:
	//constructor
	FloatBipolarPID();
	float CurrentOutput;
	void PIDInit(float thePIDStepSize, float theProportional, float theIntegral, float theDifferential, float theInputGainPos, float theInputGainNeg, float theUpperIntegratorLimit, float theLowerIntegratorLimit, float theUpperOutputLimit, float theLowerOutputLimit, int theStoredValuesNum);
	float PIDStep(float theInput);
	void SetProp(float theProportional);
	void SetInt(float theIntegral);
	void SetDiff(float theDifferential);
	void SetPIDStepSize(float thePIDStepSize);
	void SetPosInputGain(float theInputGain);
	void SetNegInputGain(float theInputGain);

	void SetUpperIntegratorLimit(float theUpperIntegratorLimit);
	void SetLowerIntegratorLimit(float theLowerIntegratorLimit);

	void SetUpperOutputLimit(float theUpperOutputLimit);
	void SetLowerOutputLimit(float theLowerOutputLimit);

	void SetLowerLimit(float theLowerLimit);
	void SetUpperLimit(float theUpperLimit);

	void ResetData(float TheInputState, float TheIntegratorState);
	void SetpointSet(float theSetpoint);
	float GetValue(unsigned int ValueNum);
	int GetCurrentPosition(void);
	void SetCurrentPos(int theCurrentPosition);
	float GetCurrentOutput(void);

	float* ValuesArray;

private:

	volatile float PIDStepSize;

	volatile float Proportional;
	volatile float Differential;
	volatile float Integral;
	volatile float InputGainPos;
	volatile float InputGainNeg;

	volatile float UpperIntegratorLimit;
	volatile float LowerIntegratorLimit;
	volatile float UpperOutputLimit;
	volatile float LowerOutputLimit;

	volatile float CurrentInput;
	volatile float Integrator;
	volatile float Setpoint;


	volatile int StoredValuesNum;
	volatile int CurrentPosition;

};


#endif /* PID_H_ */

